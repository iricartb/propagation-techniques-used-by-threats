#include <wininet.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#define ASCII                       0 
#define BINARY                      1
#define HTTP_STATUS_CODE_LEN        3
#define HTTP_BUFFER_READ            50
#define HTTP_CODE_FILE_NOT_FOUND    "404"
#define HTTP_CODE_STATUS_OK         "200"
#define MAX_SIZE_HEADER             4096

#pragma once

typedef unsigned int PORT;

class HTTPClient {
   private:
      HINTERNET HTTPOpen, HTTPConnect, HTTPRequest;
      PORT port;
      char * hostname, * HTTPData;
      char HTTPResponseHeader[MAX_SIZE_HEADER];
      bool HTTPConnected;
      bool SecureSocketLayer;
      bool getHTTPData(const char * destinyFileName, const char * sourceFileName, unsigned int type = ASCII, const char * requestHeaders = NULL);
      
   public:
      HTTPClient(const char * host, const PORT numPort = 80, bool SSL = false);
      ~HTTPClient() { delete [] hostname; delete [] HTTPData; }
      
      bool connect(const char * userAgent = "HTTPClient");
      bool get(const char * destinyFileName, const char * requestHeaders = NULL);
      bool getHeaders(const char * destinyFileName, const char * requestHeaders = NULL);
      bool getFile(const char * destinyFileName, const char * sourceFileName, unsigned int type = ASCII, const char * requestHeaders = NULL);
      bool close();
      void setHostname(const char * host);
      void setPort(const PORT numPort);
      char * getHostname();
      char * getData();
      char * getResponseHeaders();
      PORT getPort();
};
