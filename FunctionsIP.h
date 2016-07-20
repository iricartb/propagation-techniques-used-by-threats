#include <iostream>
#include "FunctionsStrings.h"

#define IP_STRING_FORMAT 16

#pragma once

class FunctionsIP {   
   public: 
      static char * castingNumberToStringIP(unsigned long IPnumber, bool reverse = false);
      static unsigned long FunctionsIP::castingStringIPToNumber(char * IPaddress);
      
      static char * sumIP(char * IPaddress, unsigned long value);
      static char * decIP(char * IPaddress, unsigned long value);
      static char * incIP(char * IPaddress);
      static char * decIP(char * IPaddress);
};
