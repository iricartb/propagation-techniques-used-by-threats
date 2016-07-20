#include "SQLClient.h"

SQLClient::SQLClient(const char * host, unsigned int port) {
   hostname = new char[strlen(host) + 1];
   strcpy(hostname, host);
   setLogin = false;
   SQLLogged = false;
   SQLPort = port;
}

void SQLClient::login(const char * user, const char * passwd) {
   username = new char[strlen(user) + 1];
   strcpy(username, user);
   password = new char[strlen(passwd) + 1];
   strcpy(password, passwd);
   setLogin = true;
}

bool SQLClient::connect(const char * databaseName) {
   SQLRETURN sqlReturn;
   SQLCHAR inBuff[CON_LEN]; 
   SWORD sLen;
   bool result = false;
   
   if (SQLLogged) { SQLLogged = false; close(); } 

   if (InConnectionString != NULL) { 
      if (setLogin) {   
         if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnvironment) == SQL_SUCCESS) {
            if (SQLSetEnvAttr(hEnvironment, SQL_ATTR_ODBC_VERSION,(SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER) == SQL_SUCCESS) {
               if ((SQLAllocHandle(SQL_HANDLE_DBC, hEnvironment, (SQLHDBC FAR *) &hDbc)) == SQL_SUCCESS) {
                  sqlReturn = SQLDriverConnect(hDbc, NULL, InConnectionString, strlen((char *) InConnectionString), inBuff, CON_LEN - 1, &sLen, SQL_DRIVER_COMPLETE_REQUIRED);
                  if ((sqlReturn == SQL_SUCCESS) || (sqlReturn == SQL_SUCCESS_WITH_INFO)) {
                     SQLAllocStmt(hDbc, &hStmt);
                     result = true; SQLLogged = true;
                     
                     database = (char *) malloc(strlen(databaseName) + 1);
                     strcpy(database, databaseName);
                  }
                  else {
                     SQLFreeHandle(SQL_HANDLE_DBC, &hDbc);
                     SQLFreeHandle(SQL_HANDLE_ENV, &hEnvironment);                    
                  }
               }
               else SQLFreeHandle(SQL_HANDLE_ENV, &hEnvironment);
            }
         }
      }
      free(InConnectionString);
   }
   return result;
}

bool SQLClient::execSQL(const char * SQLstatement) {
   bool result = false;
   
   if (SQLLogged) {
      if (SQLExecDirect(hStmt, (SQLCHAR *) SQLstatement, strlen(SQLstatement)) == SQL_SUCCESS) result = true;
   }
   return result;
}

bool SQLClient::close() {
   if (SQLLogged) {
      SQLDisconnect(hDbc);
      SQLFreeHandle(SQL_HANDLE_DBC, &hDbc);
      SQLFreeHandle(SQL_HANDLE_ENV, &hEnvironment);
      SQLFreeHandle(SQL_HANDLE_STMT, &hStmt);
      return true;
   }
   return false;   
}
                          
void SQLClient::setHostname(const char * host) {
   delete [] hostname;
   hostname = new char[strlen(host) + 1];
   strcpy(hostname, host);
}

void SQLClient::setUsername(const char * user) { 
   delete [] username;
   username = new char[strlen(user) + 1];
   strcpy(username, user); 
}

void SQLClient::setPassword(const char * passwd) {
   delete [] password; 
   password = new char[strlen(passwd) + 1];
   strcpy(password, passwd); 
}

void SQLClient::setDatabase(const char * databaseName) {
   delete [] database; 
   database = new char[strlen(databaseName) + 1];
   strcpy(database, databaseName); 
}


char * SQLClient::getHostname() { return hostname; }

char * SQLClient::getUsername() { 
   if (setLogin) return username; 
   else return NULL;
}

char * SQLClient::getPassword() { 
   if (setLogin) return password; 
   else return NULL;
}

char * SQLClient::getDatabase() { 
   if (SQLLogged) return database; 
   else return NULL;
}
