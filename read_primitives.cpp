#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <netinet/if_ether.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <signal.h>
#include <errno.h>
#include "loopethernet.h"
#include <dic.hxx>
#include <iostream>

#include "user2.h"

//Na62-farm
#include "structs/DataContainer.h"
#include <socket/EthernetUtils.h>
#include "l0/MEP.h"
//end Na62-farm

using namespace std;

char str[80];
char str_res[10][80];

int *tsbuffer0_phy;
int *tempbuffer;
int tsbuffer_length_phy[300000];
int nbad;
int npacket_sob;
int npacket_eob;
int npacket_phy;
int npacket_phy_tot;
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
int numberofpackets_received = 0;
int numberofpackets_sent = 0;

char sdate[150];
char file_output_name[150];

int ieth;
int eob;

#define debug 0
#define debug_rate 0

#define store_size 4026530000
#define buf_scale 1000
#define tout 1000000.

#define packet_offset 0

#define PORTA 58913
#define PORTA_FARM 58913 
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
            printf("packet received %d\n",numberofpackets_received);
            printf("packet sent %d\n",numberofpackets_sent);

            numberofpackets_received =0;
            numberofpackets_sent =0;
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
    if (i == SIGINT || i == SIGSEGV || 1) {
        printf("----  %d primitive packets received\n",npacket_phy_tot);
        printf("SoB Packets: %d\n", npacket_sob);
        printf("Physics Packets: %d\n", npacket_phy_tot);
        printf("EoB Packets: %d\n", npacket_eob);
        printf("Number of Bad file descriptor errors: %d\n", nbad);
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
    adr_inet.sin_port = htons(58913);
    adr_inet.sin_addr.s_addr =  inet_addr("192.168.1.20");//Addres of this computer

    if (adr_inet.sin_addr.s_addr == INADDR_NONE) {
        displayError("bad address.");
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
    adr_inet1.sin_port = htons(58913);
    adr_inet1.sin_addr.s_addr =  inet_addr(argv[nIP]);

    if (adr_inet1.sin_addr.s_addr == INADDR_NONE) {
        displayError("bad address sending.");
    }
    len_inet1 = sizeof adr_inet1;
    //bind(toFARM, (struct sockaddr *)&adr_inet1,len_inet1);

    /*******************RECEIVING FROM DE4***********************/
    for (int g = 0; g < 10000; g++){
        tsbuffer_length_phy[g]= -1;
    }
    signal(SIGINT, sighandler);

    //       cout<<"read to receive from de4"<<endl;

    while(1){
           InBurst = 1;
           nbad = 0;
           npacket_sob = 0;
           npacket_eob = 0;
           npacket_phy = 0;
           npacket_phy_tot = 0;
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

               cout << "Packet length: "<< length_received << endl;

               if ( length_received < 0 ) displayError("recvfrom(2)");

               if(length_received!=0 && length_received!=-1) numberofpackets_received+=1;



               //Na62-farm-lib-function
               na62::DataContainer container;
               char * primitive_pointer = nullptr;
               na62::l0::MEP* mep = nullptr;
               int_fast16_t mep_factor_temp;
	           uint_fast32_t fist_event_number_temp;
               //Na62-farm-lib-function
               primitive_pointer = primitive;
               container.data = primitive_pointer;
               container.length = length_received;
               container.ownerMayFreeData = true;

               mep = new na62::l0::MEP(container.data, container.length, container);
               mep_factor_temp = mep->getNumberOfFragments();
	           fist_event_number_temp = mep->getFirstEventNum();
               cout <<"n fraqments: " <<mep_factor_temp<<endl;


               //primitive_pointer = primitive;
               //container.data = primitive_pointer;
               //container.length = length_received;
               //container.ownerMayFreeData = true;

               //na62::l0::MEP* mep = new na62::l0::MEP(container.data, container.length, container);
               //mep = new na62::l0::MEP(container.data, container.length, container);
               //int_fast16_t mepfactorTEMP = mep->getNumberOfFragments();
	           //uint_fast32_t fist_event_number_temp = mep->getFirstEventNum();
               //cout <<"n fraqments: " <<mepfactorTEMP<<endl;


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
                   ++numberofpackets_sent;
               } else {
                   cout<<"packet not sent"<<endl;
               }
               //cout<<"nIP: "<<argv[nIP]<<" numberofpackets_sent: "<<numberofpackets_sent<<endl;

               /*
                * Loop on all packets
                * and set back to 1
                */
               if (address_skip == 0) {
                   if (numberofpackets_sent%address_skip == 0 || numberofpackets_sent == 1) {
                       if (nIP != argc-1) {
                           ++nIP;
                       } else {
                           nIP = 1;
                       }
                   }
               } else {
                   if (numberofpackets_sent%address_skip == 0) {
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
           } //end of while(a)

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
