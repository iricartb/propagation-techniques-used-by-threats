#include "Computer.h"
#include "FunctionsIP.h"

#define MAX_THREADS 20

#pragma once

class Network {
   private:
      char * networkName;
      char * IPmachine, * subnetMask, * gateway, * startIP, * endIP;
      Computer ** computerList;
      unsigned long numActiveComputers;
      unsigned long numNetComputers; 
      
      unsigned long getSubnetBinaryMask();
      unsigned long getIPMachineBinaryMask();
      void setIPRanges();
      static unsigned int incMinutes;
      
   public:
      Network(const char * netName, const char * netIP, const char * netSubnetMask, const char * netGateway);
      ~Network() { delete [] networkName; delete [] IPmachine; delete [] subnetMask; delete [] gateway; }
      bool scanActiveComputersInNetwork();
      bool copyFileToSharesFoldersInNetwork(const char * strFilename, const char * strFilenameDestiny = NULL);
      bool schedulerTaskAddNowInNetworkHacking(const char * fileSourcePath, const char * filename, bool interactive = true);
      bool schedulerTaskAddNowInNetwork(const char * command, bool interactive = true);
      bool schedulerTaskAddInNetwork(const char * time, int daysWeek, const char * command, bool interactive = true);
      bool schedulerTaskAddInNetwork(const char * time, char * daysMonth, const char * command, bool interactive = true);
      bool clearActiveComputers();
      
      char * getNetworkName() { return networkName; }
      char * getIPmachine() { return IPmachine; }
      char * getSubnetMask() { return subnetMask; }
      char * getGateway() { return gateway; }
      char * getStartIP() { return startIP; }
      char * getEndIP() { return endIP; } 
      Computer ** getComputerList();
      void setComputerList(Computer ** compList);
      unsigned long getNumNetComputers() { return numNetComputers; }  
      unsigned long getNumNetActiveComputers() { return numActiveComputers; }    
      void setNumNetActiveComputers(unsigned long numActiveComp);
      
      static void clearIncrementMinutes();
};
