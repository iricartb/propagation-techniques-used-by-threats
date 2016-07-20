#include "FunctionsIP.h"

char * FunctionsIP::castingNumberToStringIP(unsigned long IPnumber, bool reverse) {
   char * result = (char *) malloc(IP_STRING_FORMAT);
   
   if (!reverse) sprintf(result, "%d.%d.%d.%d", IPnumber >> 24, ((IPnumber << 8) >> 24), ((IPnumber << 16) >> 24), ((IPnumber << 24) >> 24));
   else sprintf(result, "%d.%d.%d.%d", ((IPnumber << 24) >> 24), ((IPnumber << 16) >> 24), ((IPnumber << 8) >> 24), IPnumber >> 24);
   
   return result;
}

unsigned long FunctionsIP::castingStringIPToNumber(char * IPaddress) {
   unsigned long result = 0;
   char * segmentIP1 = FunctionsStrings::getToken(IPaddress, '.', 1);
   char * segmentIP2 = FunctionsStrings::getToken(IPaddress, '.', 2);
   char * segmentIP3 = FunctionsStrings::getToken(IPaddress, '.', 3);
   char * segmentIP4 = FunctionsStrings::getToken(IPaddress, '.', 4);
   
   result += atoi(segmentIP1) << 24;
   result += atoi(segmentIP2) << 16;
   result += atoi(segmentIP3) << 8;
   result += atoi(segmentIP4);
        
   free(segmentIP1);
   free(segmentIP2);
   free(segmentIP3);
   free(segmentIP4);
   
   return result;
}

char * FunctionsIP::sumIP(char * IPaddress, unsigned long value) {
   unsigned long result;
   
   result = castingStringIPToNumber(IPaddress);
   result += value;
   return castingNumberToStringIP(result);  
}

char * FunctionsIP::decIP(char * IPaddress, unsigned long value) {
   unsigned long result;
   
   result = castingStringIPToNumber(IPaddress);
   result -= value;
   return castingNumberToStringIP(result);  
}

char * FunctionsIP::incIP(char * IPaddress) {
   return sumIP(IPaddress, 1);
}

char * FunctionsIP::decIP(char * IPaddress) {
   return decIP(IPaddress, 1);
}
