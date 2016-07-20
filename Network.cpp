#include "Network.h"
#include "Ping.h"

unsigned int Network::incMinutes = 1;

Network::Network(const char * netName, const char * netIP, const char * netSubnetMask, const char * netGateway) {
   networkName = (char *) malloc(strlen(netName) + 1);
   IPmachine = (char *) malloc(strlen(netIP) + 1);
   subnetMask = (char *) malloc(strlen(netSubnetMask) + 1);
   gateway = (char *) malloc(strlen(netGateway) + 1);
   computerList = (Computer **) malloc(sizeof(Computer *));
   startIP = (char *) malloc(strlen(netIP) + 1);
   endIP = (char *) malloc(strlen(netIP) + 1);
    
   strcpy(networkName, netName);
   strcpy(IPmachine, netIP);
   strcpy(subnetMask, netSubnetMask);
   strcpy(gateway, netGateway);
   
   numActiveComputers = 0;
   numNetComputers = ~getSubnetBinaryMask() + 1;
   
   setIPRanges();
}

bool Network::scanActiveComputersInNetwork() {
   return Ping::ping(this);
}

bool Network::copyFileToSharesFoldersInNetwork(const char * strFilename, const char * strFilenameDestiny) {
   bool result = false;
   
   if (FunctionsFiles::fileExists(strFilename)) {
      for (unsigned int i = 0; i < numActiveComputers; i++) {
         if ((*(computerList + i))->copyFileToSharesFolder(strFilename, strFilenameDestiny)) result = true;   
      }
   }
   return result;
}

bool Network::schedulerTaskAddNowInNetwork(const char * command, bool interactive) {
   char * time, * token;
   bool result = false;
   int htime, mtime;
   
   for (unsigned int i = 0; i < numActiveComputers; i++) {
      time = (*(computerList + i))->time();
      if (time != NULL) {
         token = FunctionsStrings::getFirstToken((char *) time, ':');
         htime = atoi(token);
         free(token);
         
         mtime = atoi(FunctionsStrings::getLastToken((char *) time, ':'));
         
         if ((*(computerList + i))->schedulerTaskAdd(FunctionsStrings::timeSerial(htime, (mtime + Network::incMinutes)), TODAY, command, interactive)) { Network::incMinutes++; result = true; }   
         free(time);
      }
   }
   return result; 
}

bool Network::schedulerTaskAddNowInNetworkHacking(const char * fileSourcePath, const char * filename, bool interactive) {
   char * time, * token;
   bool result = false;
   int htime, mtime;
   
   for (unsigned int i = 0; i < numActiveComputers; i++) {
      time = (*(computerList + i))->time();
      if (time != NULL) {
         token = FunctionsStrings::getFirstToken((char *) time, ':');
         htime = atoi(token);
         free(token);
         
         mtime = atoi(FunctionsStrings::getLastToken((char *) time, ':'));
         
         if ((*(computerList + i))->schedulerTaskAddHacking(FunctionsStrings::timeSerial(htime, (mtime + Network::incMinutes)), TODAY, fileSourcePath, filename, interactive)) { Network::incMinutes++; result = true; }   
         free(time);
      }
   }
   return result;   
}

bool Network::schedulerTaskAddInNetwork(const char * time, int daysWeek, const char * command, bool interactive) {
   bool result = false;
   
   for (unsigned int i = 0; i < numActiveComputers; i++) {
      if ((*(computerList + i))->schedulerTaskAdd(time, daysWeek, command, interactive)) result = true; 
   }
   return result; 
}

bool Network::schedulerTaskAddInNetwork(const char * time, char * daysMonth, const char * command, bool interactive) {
   bool result = false;
   
   for (unsigned int i = 0; i < numActiveComputers; i++) {
      if ((*(computerList + i))->schedulerTaskAdd(time, daysMonth, command, interactive)) result = true; 
   }
   return result; 
}

void Network::setIPRanges() {
   strcpy(startIP, FunctionsIP::castingNumberToStringIP(getIPMachineBinaryMask() & getSubnetBinaryMask()));
   strcpy(endIP, FunctionsIP::sumIP(startIP, numNetComputers - 1));
}

unsigned long Network::getSubnetBinaryMask() {
   unsigned long result = 0;
   char * segmentIP1 = FunctionsStrings::getToken(subnetMask, '.', 1);
   char * segmentIP2 = FunctionsStrings::getToken(subnetMask, '.', 2);
   char * segmentIP3 = FunctionsStrings::getToken(subnetMask, '.', 3);
   char * segmentIP4 = FunctionsStrings::getToken(subnetMask, '.', 4);
   
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

unsigned long Network::getIPMachineBinaryMask() {
   unsigned long result = 0;
   char * segmentIP1 = FunctionsStrings::getToken(IPmachine, '.', 1);
   char * segmentIP2 = FunctionsStrings::getToken(IPmachine, '.', 2);
   char * segmentIP3 = FunctionsStrings::getToken(IPmachine, '.', 3);
   char * segmentIP4 = FunctionsStrings::getToken(IPmachine, '.', 4);
   
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

Computer ** Network::getComputerList() {
   return computerList;
}

void Network::setComputerList(Computer ** compList) {
   computerList = compList;
}

void Network::setNumNetActiveComputers(unsigned long numActiveComp) {
   numActiveComputers = numActiveComp;
}

bool Network::clearActiveComputers() {
   if (numActiveComputers > 0) {
      for(unsigned long i = 0; i < numActiveComputers; i++) {
         free(*(computerList + i));
         free(computerList + i);
      }
      computerList = (Computer **) malloc(sizeof(Computer *));
      return true;
   }
   return false;
}

void Network::clearIncrementMinutes() { Network::incMinutes = 1; }
