#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <vector>


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

#define store_size 4026530000

#define PORT_DE4 58913
#define PORT_FARM 58913 

/*
 */
static void displayError(const char *on_what) {
    fputs(strerror(errno),stderr);
    fputs(": ",stderr);
    fputs(on_what,stderr);
    fputc('\n',stderr);
    exit(1);
}

/*
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
    signal(SIGINT, sighandler);


    vector<char*> farm_ip; 

    /*
     * Print all ip address
     * set from l0server (Run Control)
     */
    printf("argv size: %d\n", argc);
    for (int i=1; i < argc; i++) {
        printf("argv: %s\n", argv[i]);
        farm_ip.push_back(argv[i]);
        printf("argv: %s\n", farm_ip.back());
    }
    int n_farm = argc;

    /*
     * Create a UDP socket to use:
     * Receive from FPGA
     */
    fromDE4 = socket(AF_INET,SOCK_DGRAM, 0);
    if (fromDE4 == -1) {
        displayError("Error opening socket to fpga");
    }
  
    /*******************RECEIVING FROM DE4***********************/
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

    //To check the stream order
    bool is_event_id_aligned = false;
    bool is_stream_broken = false;
    int flow_break_count = 0;

    char * primitive_pointer = nullptr;
    na62::l0::CustomMEP* mep = new na62::l0::CustomMEP(20);

    int_fast16_t mep_factor_temp;
    uint_fast32_t first_event_number_temp;
    uint_fast32_t expected_first_event_number = 0;

    int mep_jump;
    int n_ip_to_skip;
    bool is_skip_int;
    nIP = 0;

    while(1){
        length_received = recvfrom(fromDE4,
                primitive,
                2048, // Max recv buf size   */
                0,
                (struct sockaddr *) &adr_clnt,
                (socklen_t*) &adr_clnt
        );

        if ( length_received < 0 ) {
            cout<<"Error Receiving packet"<<endl;
            continue; 
        }   
        ++packets_received;
        ++packets_per_burst;
        n_ip_to_skip = 0;

        //I can remove this
        primitive_pointer = primitive;

        //mep = new na62::l0::CustomMEP(primitive_pointer, length_received);
        mep->initializeMEPFragments(primitive_pointer, length_received);

        first_event_number_temp = mep->getFirstEventNum();
	if ( ! mep->isLastBurstPacket() ) {
            mep_factor_temp = mep->getNumberOfFragments();
        }

        //understand if a new burst is started
        if (first_event_number_temp < expected_first_event_number) {
            is_event_id_aligned = false;
            cout<<" => New burst is started "<<endl;
            cout<<"Packets received previous burst: "<<packets_per_burst<<endl
                <<"Broken flow packets previous burst: "<< flow_break_count<<endl
                <<"Total packets received: "<<packets_received<<endl
                <<endl;

            packets_per_burst = 0;
            flow_break_count = 0;
            nIP = 0;
            nIP = first_event_number_temp % farm_ip.size();

        }

        //Check flow consistency
        if(is_event_id_aligned){
            expected_first_event_number = expected_first_event_number + mep_factor_temp;
        } else {
            expected_first_event_number = first_event_number_temp;
            is_event_id_aligned = true;
        }

        if (first_event_number_temp > expected_first_event_number) {
            mep_jump = first_event_number_temp - expected_first_event_number;
            n_ip_to_skip = (first_event_number_temp - expected_first_event_number)/mep_factor_temp;
            if ((first_event_number_temp - expected_first_event_number) % mep_factor_temp == 0 ) {
                is_skip_int = true;
            } else {
                is_skip_int = false;
            }
            cout << ">!! Mep Jump: "<< mep_jump<<" at packet "<<packets_per_burst<<" of this burst"<<endl
                 << "Packets count: "<< packets_received<<endl
                 << "Ip to Skip: "<< n_ip_to_skip<<endl
                 << "Is skip int?: "<< is_skip_int<<endl
                 <<endl;
            is_event_id_aligned = false;
            is_stream_broken = true;
            ++flow_break_count;
        }

        /*
         * Increase packets number and set back to 1
         */
        ++nIP;
        nIP = nIP + n_ip_to_skip;
        while (nIP > argc-1) {
            nIP = nIP - (argc -1);
        }

        /******************SENDING TO FARM****************************/
        //adr_inet1.sin_addr.s_addr =  inet_addr(argv[nIP]);
        //cout<<"Event number: "<<first_event_number_temp<<" Directed to: "<<farm_ip[(first_event_number_temp/mep_factor_temp) % farm_ip.size()]<<endl;
        
        adr_inet1.sin_addr.s_addr =  inet_addr(farm_ip[(first_event_number_temp/mep_factor_temp) % farm_ip.size()]);

        length_sent = sendto(toFARM,
                primitive,
                length_received,
                0,
                (struct sockaddr *) &adr_inet1,
                sizeof(adr_inet1)
        );

        if ( length_sent < 0 ) {
            cout<<"Error sending packet"<<endl;
            continue; 
        }   
        ++packets_sent;
    } 
    delete mep;
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
