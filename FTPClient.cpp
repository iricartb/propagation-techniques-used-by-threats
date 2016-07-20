#include "FTPClient.h"

FTPClient::FTPClient(const char * host, const PORT numPort) {
   hostname = new char[strlen(host) + 1];
   strcpy(hostname, host);
   port = numPort;
   setLogin = false;
   FTPLogged = false;
}

void FTPClient::login(const char * user, const char * passwd) {
   username = new char[strlen(user) + 1];
   strcpy(username, user);
   password = new char[strlen(passwd) + 1];
   strcpy(password, passwd);
   setLogin = true;
}

bool FTPClient::connect(const char * userAgent) {
   bool result = false;
   
   if (!FTPLogged) { 
                   
      if (setLogin) {                   
         FTPOpen = InternetOpenA(userAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
         if (FTPOpen != NULL) {
            FTPConnect = InternetConnectA(FTPOpen, hostname, port, username, password, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
            if (FTPConnect != NULL) { result = true; FTPLogged = true; }
            else InternetCloseHandle(FTPOpen);
         }
      }   
   }
   return result;
}

bool FTPClient::get(const char * destinyFileName, const char * sourceFileName, int TYPE, long ATTR) {
   bool result = false;
   
   if (FTPLogged) {
      if (TYPE == ASCII) {
         if (FtpGetFile(FTPConnect, destinyFileName, sourceFileName, false, ATTR, FTP_TRANSFER_TYPE_ASCII, INTERNET_FLAG_RELOAD)) result = true;
      }
      else if (TYPE == BINARY) { 
         if (FtpGetFile(FTPConnect, destinyFileName, sourceFileName, false, ATTR, FTP_TRANSFER_TYPE_BINARY, INTERNET_FLAG_RELOAD)) result = true;
      }
      else { 
         if (FtpGetFile(FTPConnect, destinyFileName, sourceFileName, false, ATTR, FTP_TRANSFER_TYPE_UNKNOWN, INTERNET_FLAG_RELOAD)) result = true;
      }
   }
   return result;
}

bool FTPClient::close() { 
   bool result = false;
   
   if (InternetCloseHandle(FTPConnect)) {
      if (InternetCloseHandle(FTPOpen)) result = true;  
   }
   return result;
}   
                          
void FTPClient::setHostname(const char * host) {
   delete [] hostname;
   hostname = new char[strlen(host) + 1];
   strcpy(hostname, host);
}

void FTPClient::setUsername(const char * user) { 
   delete [] username;
   username = new char[strlen(user) + 1];
   strcpy(username, user); 
}

void FTPClient::setPassword(const char * passwd) {
   delete [] password; 
   password = new char[strlen(passwd) + 1];
   strcpy(password, passwd); 
}

void FTPClient::setPort(const PORT numPort) { port = numPort; }

char * FTPClient::getHostname() { return hostname; }

char * FTPClient::getUsername() { 
   if (setLogin) return username; 
   else return NULL;
}

char * FTPClient::getPassword() { 
   if (setLogin) return password; 
   else return NULL;
}

PORT FTPClient::getPort() { return port; }
