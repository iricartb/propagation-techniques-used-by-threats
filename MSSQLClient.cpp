#include "MSSQLClient.h"

MSSQLClient::MSSQLClient(const char * host, unsigned int port) : SQLClient(host, port) { }

bool MSSQLClient::connect(const char * database) { 
   if (setLogin) {
      InConnectionString = (SQLCHAR *) malloc(strlen("DRIVER={SQL Server};SERVER=") + strlen(hostname) + strlen(";DATABASE=") + strlen(database) + strlen(";UID=") + strlen(username) + strlen(";PWD=") + strlen(password) + 7);
      
      if (InConnectionString != NULL) {
         sprintf((char *) InConnectionString, "DRIVER={SQL Server};SERVER=%s,%u;DATABASE=%s;UID=%s;PWD=%s", hostname, SQLPort, database, username, password);
         return SQLClient::connect(database);
      }
   }
   else return false; 
}
