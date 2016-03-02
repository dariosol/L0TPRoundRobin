#ifndef USER1_H
#define USER1_H

#include <netpacket/packet.h>
#include <linux/if_ether.h>

#include "typedefs.h"

// ----------------------------------------------------------------------------
//typedef int bool;
#define true 1
#define false 0
#define DEV_ADDR_SZ ETH_ALEN

bool   setRecvTimeOut( s32 _socket,s32 timeout_mus );
bool   recvFrame(s32 *_socket);
bool   recvFrame0();
bool   recvFrame1();
bool   recvFrame2();
bool   recvFrame3();
void   setSocketErr(char* str );

// The local network interface
char   _ifName0[4];
s32    _ifIndex0;
char   _ifName1[4];
s32    _ifIndex1;
char   _ifName2[4];
s32    _ifIndex2;
char   _ifName3[4];
s32    _ifIndex3;

// Socket stuff
s32                _socket0;
s32                _socket1;
s32                _socket2;
s32                _socket3;

struct sockaddr_ll _sAddrLl0;
struct sockaddr_ll _sAddrLl1;
struct sockaddr_ll _sAddrLl2;
struct sockaddr_ll _sAddrLl3;

// Error stuff
bool   _err, _socketErr;
char  _socketErrStr[255];

// Source and destination MAC addresses
u8     _srcMacAddr0[DEV_ADDR_SZ], _dstMacAddr0[DEV_ADDR_SZ];
u8     _srcMacAddr1[DEV_ADDR_SZ], _dstMacAddr1[DEV_ADDR_SZ];
u8     _srcMacAddr2[DEV_ADDR_SZ], _dstMacAddr2[DEV_ADDR_SZ];
u8     _srcMacAddr3[DEV_ADDR_SZ], _dstMacAddr3[DEV_ADDR_SZ];


// Storage for a received frame with data from the DEV
u8     _recvFrame0[256];
int    _recvFrameLen0;

u8     _sendFrame0[2000000];
int    _sendFrameLen0;


#define TYPE_MASK 0xf0000000
#define TDC_MASK  0x0f000000
#define CH_MASK   0x00f80000
#define L_TYPE    0x40000000
#define T_TYPE    0x50000000
#define TRIG_MASK 0x0000ff00
#define SOB_TRIG  0x00008800
#define PHY_TRIG  0x00000400
#define EOB_TRIG  0x00008c00
#define TRIG_MASK_BYTE 0xff
#define SOB_TRIG_BYTE  0x88
#define PHY_TRIG_BYTE  0x04
#define EOB_TRIG_BYTE  0x8c

// ----------------------------------------------------------------------------
#endif 
