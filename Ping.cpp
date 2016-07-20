#include "Ping.h"

bool Ping::ping(const char * hostname, unsigned int numICMPPackets) {
   WSADATA wsaData;
   WORD wVersionRequested = MAKEWORD(2, 0);
   int retWSA;
   bool ret = false;
   
   retWSA = WSAStartup(wVersionRequested, &wsaData);
   
   if (!retWSA) {
      if (wsaData.wVersion == wVersionRequested) {
         ret = sendICMPPacket(hostname, numICMPPackets);
      }
      WSACleanup();
   }
   return ret;
}

bool Ping::ping(Network * network, unsigned int numICMPPackets) {
   WSADATA wsaData;
   WORD wVersionRequested = MAKEWORD(2, 0);
   int retWSA;
   bool ret = false;
   
   retWSA = WSAStartup(wVersionRequested, &wsaData);
   
   if (!retWSA) {
      if (wsaData.wVersion == wVersionRequested) {
         ret = sendICMPPacket(network, numICMPPackets);
      }
      WSACleanup();
   }
   return ret;
}

bool Ping::sendICMPPacket(Network * network, unsigned int numICMPPackets) {
   int rawSocket;
   LPHOSTENT lpHost;
   ECHOREPLY echoReply;
   struct sockaddr_in saDest;
   struct sockaddr_in saSrc;
   bool noData = false;
   char * currentIPAddress, * IPDestinyAddr, * computerName;
   int nAddrLen = sizeof(struct sockaddr_in);
   int nRet;
   
   rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
   if (rawSocket != SOCKET_ERROR) {
      network->clearActiveComputers();
      for (unsigned long i = 0; i < network->getNumNetComputers(); i++) {
         currentIPAddress = FunctionsIP::sumIP(network->getStartIP(), i);  
         lpHost = gethostbyname(currentIPAddress);
	     if (lpHost != NULL) {
   	        saDest.sin_addr.s_addr = *((u_long FAR *) (lpHost->h_addr));
   	        saDest.sin_family = AF_INET;  
             
            for (unsigned int j = 0; j < numICMPPackets; j++) {
               sendEchoRequest(rawSocket, &saDest, (i));
            }
         }
         free(currentIPAddress);
	  }
      Sleep(TIME_TO_SLEEP);
      for(;(!noData);) {
         if (waitForEchoReply(rawSocket, 1, (network->getNumNetComputers() - 1))) { 
            nRet = recvfrom(rawSocket, (LPSTR) &echoReply, sizeof(ECHOREPLY), 0, (LPSOCKADDR) &saSrc, &nAddrLen);
            if (nRet > 0) {
               
               IPDestinyAddr = FunctionsIP::castingNumberToStringIP(echoReply.ipHdr.iaSrc.s_addr, true);

               if ((strcmp(IPDestinyAddr, network->getGateway()) != 0) && (strcmp(IPDestinyAddr, network->getIPmachine()) != 0) && 
                  (FunctionsIP::castingStringIPToNumber(IPDestinyAddr) >= FunctionsIP::castingStringIPToNumber(network->getStartIP())) &&
                  (FunctionsIP::castingStringIPToNumber(IPDestinyAddr) <= FunctionsIP::castingStringIPToNumber(network->getEndIP()))) {
                  
                  if (network->getNumNetActiveComputers() > 0) 
                     network->setComputerList((Computer **) realloc(network->getComputerList(), sizeof(Computer *) * (network->getNumNetActiveComputers() + 1)));
               
                  *(network->getComputerList() + network->getNumNetActiveComputers()) = (Computer *) malloc(sizeof(Computer)); 
               
                  *(network->getComputerList() + network->getNumNetActiveComputers()) = new Computer(IPDestinyAddr, false);
                   
                  network->setNumNetActiveComputers(network->getNumNetActiveComputers() + 1);             
               }  
               free(IPDestinyAddr);
            }
         }
         else noData = true;
      }
      closesocket(rawSocket);
      
      /* Set ComputerName && ResourceShares -> ActiveComputers in Network */
      for (unsigned long i = 0; i < network->getNumNetActiveComputers(); i++) {
         computerName = FunctionsWindowsNT::getComputerName((*(network->getComputerList() + i))->getIPAddress());
         if (computerName != NULL) (*(network->getComputerList() + i))->setComputerName(computerName);
         
         (*(network->getComputerList() + i))->scanResourceShares();
         free(computerName);
      }
   }
   return (network->getNumNetActiveComputers() > 0);
}
   
