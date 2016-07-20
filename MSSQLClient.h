#include "SQLClient.h"

#define MSSQL_DEFAULT_PORT      1433

#pragma once

class MSSQLClient : public SQLClient {

   public:
      MSSQLClient(const char * host, unsigned int port = MSSQL_DEFAULT_PORT);
      ~MSSQLClient() { delete [] hostname; delete [] username; delete [] password; delete [] database; }
      
      bool connect(const char * database);
};
