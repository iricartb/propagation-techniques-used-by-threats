#include "HTTPClient.h"

HTTPClient::HTTPClient(const char * host, const PORT numPort, bool SSL) {
   hostname = new char[strlen(host) + 1];
   strcpy(hostname, host);
   port = numPort;
   HTTPConnected = false;
   HTTPData = NULL;
   SecureSocketLayer = SSL;
}

bool HTTPClient::connect(const char * userAgent) {
   bool result = false;
   
   if (!HTTPConnected) { 
                                   
      HTTPOpen = InternetOpenA(userAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
      if (HTTPOpen != NULL) {
         HTTPConnect = InternetConnectA(HTTPOpen, hostname, port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
         if (HTTPConnect != NULL) { result = true; HTTPConnected = true; }
         else InternetCloseHandle(HTTPOpen);
      }         
   }
   return result;
}

bool HTTPClient::get(const char * destinyFileName, const char * requestHeaders) {
   return getHTTPData(destinyFileName, NULL, ASCII, requestHeaders);
}

bool HTTPClient::getFile(const char * destinyFileName, const char * sourceFileName, unsigned int type, const char * requestHeaders) {
   return getHTTPData(destinyFileName, sourceFileName, type, requestHeaders);   
}

bool HTTPClient::getHeaders(const char * destinyFileName, const char * requestHeaders) {
   char statusCode[HTTP_STATUS_CODE_LEN + 1];
   DWORD sizeCode = sizeof(statusCode);
   DWORD sizeHeader = MAX_SIZE_HEADER;
   DWORD flags = INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_UI | INTERNET_FLAG_RELOAD;
   bool result = false;
   bool sendOK = false;
   
   if (SecureSocketLayer) flags |= INTERNET_FLAG_SECURE;
   
   if (HTTPConnected) { 
      HTTPRequest = HttpOpenRequest(HTTPConnect, "GET", destinyFileName, "HTTP/1.1", NULL, NULL, flags, 0);
      if (HTTPRequest != NULL) {
         if (requestHeaders == NULL)
            sendOK = HttpSendRequest(HTTPRequest, NULL, 0, NULL, 0);
         else
            sendOK = HttpSendRequest(HTTPRequest, requestHeaders, strlen(requestHeaders), NULL, 0);
            
         if (sendOK) {
            if (HttpQueryInfo(HTTPRequest, HTTP_QUERY_STATUS_CODE, statusCode, &sizeCode, NULL)) {
               if (strcmp(statusCode, HTTP_CODE_STATUS_OK) == 0) {
                  if (HttpQueryInfo(HTTPRequest, HTTP_QUERY_RAW_HEADERS_CRLF, HTTPResponseHeader, &sizeHeader, NULL)) result = true; 
               }
            }
         }
      }
   }
   return result;
}

bool HTTPClient::getHTTPData(const char * destinyFileName, const char * sourceFileName, unsigned int type, const char * requestHeaders) {
   bool result = false, err = false;
   DWORD numReadedBytes;
   FILE * stream;
   char data[HTTP_BUFFER_READ], statusCode[HTTP_STATUS_CODE_LEN + 1];
   bool quit, correct = false;
   DWORD sizeCode = sizeof(statusCode);
   DWORD sizeHeader = MAX_SIZE_HEADER;
   DWORD flags = INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_UI | INTERNET_FLAG_RELOAD;
   bool sendOK = false;
   
   if (SecureSocketLayer) flags |= INTERNET_FLAG_SECURE;
   
   if (HTTPConnected) { 
      HTTPRequest = HttpOpenRequest(HTTPConnect, "GET", destinyFileName, "HTTP/1.1", NULL, NULL, flags, 0);
      if (HTTPRequest != NULL) {
         if (requestHeaders == NULL)
            sendOK = HttpSendRequest(HTTPRequest, NULL, 0, NULL, 0);
         else
            sendOK = HttpSendRequest(HTTPRequest, requestHeaders, strlen(requestHeaders), NULL, 0);
            
         if (sendOK) {
            if (HttpQueryInfo(HTTPRequest, HTTP_QUERY_STATUS_CODE, statusCode, &sizeCode, NULL)) {
               if (strcmp(statusCode, HTTP_CODE_STATUS_OK) == 0) {
                  HttpQueryInfo(HTTPRequest, HTTP_QUERY_RAW_HEADERS_CRLF, HTTPResponseHeader, &sizeHeader, NULL);                                                                              
                  if (sourceFileName != NULL) {
                     if (type = ASCII) stream = fopen(sourceFileName, "w");
                     else stream = fopen(sourceFileName, "wb");
               
                     if (stream == NULL) err = true;              
                  }
                  else {
                     if (HTTPData != NULL) { free(HTTPData); HTTPData = NULL; }
                     HTTPData = (char *) malloc(sizeof(data) + 1);
                     if (HTTPData == NULL) err = true;
                     else *(HTTPData) = '\x00';
                  }
            
                  if (!err) {
                     quit = false;
                     for(int i = 1; ((InternetReadFile(HTTPRequest, data, sizeof(data) - 1, &numReadedBytes)) && (!err) && (!quit)); i++) {
                        data[numReadedBytes] = '\x00';
                        if (numReadedBytes > 0) {
                           correct = true;
                           if (sourceFileName != NULL) {
                              if (fwrite(data, sizeof(char), numReadedBytes, stream) == EOF) err = true;              
                           }
                           else {
                              if (i > 1) {
                                 HTTPData = (char *) realloc(HTTPData, (i * sizeof(data)) + 1);
                                 if (HTTPData == NULL) err = true;
                              }
                              if (!err) strcat(HTTPData, data);                    
                           }
                        }
                        else quit = true;
                     }
                     if (sourceFileName != NULL) {
                        fflush(stream);
                        fclose(stream);
                     }
                     if ((!err) && (correct)) result = true;
                  }
               }
            }
         }
      }
   }
   return result;   
}              

bool HTTPClient::close() { 
   bool result = false;
   
   if (InternetCloseHandle(HTTPConnect)) {
      if (InternetCloseHandle(HTTPOpen)) result = true;  
   }
   return result;
}

void HTTPClient::setHostname(const char * host) {
   delete [] hostname;
   hostname = new char[strlen(host) + 1];
   strcpy(hostname, host);
}

void HTTPClient::setPort(const PORT numPort) { port = numPort; }

char * HTTPClient::getHostname() { return hostname; }

char * HTTPClient::getData() {
   return HTTPData;
}

char * HTTPClient::getResponseHeaders() {
   return HTTPResponseHeader;
}

PORT HTTPClient::getPort() { return port; }