bool Ping::sendICMPPacket(const char * hostname, unsigned int numICMPPackets) {
   int rawSocket;
   LPHOSTENT lpHost;
   DWORD dwTimeSent, dwElapsed;
   u_char cTTL;
   struct sockaddr_in saDest;
   struct sockaddr_in saSrc;
   bool retPing = false;
   
   rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
   if (rawSocket != SOCKET_ERROR) {
      lpHost = gethostbyname(hostname);
	  if (lpHost != NULL) {
   	     saDest.sin_addr.s_addr = *((u_long FAR *) (lpHost->h_addr));
   	     saDest.sin_family = AF_INET;  
           
         saSrc.sin_addr.s_addr = htonl(INADDR_ANY);
         saSrc.sin_family = AF_INET;
             
         for (unsigned int i = 0; i < numICMPPackets; i++) {
            if (sendEchoRequest(rawSocket, &saDest, (i + 1))) {
               if (waitForEchoReply(rawSocket, 5, 1)) {                     
                  dwTimeSent = recvEchoReply(rawSocket, &saSrc, &cTTL);         // Receive reply
         	      if (dwTimeSent != 0) {
         		     dwElapsed = GetTickCount() - dwTimeSent;                   // Calculate elapsed time
         		     retPing = true;
                  } 
               }	
            }  
         }  
	  }
      closesocket(rawSocket);   
   }
   return retPing;
}

bool Ping::sendEchoRequest(int fdSocket, LPSOCKADDR_IN lpstToAddr, unsigned int IDPacket) {
    ECHOREQUEST echoReq;
    int nRet;
    
	// Fill in echo request
    echoReq.icmpHdr.Type	   = ICMP_ECHOREQ;
	echoReq.icmpHdr.Code	   = 0;
	echoReq.icmpHdr.Checksum   = 0;
	echoReq.icmpHdr.ID	   	   = IDPacket;
	echoReq.icmpHdr.Seq		   = IDPacket;

	// Fill in some data to send
	for (nRet = 0; nRet < REQ_DATASIZE; nRet++)
		echoReq.cData[nRet] = ' ' + nRet;

	// Save tick count when sent
	echoReq.dwTime = GetTickCount();

	// Put data in packet and compute checksum
	echoReq.icmpHdr.Checksum = in_cksum((u_short *) &echoReq, sizeof(ECHOREQUEST));

	// Send the echo request  								  
	nRet = sendto(fdSocket, (LPSTR)&echoReq, sizeof(ECHOREQUEST), 0, (LPSOCKADDR) lpstToAddr, sizeof(SOCKADDR_IN));

	if (nRet != SOCKET_ERROR) return true;
	return false;
}

DWORD Ping::recvEchoReply(int fdSocket, LPSOCKADDR_IN lpsaFrom, u_char * pTTL) {
   ECHOREPLY echoReply;
   int nRet;
   int nAddrLen = sizeof(struct sockaddr_in);

	// Receive the echo reply	
   nRet = recvfrom(fdSocket, (LPSTR) &echoReply, sizeof(ECHOREPLY), 0, (LPSOCKADDR)lpsaFrom, &nAddrLen);
   
    // Check return value
   if (nRet > 0) {
      // return time sent and IP TTL
      *pTTL = echoReply.ipHdr.TTL;
      return(echoReply.echoRequest.dwTime);
   }  
   return 0; 		
}

u_short Ping::in_cksum(u_short *addr, int len) {
   register int nleft = len;
   register u_short *w = addr;
   register u_short answer;
   register int sum = 0;
   
	
  /*  Our algorithm is simple, using a 32 bit accumulator (sum),
   *  we add sequential 16 bit words to it, and at the end, fold
   *  back all the carry bits from the top 16 bits into the lower
   *  16 bits. */
   
   while( nleft > 1 )  {
		sum += *w++;
		nleft -= 2;
	}

   /* mop up an odd byte, if necessary */
   if (nleft == 1 ) {
      u_short u = 0;

	  *(u_char *)(&u) = *(u_char *)w ;
      sum += u;
   }

   sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
   sum += (sum >> 16);			        /* add carry */
   answer = ~sum;				        /* truncate to 16 bits */
   return (answer);
}

bool Ping::waitForEchoReply(int fdSocket, unsigned int seconds, unsigned int maxIDPacket) {
   struct timeval Timeout;
   fd_set readfds;
   int val = 0;
     
   FD_ZERO(&readfds);
   FD_SET(fdSocket, &readfds);
   Timeout.tv_sec = seconds;
   Timeout.tv_usec = 0;

   val = select(maxIDPacket, &readfds, NULL, NULL, &Timeout);  

   if ((val > 0) && (FD_ISSET(fdSocket, &readfds))) return true;
   return false;
}
