#include <windows.h>
#include <winsock.h>

#define BUFFER_SIZE 256

#pragma once

typedef unsigned int PORT;


class TCPSocket {
   protected:    
      PORT port;
      char * hostname, * username, * password, * recvData;
      int sockData;
      bool TCPConnected, TCPAuthRequiered, TCPSetLogin;
      
   public:
      TCPSocket(const char * host, const PORT numPort, bool authRequiered = false);
      ~TCPSocket() { delete [] recvData; delete [] hostname; delete [] username; delete [] password; }
     void destroy() { this->~TCPSocket(); delete this; }
     
     void login(const char * user, const char * passwd);
     bool connect();        
     bool send(const char * message, unsigned long numBytes = 0);
     bool recv(bool blocked = true);
     bool close();
     bool serverRequieredAuth();
     void setUsername(const char * user);
     void setPassword(const char * passwd);
     void setHostname(const char * host);
     void setPort(const PORT numPort);
     void setReferenceData(char * pointer);
     char * getUsername();
     char * getPassword();
     char * getHostname();
     char * getServerData();
     PORT getPort();    
};

class TCPFunctions {
   public:
      static bool TCPconnect(SOCKET socket, const struct sockaddr * sockAddr, int sockAddrSize);
      static bool TCPsend(int socket, const char * message, unsigned long numBytes);
      static bool TCPrecv(int socket, TCPSocket * object, bool blocked);
};

