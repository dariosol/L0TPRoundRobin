#include <stdio.h>
#include <iostream>

#include <arpa/inet.h>
#include <dic.hxx>

#include "user2.h"
#include "loopethernet.h"

#include "CustomMEP.h"

using namespace std;

int npacket_no;
int old_dim_sob;
int old_dim_eob;
int old_dim_phy;
int new_eob;
int *evnum;
float *evtts;
int length_received;
int length_sent;
int nIP=1;
int packets_received = 0;
int packets_per_burst = 0;
int packets_sent = 0;

#define debug 0
#define debug_rate 0

#define store_size 4026530000
#define buf_scale 1000
#define tout 1000000.

#define packet_offset 0

#define PORT_DE4 58913
#define PORT_FARM 58913 
#define PACKET_TYPE_OFFSET 45
int address_skip =1;

class Burst : public DimClient {
    DimUpdatedInfo Sob;
    DimUpdatedInfo Eob;
     public:
    Burst(): Sob("NA62/Timing/SOB",-1,this), Eob("NA62/Timing/EOB",-1,this) {InBurst = 0;}
    int GetInBurst(){return InBurst;}

    void infoHandler() {
        DimUpdatedInfo *curr = (DimUpdatedInfo *) getInfo();
        if (curr==&Sob) {
            cout << "SOB received " << curr->getInt() << endl;
            InBurst = 1;
        }
        if (curr==&Eob) {
            cout << "EOB received " << curr->getInt() << endl;
            cout << endl;
            InBurst = 0;
            nIP = 1;
            printf("packet received %d\n",packets_received);
            printf("packet sent %d\n",packets_sent);

            packets_received =0;
            packets_sent =0;
        }
    }
    int InBurst;
};

class FakeDim {
    public:
    int GetInBurst(){return 1;}
};

/*
 * Show messages in run control
 */
static void displayError(const char *on_what) {
    fputs(strerror(errno),stderr);
    fputs(": ",stderr);
    fputs(on_what,stderr);
    fputc('\n',stderr);
    exit(1);
}

/*
 * Show messages in run control
 */
void sighandler(int i){
    cout<<"I have been killed"<<endl;
    if (i == SIGINT || i == SIGSEGV || 1) {
        //printf("----  %d primitive packets received\n", npacket_phy_tot);
        //you can print here your burst statistic
    }
    if (i != -1)
        exit(1);
}

