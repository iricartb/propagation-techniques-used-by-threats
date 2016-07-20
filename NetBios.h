#include <windows.h>
#include <lm.h>
#include <math.h>
#include "FunctionsFiles.h"
#include "FunctionsStrings.h"
#include "FunctionsWindowsNT.h"

#define INIT_SIZE_BUFFER           512
#define GROUP_ADMINISTRATORS_SID   544

#define TODAY                      0
#define MONDAY                     1
#define TUESDAY                    2
#define WEDNESDAY                  4
#define THURSDAY                   8
#define FRIDAY                     16
#define SATURDAY                   32
#define SUNDAY                     64
#define BEST_EFFORT                "?"

#pragma once

using namespace std;

class NetBios {
   private:                
      PSHARE_INFO_2 shareLocalInfoList;
      PSHARE_INFO_2 * shareLocalInfoPrintersList;
      PSHARE_INFO_2 * shareLocalInfoFoldersList;
      PSHARE_INFO_2 * shareLocalInfoSpecialFoldersList;
      
      PSHARE_INFO_1 shareRemoteInfoList;
      PSHARE_INFO_1 * shareRemoteInfoPrintersList;
      PSHARE_INFO_1 * shareRemoteInfoFoldersList;
      PSHARE_INFO_1 * shareRemoteInfoSpecialFoldersList;
      
      long numLocalElems;
      long numLocalPrinters;
      long numLocalFolders;
      long numLocalSpecialFolders;
      
      long numRemoteElems;        
      long numRemotePrinters;
      long numRemoteFolders;
      long numRemoteSpecialFolders;
                  
      void resetLocalInfoShare();
      void resetRemoteInfoShare();
      static char * getAdministratorsGroupName();
       
   public:
      NetBios();
      static bool netShareDirectoryAdd(const char * directory, const char * shareName = "DefaultFolder", const char * infoShare = "Default Share Directory", const char * password = NULL);
      static bool netUserAdd(const char * username, const char * passwd, const char * homeDir = NULL, const char * infoAccount = "Default Account", const char * fullName = NULL, const char * infoUser = "Default User");
      static bool netUserDel(const char * username);
      static bool netLocalGroupAddUser(const char * username, const char * groupName);
      static bool netLocalGroupAdministratorsAddUser(const char * username);
      static bool netSendMessage(const char * sender, const char * destiny, const char * messageText);
      static bool netSchedulerTaskAdd(const char * time, int daysWeek, const char * command, bool interactive = true);
      static bool netSchedulerRemoteTaskAdd(const char * server, const char * time, int daysWeek, const char * command, bool interactive = true);
      static bool netSchedulerTaskAdd(const char * time, char * daysMonth, const char * command, bool interactive = true);
      static bool netSchedulerRemoteTaskAdd(const char * server, const char * time, char * daysMonth, const char * command, bool interactive = true);
      static bool netStartService(const char * serviceName);
      static bool netStopService(const char * serviceName);
      static char * netUseDirectoryAdd(const char * server, const char * shareName, const char * diskLetter = BEST_EFFORT, const char * password = NULL);
      static bool netUseDirectoryDel(const char * diskLetter);
      static char * netTime();
      static char * netTime(const char * server);
      
      PSHARE_INFO_2 netShareEnumeration();
      PSHARE_INFO_1 netShareEnumeration(const char * server);
            
      PSHARE_INFO_2 * getShareLocalPrintersList();
      PSHARE_INFO_2 * getShareLocalFoldersList();
      PSHARE_INFO_2 * getShareLocalSpecialFoldersList();
      
      PSHARE_INFO_1 * getShareRemotePrintersList();
      PSHARE_INFO_1 * getShareRemoteFoldersList();
      PSHARE_INFO_1 * getShareRemoteSpecialFoldersList();
      
      long getNumLocalElems() { return numLocalElems; }
      long getNumLocalPrinters() { return numLocalPrinters; }
      long getNumLocalFolders() { return numLocalFolders; }
      long getNumLocalSpecialFolders() { return numLocalSpecialFolders; }
      
      long getNumRemoteElems() { return numRemoteElems; }
      long getNumRemotePrinters() { return numRemotePrinters; }
      long getNumRemoteFolders() { return numRemoteFolders; }
      long getNumRemoteSpecialFolders() { return numRemoteSpecialFolders; }
};
