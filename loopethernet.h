#ifndef LOOPETHERNET_H
#define LOOPETHERNET_H
int z;
char *srvr_addr = NULL;
struct sockaddr_in adr_inet; // AF_INET
struct sockaddr_in adr_clnt; // AF_INET
int len_inet;                // length
int fromDE4;                       // Socket
char primitive[2048];
//char * primitive;

int status125_port;
int *status125_result;

int z1;
char *srvr_addr1 = NULL;
struct sockaddr_in adr_inet1; // AF_INET
struct sockaddr_in adr_clnt1; // AF_INET
int len_inet1;                // length
int toFARM;
int numberofpackets=0;

int set_port();
void hexdump(const void * buf, size_t size);

#endif