int main(int argc, char **argv){
    int aaa=1;

    //Burst Timing;
    FakeDim Timing;

    int InBurst = 0;

    /*
     * Print all ip address
     * set from l0server (Run Control)
     */
    printf("argv size: %d\n", argc);
    for (int i=0; i < argc; i++) {
        printf("argv: %s\n", argv[i]);
    }

    /*
     * Create a UDP socket to use:
     * Receive from FPGA
     */
    fromDE4 = socket(AF_INET,SOCK_DGRAM, 0);
    if (fromDE4 == -1) {
        displayError("Error opening socket to fpga");
    }

    /*---RECEIVING FROM DE4------------*/
    memset(&adr_inet, 0, sizeof adr_inet);
    adr_inet.sin_family = AF_INET;
    adr_inet.sin_port = htons(PORT_DE4);

    adr_inet.sin_addr.s_addr =  inet_addr("192.168.1.20");//Addres of this computer

    if (adr_inet.sin_addr.s_addr == INADDR_NONE) {
        displayError("bad address receiving.");
    }
    len_inet = sizeof adr_inet;

    z = bind(fromDE4, (struct sockaddr *)&adr_inet, len_inet);
    if ( z == -1 ) {
        displayError("read primitives already running");
    }

    /**************SENDING TO FARM*********************/
    toFARM = socket(AF_INET,SOCK_DGRAM,0);
    if (toFARM == -1) {
        displayError("Error opening socket to farm");
    }

    bzero(&adr_inet1, sizeof(adr_inet1));
    adr_inet1.sin_family = AF_INET;
    adr_inet1.sin_port = htons(PORT_FARM);

    adr_inet1.sin_addr.s_addr =  inet_addr(argv[nIP]);

    if (adr_inet1.sin_addr.s_addr == INADDR_NONE) {
        displayError("bad address sending.");
    }
    len_inet1 = sizeof adr_inet1;
    //bind(toFARM, (struct sockaddr *)&adr_inet1,len_inet1);

    /*******************RECEIVING FROM DE4***********************/
    signal(SIGINT, sighandler);

    //cout<<"read to receive from de4"<<endl;

    //To check the stream order
    bool is_event_id_aligned = false;
    bool is_stream_broken = false;
    int flow_break_count = 0;

    //Na62-farm-lib-function
    //na62::DataContainer container;
    char * primitive_pointer = nullptr;
    na62::l0::CustomMEP* mep = nullptr;
    //CustomMEP* mep = nullptr;
    int_fast16_t mep_factor_temp;
    uint_fast32_t first_event_number_temp;
    uint_fast32_t expected_first_event_number = 0;

    while(1){
           InBurst = 1;
           new_eob = 0;
           old_dim_sob = 0;
           old_dim_phy = 0;
           old_dim_eob = 0;

           while(aaa){
               length_received = recvfrom(fromDE4,
                       primitive,
                       2048, // Max recv buf size   */
                       0,
                       (struct sockaddr *) &adr_clnt,
                       (socklen_t*) &adr_clnt
               );

               //cout << "Packet length: "<< length_received << endl;

               if ( length_received < 0 ) displayError("recvfrom(2)");

               if(length_received!=0 && length_received!=-1) {
                   ++packets_received;
                   ++packets_per_burst;
               }

               primitive_pointer = primitive;

               mep = new na62::l0::CustomMEP(primitive_pointer, length_received);
               first_event_number_temp = mep->getFirstEventNum();
               mep_factor_temp = mep->getNumberOfFragments();

               //understand if a new burst is started
               if (first_event_number_temp < expected_first_event_number) {
                   is_event_id_aligned = false;
                   cout<<"New burst is started "<<endl;
                   cout<<"Packets received previous burst: "<<packets_per_burst<<endl
                       <<"Broken flow packets previous burst: "<< flow_break_count<<endl
                       <<"Total packets received: "<<packets_received<<endl
                       <<endl;

                   packets_per_burst = 0;
                   flow_break_count = 0;
               }

               //Check flow consistency
               if(!is_event_id_aligned){
                   is_event_id_aligned = true;
                   //cout<<"Mp recognised: "<< dec << mep_factor_temp <<endl;
                   expected_first_event_number = first_event_number_temp;
               } else {
                   expected_first_event_number = expected_first_event_number + mep_factor_temp;
               }

               if (first_event_number_temp > expected_first_event_number) {
                   is_event_id_aligned = false;
                   is_stream_broken = true;
                   //cout<<"Stream Broken at packet: "<< dec <<packets_per_burst << " expecting: "<< expected_first_event_number << " instead: " << first_event_number_temp <<endl;
                   //cout<<"mep multiple? "<< abs((double) expectedfirst_event_number - (double) first_event_number_temp)/(double)mep_factor_temp<<endl;
                   ++flow_break_count;
               }
               delete mep;





               // hexdump((void*)&primitive,40);
               // cout<<"received from de4"<<endl;

               /******************SENDING TO FARM****************************/
               adr_inet1.sin_addr.s_addr =  inet_addr(argv[nIP]);
               length_sent = sendto(toFARM,
                       primitive,
                       length_received,
                       0,
                       (struct sockaddr *) &adr_inet1,
                       sizeof(adr_inet1)
               );

               //    hexdump((void*)&primitive,40);
               if(length_sent!=0 && length_sent!=-1) {
                   ++packets_sent;
               } else {
                   cout<<"packet not sent"<<endl;
               }
               //cout<<"nIP: "<<argv[nIP]<<" packets_sent: "<<packets_sent<<endl;

               /*
                * Loop on all packets
                * and set back to 1
                */
               if (address_skip == 0) {
                   if (packets_sent%address_skip == 0 || packets_sent == 1) {
                       if (nIP != argc-1) {
                           ++nIP;
                       } else {
                           nIP = 1;
                       }
                   }
               } else {
                   if (packets_sent%address_skip == 0) {
                       if(nIP!=argc-1) {
                           ++nIP;
                       }else{
                           nIP = 1;
                       }
                   }
               }

               if ( length_sent < 0 ) {
                   displayError("sendto(2)");
               }

               if(!Timing.GetInBurst() || old_dim_phy/4 >= store_size - 20) {
                   break;
               }
           } //end of while(aaa)

        if(!Timing.GetInBurst() && InBurst){
            InBurst = 0;
        }
    }
return 0;
}

void hexdump(const void * buf, size_t size)
{
    const unsigned char * cbuf = (const unsigned char *) buf;
    //const unsigned char * cbuf =0;
    const unsigned long BYTES_PER_LINE = 16;
    unsigned long offset, minioffset;

    for (offset = 0; offset < size; offset += BYTES_PER_LINE)
    {
        // OFFSETXX  xx xx xx xx xx xx xx xx  xx xx . . .
        //     . . . xx xx xx xx xx xx   abcdefghijklmnop
        //printf("%08x  ", offset);
        for (minioffset = offset;
                minioffset < offset + BYTES_PER_LINE;
                minioffset++)
        {
            if (minioffset - offset == (BYTES_PER_LINE / 2)) {
                printf(" ");
            }

            if (minioffset < size) {
                printf("%02x", cbuf[minioffset]);
            } else {
                printf("   ");
            }
        }
        printf("  ");

        for (minioffset = offset;
                minioffset < offset + BYTES_PER_LINE;
                minioffset++)
        {
            if (minioffset >= size)
                break;

            if (cbuf[minioffset] < 0x20 ||
                    cbuf[minioffset] > 0x7e)
            {
                //printf(".");
            } else {
                // printf("%c", cbuf[minioffset]);
            }
        }
        //   printf("\n");
    }
}
