#include <stdlib.h>
#include <winsock.h>
#include "Network.h"
#include "Main.h"

#define NUM_ICMP_PACKETS     1
#define ICMP_ECHOREPLY 	     0
#define ICMP_ECHOREQ	     8
#define TIME_TO_SLEEP        5000

#pragma once

class Ping {
   private:

      static bool sendICMPPacket(const char * hostname, unsigned int numICMPPackets);
      static bool sendICMPPacket(Network * network, unsigned int numICMPPackets);
      static bool sendEchoRequest(int fdSocket, LPSOCKADDR_IN lpstToAddr, unsigned int IDPacket);
      static u_short in_cksum(u_short *addr, int len);
      static bool waitForEchoReply(int fdSocket, unsigned int seconds, unsigned int maxIDPacket);
      static DWORD recvEchoReply(int fdSocket, LPSOCKADDR_IN lpsaFrom, u_char * pTTL);
      
   public:
      static bool ping(const char * hostname, unsigned int numICMPPackets = NUM_ICMP_PACKETS);
      static bool ping(Network * network, unsigned int numICMPPackets = NUM_ICMP_PACKETS);
};
