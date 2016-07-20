#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>

#define CON_LEN       1025

#pragma once

class SQLClient {
   protected:
      SQLHDBC hDbc;
      HSTMT hStmt;
      SQLHANDLE hEnvironment;
      SQLCHAR * InConnectionString;
      char * hostname, * username, * password, * database;
      bool setLogin, SQLLogged;
      unsigned int SQLPort;
      
   public:
      SQLClient(const char * host, unsigned int port);
      ~SQLClient() { delete [] hostname; delete [] username; delete [] password; delete [] database; }
      
      void login(const char * user, const char * passwd);
      virtual bool connect(const char * database) = 0;
      bool execSQL(const char * SQLstatement);
      bool close();
      void setHostname(const char * host);
      void setUsername(const char * user);
      void setPassword(const char * passwd);
      void setDatabase(const char * databaseName);
      char * getHostname();
      char * getUsername();
      char * getPassword();
      char * getDatabase();
};
