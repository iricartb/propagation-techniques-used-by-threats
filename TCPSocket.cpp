#include "TCPSocket.h"
#include "FunctionsWindowsNT.h"

TCPSocket::TCPSocket(const char * host, const PORT numPort, bool authRequiered) {
   hostname = new char[strlen(host) + 1];
   strcpy(hostname, host);
   port = numPort;
   username = NULL;
   password = NULL;
   recvData = NULL;
   
   TCPSetLogin = false;
   TCPConnected = false;
   TCPAuthRequiered = authRequiered;
}

void TCPSocket::login(const char * user, const char * passwd) {
   if (TCPAuthRequiered) {
      username = new char[strlen(user) + 1];
      strcpy(username, user);
      password = new char[strlen(passwd) + 1];
      strcpy(password, passwd);
      
      TCPSetLogin = true;
   }
}

bool TCPSocket::connect() {
   WSADATA wsaData;
   struct sockaddr_in their_addr;
   char * IPAddress;
   bool result = false, err = false;
   
   if (!TCPConnected) { 
      
      if (TCPAuthRequiered) {
         if (!TCPSetLogin) err = true;      
      }
      
      if (!err) {
         if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0) {
            if ((sockData = socket(AF_INET, SOCK_STREAM, 0)) != INVALID_SOCKET) {
               IPAddress = FunctionsWindowsNT::DNShostname(hostname);
               if (IPAddress != NULL) {
                                            /* Server data struct */
                                            
                  their_addr.sin_family = AF_INET;                      // ; Family AF_INET
                  their_addr.sin_addr.s_addr = inet_addr(IPAddress);    // ; IP Address = MSN_HOST
                  their_addr.sin_port = htons(port);                    // ; Port = MSN_PORT
                  memset(&(their_addr.sin_zero), '0', 8);               // ; IP:Port = MSN_HOST:MSN_PORT  
         
                  if (TCPFunctions::TCPconnect(sockData, (struct sockaddr *) &their_addr, sizeof(struct sockaddr))) {
                     TCPConnected = true; result = true;
                  }

                  else err = true; 
                  free(IPAddress); 
               }
               else err = true;
            }
            else err = true;
            
            if (err) WSACleanup();
         }         
      }                              
   }
   return result;
}

bool TCPSocket::send(const char * message, unsigned long numBytes) {
   if (TCPConnected) {
      if (TCPFunctions::TCPsend(sockData, message, numBytes)) return true;
   }
   return false;  
}

bool TCPSocket::recv(bool blocked) {
   if (TCPConnected) {
      free(recvData);
      recvData = (char *) malloc(BUFFER_SIZE + 1);
      
      if (recvData != NULL) {
         if (TCPFunctions::TCPrecv(sockData, this, blocked)) return true;      
      }
   }
   return false;   
}

bool TCPSocket::close() { 
   if (TCPConnected) {
      if (closesocket(sockData) == 0) return true;
   }
   return false; 
}   

bool TCPSocket::serverRequieredAuth() {
   return TCPAuthRequiered;
}
                          
void TCPSocket::setHostname(const char * host) {
   delete [] hostname;
   hostname = new char[strlen(host) + 1];
   strcpy(hostname, host);
}

void TCPSocket::setUsername(const char * user) { 
   
   if (TCPAuthRequiered) {
      delete [] username;
      username = new char[strlen(user) + 1];
      strcpy(username, user);
      if (password != NULL) TCPSetLogin = true;
   }
}

void TCPSocket::setPassword(const char * passwd) {
  
   if (TCPAuthRequiered) {
      delete [] password; 
      password = new char[strlen(passwd) + 1];
      strcpy(password, passwd);
      if (username != NULL) TCPSetLogin = true;
   } 
}

void TCPSocket::setPort(const PORT numPort) { port = numPort; }

void TCPSocket::setReferenceData(char * pointer) {
   recvData = pointer;
}

char * TCPSocket::getHostname() { return hostname; }

char * TCPSocket::getUsername() { 
   if (TCPAuthRequiered) {
      if ((TCPSetLogin) || (username != NULL)) {
         return username;
      }
   }
   return NULL;
}

char * TCPSocket::getPassword() { 
   if (TCPAuthRequiered) {
      if ((TCPSetLogin) || (password != NULL)) {
         return password;
      }
   }
   return NULL;
}

PORT TCPSocket::getPort() { return port; }

char * TCPSocket::getServerData() {
   if (recvData != NULL) {
      if (strlen(recvData) > 0) {
         return recvData;
      }
   }
   return NULL;
}

bool TCPFunctions::TCPconnect(SOCKET socket, const struct sockaddr * sockAddr, int sockAddrSize) {
   if (connect(socket, sockAddr, sockAddrSize) != SOCKET_ERROR) return true;
   return false;
}

bool TCPFunctions::TCPsend(int socket, const char * message, unsigned long numBytes) {
   if (numBytes == 0) {
      if (send(socket, message, strlen(message), 0) == strlen(message)) return true;
   }
   else { 
      if (send(socket, message, numBytes, 0) == numBytes) return true;
   }
   return false;
}

/* ##################################################################################
   ##                                                                              ##
   ##  [ + ] FUNCTION: TCPrecv                                                     ##
   ##  [ + ] DESCRIPTION: block until data is avaliable, recive data from server   ##
   ##                     set non_block socket flag and get all pending data       ##
   ##  [ + ] PARAMETERS: socket -> socket desctiptor                               ##
   ##                    buffer -> message stored                                  ##
   ##  [ + ] RETURN: bool -> true: if recv data correctly                          ##
   ##                        false: if not recv data correctly                     ##
   ##                                                                              ##
   ################################################################################## */
   
bool TCPFunctions::TCPrecv(int socket, TCPSocket * object, bool blocked) {
   unsigned long total = 0, flag = 1;
   char localBuffer[BUFFER_SIZE + 1];
   bool err = false, first = true;
   char * pBuffer = (char *) malloc(BUFFER_SIZE);
   
   if (pBuffer != NULL) {
      *(pBuffer) = '\x00';
      if (object->getServerData() != NULL) { free(object->getServerData()); object->setReferenceData(NULL); }
      if (!blocked) {
         /* set socket flag -> NON_BLOCK */
         if (ioctlsocket(socket, FIONBIO, &flag) == SOCKET_ERROR) err = true;
         else first = false;      
      }
      for(long n = 0;(((n = recv(socket, localBuffer, BUFFER_SIZE, 0)) > 0) && (!err));) {
         if ((first) && (blocked)) {
            /* set socket flag -> NON_BLOCK => get all pending data (if no data avaliable - exit for) */
            if (ioctlsocket(socket, FIONBIO, &flag) == SOCKET_ERROR) err = true;
            else first = false;
         }
         if (!err) {
            total += n;
            localBuffer[n] = '\x00';
             
            pBuffer = (char *) realloc(pBuffer, total + 1);
    
            if (pBuffer != NULL) {
               object->setReferenceData(pBuffer);
               strcat(pBuffer, localBuffer);
            }
            else err = true;
         }
      }
   }
   /* set socket flag -> BLOCK => restore socket flag */
   if (!first) { flag--; ioctlsocket(socket, FIONBIO, &flag); }
   if ((!err) && (total > 0)) return true;
   return false;
}   
