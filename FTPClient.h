#include <wininet.h>

#define ASCII               0 
#define BINARY              1

#define CHMOD_READONLY      1
#define CHMOD_HIDDEN        2
#define CHMOD_SYSTEM        4
#define CHMOD_ARCHIVE       32
#define CHMOD_TEMPORARY     256
#define CHMOD_NORMAL        128
#define CHMOD_OFFLINE       4096
#define CHMOD_ENCRYPTED     16384

#pragma once

typedef unsigned int PORT;

class FTPClient {
   private:
      HINTERNET FTPOpen, FTPConnect;
      PORT port;
      char * hostname, * username, * password;
      bool setLogin, FTPLogged;
      
   public:
      FTPClient(const char * host, const PORT numPort = 21);
      ~FTPClient() { delete [] hostname; delete [] username; delete [] password; }
      
      void login(const char * user, const char * passwd);
      bool connect(const char * userAgent = "FTPClient");
      bool get(const char * destinyFileName, const char * sourceFileName, int TYPE = ASCII, long ATTR = CHMOD_NORMAL);
      bool close();
      void setHostname(const char * host);
      void setUsername(const char * user);
      void setPassword(const char * passwd);
      void setPort(const PORT numPort);
      char * getHostname();
      char * getUsername();
      char * getPassword();
      PORT getPort();
};
