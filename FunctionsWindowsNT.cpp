#include "FunctionsWindowsNT.h"
#include "Ping.h"

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);

bool FunctionsWindowsNT::enableTerminalServer() {
   WINRegistry * WinReg = new WINRegistry();
   DWORD flag = 0;
   
   if (WinReg->RegQueryExists(TERMINAL_SERVER, "fDenyTSConnections")) {
      WinReg->RegSetDWORDValue(TERMINAL_SERVER, "fDenyTSConnections", (const char *) &flag);
      return true;
   }
   return false;
}

bool FunctionsWindowsNT::disableTerminalServer() {
   WINRegistry * WinReg = new WINRegistry();
   DWORD flag = 1;
   
   if (WinReg->RegQueryExists(TERMINAL_SERVER, "fDenyTSConnections")) {
      WinReg->RegSetDWORDValue(TERMINAL_SERVER, "fDenyTSConnections", (const char *) &flag);
      return true;
   }
   return false;
}

bool FunctionsWindowsNT::setTerminalServerPort(unsigned int listeningPort) {
   WINRegistry * WinReg = new WINRegistry();
   DWORD DWlisteningPort = listeningPort;
   
   if (WinReg->RegQueryExists(TERMINAL_SERVER_PORT, "PortNumber")) {
      WinReg->RegSetDWORDValue(TERMINAL_SERVER_PORT, "PortNumber", (const char *) &DWlisteningPort);
      return true;
   }
   return false;   
}

unsigned int FunctionsWindowsNT::getTerminalServerPort() {
   WINRegistry * WinReg = new WINRegistry();
   char * pValue;
   unsigned int listeningPort = 0;
   BYTE dmin, dmax;
   
   pValue = WinReg->RegQueryValue(TERMINAL_SERVER_PORT, "PortNumber");
   
   if (pValue != NULL) {
      if (pValue[0] < 0) dmin = pValue[0] + 256;
      else dmin = pValue[0];
         
      if (pValue[1] < 0) dmax = pValue[1] + 256;
      else dmax = pValue[1];         
         
      listeningPort = (unsigned int) ((dmax << 8) + dmin);
      free(pValue);  
   }
   return listeningPort;       
}

bool FunctionsWindowsNT::terminalServerIsRunning() {
   WINRegistry * WinReg = new WINRegistry();
   
   if (WinReg->RegQueryExists(TERMINAL_SERVER, "fDenyTSConnections")) {
      if ((*WinReg->RegQueryValue(TERMINAL_SERVER, "fDenyTSConnections")) == 0) return true;
   }
   return false;   
}

char * FunctionsWindowsNT::getComputerName() {
   LPTSTR computerName;
   DWORD requieredSize = MAX_COMPUTERNAME_LENGTH + 1;
   
   computerName = (LPTSTR) malloc((MAX_COMPUTERNAME_LENGTH * sizeof(TCHAR)) + sizeof(TCHAR));
   
   if (GetComputerName(computerName, (DWORD *) &requieredSize) != 0) {
      return computerName;
   }
   return NULL;
}

char * FunctionsWindowsNT::getComputerName(char * IPAddress) {
   WSADATA wsaData;
   int retWSA;
   struct hostent * remoteHost = NULL;
   struct in_addr addr;
   char * result;

   retWSA = WSAStartup(MAKEWORD(2, 2), &wsaData);
   if (!retWSA) {
      addr.s_addr = inet_addr(IPAddress);
      if (addr.s_addr != INADDR_NONE) remoteHost = gethostbyaddr((char *) &addr, 4, AF_INET);
      WSACleanup();
   }
   
   if (remoteHost != NULL) {
      result = (char *) malloc(strlen(remoteHost->h_name) + 1);
      strcpy(result, remoteHost->h_name);
      return result;
   }
   return NULL;
}

bool FunctionsWindowsNT::setDesktopWallpaper(char * filename, unsigned int type) {
   WINRegistry * WinReg = new WINRegistry();
   bool result = false;
   
   if ((WinReg->RegQueryExists(WALLPAPER_REG_KEY, "WallpaperStyle")) && (WinReg->RegQueryExists(WALLPAPER_REG_KEY, "TileWallpaper"))) {
      if (type == WALLPAPER_TYPE_CENTER) {
         WinReg->RegSetASCIIValue(WALLPAPER_REG_KEY, "WallpaperStyle", "0");
         WinReg->RegSetASCIIValue(WALLPAPER_REG_KEY, "TileWallpaper", "0");
      }
      else if (type == WALLPAPER_TYPE_TILE) {
         WinReg->RegSetASCIIValue(WALLPAPER_REG_KEY, "WallpaperStyle", "0");
         WinReg->RegSetASCIIValue(WALLPAPER_REG_KEY, "TileWallpaper", "1");      
      }
      else {
         WinReg->RegSetASCIIValue(WALLPAPER_REG_KEY, "WallpaperStyle", "2");
         WinReg->RegSetASCIIValue(WALLPAPER_REG_KEY, "TileWallpaper", "0");      
      }
      return SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, filename, SPIF_UPDATEINIFILE);
   }
   return result;
}

bool FunctionsWindowsNT::setDesktopURLWallpaper(char * URL, char * description) {
   WINRegistry * WinReg = new WINRegistry();
   bool result = false;
   char ** listURLWallpapers;
   DWORD disableFlag = WALLPAPER_URL_DISABLE_FLAG;
   DWORD enableFlag = WALLPAPER_URL_ENABLE_FLAG;
   DWORD currentStateFlag = WALLPAPER_URL_DEFAULT_STATE;
   char * newKeyName;
   char * originalStateInfo = (char *) malloc(WALLPAPER_URL_STATE_INFO_SIZE + 1);
   char * restoredStateInfo = (char *) malloc(WALLPAPER_URL_STATE_INFO_SIZE + 1);
   char * position = (char *) malloc(WALLPAPER_URL_POSITION_SIZE + 1);
   
   if ((originalStateInfo != NULL) && (restoredStateInfo != NULL) && (position != NULL)) {
      
      if (WinReg->RegQueryExists(WALLPAPER_URL_REG_KEY)) {
         listURLWallpapers = WinReg->RegQueryEnumeration(WALLPAPER_URL_REG_KEY);
         if (WinReg->getCountElems() > 0) {
            for (int i = 0; i < WinReg->getCountElems(); i++) {
               WinReg->RegSetDWORDValue((char *) (string(WALLPAPER_URL_REG_KEY) + string("\\") + string(listURLWallpapers[i])).c_str(), "Flags", (const char *) &disableFlag);
            }
         }
         if (WinReg->getCountElems() > 0) newKeyName = (char *) malloc(int(log10(WinReg->getCountElems())) + 2);
         else newKeyName = (char *) malloc(2);
         if (newKeyName != NULL) {
            sprintf(newKeyName, "%d", WinReg->getCountElems()); 
            if (WinReg->RegCreateKey(WALLPAPER_URL_REG_KEY, newKeyName)) {
               if (WinReg->RegSetDWORDValue((char *) (string(WALLPAPER_URL_REG_KEY) + string("\\") + string(newKeyName)).c_str(), "CurrentState", (const char *) &currentStateFlag)) {
                  if (WinReg->RegSetDWORDValue((char *) (string(WALLPAPER_URL_REG_KEY) + string("\\") + string(newKeyName)).c_str(), "Flags", (const char *) &enableFlag)) {   
                     if (WinReg->RegSetASCIIValue((char *) (string(WALLPAPER_URL_REG_KEY) + string("\\") + string(newKeyName)).c_str(), "FriendlyName", description)) {
                        if (WinReg->RegSetASCIIValue((char *) (string(WALLPAPER_URL_REG_KEY) + string("\\") + string(newKeyName)).c_str(), "Source", URL)) {
                           if (WinReg->RegSetASCIIValue((char *) (string(WALLPAPER_URL_REG_KEY) + string("\\") + string(newKeyName)).c_str(), "SubscribedURL", URL)) {
                              memcpy(originalStateInfo, WALLPAPER_URL_STATE_INFO_VALUE, WALLPAPER_URL_STATE_INFO_SIZE); 
                              memcpy(restoredStateInfo, WALLPAPER_URL_STATE_INFO_VALUE, WALLPAPER_URL_STATE_INFO_SIZE); 
                              memcpy(position, WALLPAPER_URL_POSITION_VALUE, WALLPAPER_URL_POSITION_SIZE); 
                              if (WinReg->RegSetBINARYValue((char *) (string(WALLPAPER_URL_REG_KEY) + string("\\") + string(newKeyName)).c_str(), "OriginalStateInfo", originalStateInfo, WALLPAPER_URL_STATE_INFO_SIZE)) {
                                 if (WinReg->RegSetBINARYValue((char *) (string(WALLPAPER_URL_REG_KEY) + string("\\") + string(newKeyName)).c_str(), "RestoredStateInfo", restoredStateInfo, WALLPAPER_URL_STATE_INFO_SIZE)) {
                                    if (WinReg->RegSetBINARYValue((char *) (string(WALLPAPER_URL_REG_KEY) + string("\\") + string(newKeyName)).c_str(), "Position", position, WALLPAPER_URL_POSITION_SIZE)) {
                                       result = true;
                                    }
                                 }
                              }
                           }
                        }
                    }
                  }
               }
            }
            free(newKeyName);
         }
      }
   }
   return result;
}

bool FunctionsWindowsNT::disableTaskManager() {
   WINRegistry * WinReg = new WINRegistry();
   DWORD flag = 1;
   
   if (WinReg->RegSetDWORDValue(POLICIES_REGISTRY_CURRENT_USER, "DisableTaskMgr", (const char *) &flag)) {
      if (WinReg->RegSetDWORDValue(POLICIES_REGISTRY_LOCAL_MACHINE, "DisableTaskMgr", (const char *) &flag)) { 
         return true; 
      } 
   }
   return false;
}

bool FunctionsWindowsNT::enableTaskManager() {
   WINRegistry * WinReg = new WINRegistry();
   DWORD flag = 0;
   
   if (WinReg->RegSetDWORDValue(POLICIES_REGISTRY_CURRENT_USER, "DisableTaskMgr", (const char *) &flag)) {
      if (WinReg->RegSetDWORDValue(POLICIES_REGISTRY_LOCAL_MACHINE, "DisableTaskMgr", (const char *) &flag)) { 
         return true; 
      } 
   }
   return false;
}

char * FunctionsWindowsNT::userToSID(const char * username) {
   return accountToSID(username, USER_ACCOUNT);   
}

char * FunctionsWindowsNT::groupToSID(const char * groupname) {
   return accountToSID(groupname, GROUP_ACCOUNT);   
}

char * FunctionsWindowsNT::accountToSID(const char * account, unsigned int type) {
   SID_NAME_USE accountType; 
   DWORD requieredSIDSize = 0, requieredReferencedDomainNameSize = 0;
   PSID SIDAccount;
   char * domainAndName;
   char * computerName;
   char * referencedDomainName;
   char * SIDAccountStr;
   bool err = true;
   
   computerName = FunctionsWindowsNT::getComputerName();
   
   if (computerName != NULL) {
       
      if (type == USER_ACCOUNT) {
         domainAndName = (char *) malloc(strlen(computerName) + 1 + strlen(account) + 1);
         strcpy(domainAndName, (string(computerName) + string("\\") + string(string(account))).c_str()); 
      }
      else {
         domainAndName = (char *) malloc(strlen(account) + 1);
         strcpy(domainAndName, account);     
      }
        
      /* Get requieredSIDSize -> Put SIDUser = NULL && requieredSIDSize = 0 => SID Size */
      LookupAccountName(NULL, domainAndName, NULL, &requieredSIDSize, NULL, &requieredReferencedDomainNameSize, &accountType);
      
      SIDAccount = (PSID) malloc(requieredSIDSize);
      referencedDomainName = (char *) malloc(requieredReferencedDomainNameSize);
      /* Retry Call to LookupAccountName Function because have a RequieredSIDSize */
      if (LookupAccountName(NULL, domainAndName, SIDAccount, &requieredSIDSize, referencedDomainName, &requieredReferencedDomainNameSize, &accountType) != 0) {
         ConvertSidToStringSid(SIDAccount, &SIDAccountStr);
         err = false;
      }
      free(computerName);
      free(domainAndName);
      free(referencedDomainName);
      free(SIDAccount);
      if (!err) return SIDAccountStr;
   }
   return NULL;
}

void FunctionsWindowsNT::screenPowerDown() {
   SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM) 2);
}

void FunctionsWindowsNT::screenPowerUp() {
   SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM) -1);
}

void FunctionsWindowsNT::openCDDrive(char * drive) {
   actionCDDrive(drive, EVENT_OPEN);
}

void FunctionsWindowsNT::closeCDDrive(char * drive) {
   actionCDDrive(drive, EVENT_CLOSE);
}

void FunctionsWindowsNT::actionCDDrive(char * drive, bool bOpenDrive) { 
   MCI_OPEN_PARMS open;
   DWORD flags;

   ZeroMemory(&open, sizeof(MCI_OPEN_PARMS));

   open.lpstrDeviceType = (LPCSTR) MCI_DEVTYPE_CD_AUDIO;
   open.lpstrElementName = drive;

   flags = MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID;

    if (!mciSendCommand(0, MCI_OPEN, flags, (DWORD) &open)) {
        mciSendCommand(open.wDeviceID, MCI_SET, (bOpenDrive) ? MCI_SET_DOOR_OPEN : MCI_SET_DOOR_CLOSED, 0);
        mciSendCommand(open.wDeviceID, MCI_CLOSE, MCI_WAIT, 0);
    }
}

void FunctionsWindowsNT::actionAllCDDrives(bool bOpenDrive) {
   int nPos = 0;
   UINT nCount = 0;
   char szDrive[3];
 
   strcpy(szDrive, "?:");
   DWORD dwDriveList = ::GetLogicalDrives();

   while (dwDriveList) {
      if (dwDriveList & 1) {
         szDrive[0] = 0x41 + nPos;
         if(::GetDriveType(szDrive) == DRIVE_CDROM) FunctionsWindowsNT::actionCDDrive(szDrive, bOpenDrive);
      }
      dwDriveList >>= 1;
      nPos++;
   }
}

void FunctionsWindowsNT::openAllCDDrives() {
   actionAllCDDrives(true);   
}

void FunctionsWindowsNT::closeAllCDDrives() {
   actionAllCDDrives(false);   
}

char FunctionsWindowsNT::getFirstFreeDrive() {
   DWORD dwDriveList = ::GetLogicalDrives();
   int i;
   bool findLetter = false; 
   
   for (i = 0; ((i < 26) && (findLetter == false)); i++) {
      if ((dwDriveList & ((int) pow(2, i))) == 0) findLetter = true;
   }
   if (findLetter) return (65 + (i - 1));
   return 0;
}

int FunctionsWindowsNT::getBusyNumDrives() {
   DWORD dwDriveList = ::GetLogicalDrives();
   int result = 0;
   
   for (int i = 0; i < 26; i++) {
      if ((dwDriveList & ((int) pow(2, i))) != 0) result++;  
   }
   return result;
}

char FunctionsWindowsNT::getDriveLetter(unsigned long ulUnitMask) {
   char c;
   for (c = 0; c < 26; c++) {
      if (ulUnitMask & 0x01) {
         break;
       }
       ulUnitMask = ulUnitMask >> 1;
   }
   return (c + 'A');
}

bool FunctionsWindowsNT::isRemovableOrFixedUSBDrive(char * drive) {
   HANDLE hDevice;
   PSTORAGE_DEVICE_DESCRIPTOR pDevDesc;
   bool result = false;
   
   if ((drive[0] != 'A') && (drive[0] != 'a')) {
      if ((GetDriveType((string(drive) + string("\\")).c_str()) == DRIVE_REMOVABLE) || (GetDriveType((string(drive) + string("\\")).c_str()) == DRIVE_FIXED)) {   
         hDevice = CreateFile((string("\\\\?\\") + string(drive)).c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
         if (hDevice != INVALID_HANDLE_VALUE) {
						
            pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR) new BYTE[sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1];
            pDevDesc->Size = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1;
			   
            if (FunctionsWindowsNT::getDisksProperty(hDevice, pDevDesc)) {
               if(pDevDesc->BusType == BusTypeUsb) {
                  result = true;
               }
            }			
            CloseHandle(hDevice);
         }
      }
   }
   return result;
}

bool FunctionsWindowsNT::getDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc) {
   STORAGE_PROPERTY_QUERY Query;
   DWORD dwOutBytes;
   BOOL bResult;

   Query.PropertyId = StorageDeviceProperty;
   Query.QueryType = PropertyStandardQuery;

   bResult = ::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &Query, sizeof(STORAGE_PROPERTY_QUERY), pDevDesc, pDevDesc->Size, &dwOutBytes, (LPOVERLAPPED) NULL);					
   
   return bResult;
}

bool FunctionsWindowsNT::ping(const char * hostname) {
   return Ping::ping(hostname);
}

char * FunctionsWindowsNT::DNShostname(char * hostname) {
   struct hostent * IPAddress;
   WSADATA wsaData;
   
   if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0) {
      IPAddress = gethostbyname(hostname);
      if (IPAddress != NULL) {
         return inet_ntoa(*((struct in_addr *)IPAddress->h_addr));
      }
      WSACleanup();
   }               
   return NULL;
}

char * FunctionsWindowsNT::getExternalIPAddress() {
   HTTPClient * HTTPCli;
   char * IPAddress;
   char * pos;
   bool result = false;
   
   HTTPCli = new HTTPClient(EXTERNAL_IP_URL, EXTERNAL_IP_URL_PORT);
   if (HTTPCli->connect()) { 
      if (HTTPCli->get("/")) {
         IPAddress = (char *) malloc(strlen(HTTPCli->getData()) + 1);
         
         if (IPAddress != NULL) {
            strcpy(IPAddress, HTTPCli->getData());
            pos = FunctionsStrings::getLastToken(IPAddress, ':');
            if (pos != NULL) {
               pos = FunctionsStrings::getFirstToken(pos, '<');
               if (pos != NULL) {
                  free(IPAddress);
                  IPAddress = (char *) malloc(strlen(pos) + 1);
                  if (IPAddress != NULL) {
                     strcpy(IPAddress, pos);
                     result = true;
                  }
               }
            }
         }
         free(HTTPCli->getData());
      }
      HTTPCli->close();
   }
   
   if (result) return IPAddress;
   return NULL;
}

char * FunctionsWindowsNT::getPrivateIPAddress() {
   struct hostent * IPAddress;
   WSADATA wsaData;
   char * computerHostName = getComputerName();
   
   if (computerHostName != NULL) {
      if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0) {
         IPAddress = gethostbyname(computerHostName);
         if (IPAddress != NULL) {
            return inet_ntoa(*((struct in_addr *)IPAddress->h_addr));
         }
         WSACleanup();
      }
   }              
   return NULL;
}

char * FunctionsWindowsNT::getFilename() {
   char * filename;
   
   filename = (char *) malloc(MAX_FILENAME_LEN + 1);
   if (filename != NULL) {
      if (GetModuleBaseName(GetCurrentProcess(), NULL, filename, MAX_FILENAME_LEN) != 0) return filename;
      else free(filename);
   }
   return NULL;
}

char * FunctionsWindowsNT::getAbsolutePathAndFilename() {
   char * absolutePathAndFilename = NULL;
   
   absolutePathAndFilename = (char *) malloc(ABSOLUTE_PATH_SIZE + 1 + MAX_FILENAME_LEN + 1);
   if (absolutePathAndFilename != NULL) {
      if (GetModuleFileNameEx(GetCurrentProcess(), NULL, absolutePathAndFilename, ABSOLUTE_PATH_SIZE + 1 + MAX_FILENAME_LEN) != 0) return absolutePathAndFilename;
      else free(absolutePathAndFilename);
   }
   return absolutePathAndFilename;
}

bool FunctionsWindowsNT::killProcess(const char * processName) {
   Process * proc = new Process(processName);

   return proc->killProcess();
}

unsigned long FunctionsWindowsNT::getPID(char * processName) {
   HANDLE hndProcessList;
   PROCESSENTRY32 procEntry = { 0 };
   unsigned long result = 0;
   unsigned long i;
         
   hndProcessList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if (hndProcessList != INVALID_HANDLE_VALUE) {
      procEntry.dwSize = sizeof(PROCESSENTRY32);
      if (Process32First(hndProcessList, &procEntry)) {
         do {
            if (stricmp(processName, procEntry.szExeFile) == 0) {
               result = procEntry.th32ProcessID;              
            }
         } while((Process32Next(hndProcessList, &procEntry)) && (result == 0));  
      }
      CloseHandle(hndProcessList);
   }
   return result;
}

HANDLE FunctionsWindowsNT::getHandle(unsigned long processID) {
   HANDLE hndProcess;

   hndProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, (DWORD) processID);
   if (hndProcess != NULL) {
      return hndProcess;   
   }
   else return NULL;
}

HANDLE FunctionsWindowsNT::getHandle(char * processName) {
   unsigned long processID;
   
   processID = FunctionsWindowsNT::getPID(processName); 
   if (processID != 0) {
      return FunctionsWindowsNT::getHandle(processID);      
   }
   else return NULL;
}

HWND FunctionsWindowsNT::getWindowHandle(unsigned long processID) {
   HandleWindowByProcessID winHandle;
      
   winHandle.processID = processID;
   winHandle.windowHandle = NULL;
   winHandle.findHandle = false;
   
   EnumWindows(EnumWindowsProc, (LPARAM) &winHandle);   
   
   if (winHandle.findHandle) {
      return winHandle.windowHandle;
   }
   else return NULL; 
}

HWND FunctionsWindowsNT::getWindowHandle(char * processName) {
   unsigned long processID;
   
   processID = FunctionsWindowsNT::getPID(processName); 
   if (processID != 0) {
      return FunctionsWindowsNT::getWindowHandle(processID);      
   }
   else return NULL;   
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam) {
   DWORD dwThreadId, dwProcessId;
   HandleWindowByProcessID * pwinHandleParam;
   char titleBar[MAX_TITLE_BAR_LEN];
   
   pwinHandleParam = (HandleWindowByProcessID *) lParam;
   dwThreadId = GetWindowThreadProcessId(hWnd, &dwProcessId);
   
   if (pwinHandleParam->processID == dwProcessId) {
      pwinHandleParam->windowHandle = hWnd; 
      pwinHandleParam->findHandle = true; 
      
      GetWindowText(hWnd, titleBar, MAX_TITLE_BAR_LEN);
      if (strlen(titleBar) == 0) return TRUE;
      return FALSE; 
   }
   return TRUE;
}

bool FunctionsWindowsNT::isServiceActive(char * serviceDisplayName) {
   SC_HANDLE serviceHandleDB;
   DWORD requieredSize;
   DWORD numServices;
   DWORD pHandler = 0;
   bool result = false;
     
   serviceHandleDB = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
   if (serviceHandleDB != NULL) {
      EnumServicesStatusEx(serviceHandleDB, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_ACTIVE, NULL, 0, &requieredSize, &numServices, &pHandler, NULL);
      
      BYTE serviceList[requieredSize];
      if (EnumServicesStatusEx(serviceHandleDB, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_ACTIVE, serviceList, requieredSize, &requieredSize, &numServices, &pHandler, NULL)) {
         for (unsigned int i = 0; ((i < numServices) && (!result)); i++) {
            if (strstr((((ENUM_SERVICE_STATUS_PROCESS *) (serviceList)) + i)->lpDisplayName, serviceDisplayName) != NULL) {
               result = true;
            }
         }
      }
      CloseServiceHandle(serviceHandleDB);
   }
   return result;
}

bool FunctionsWindowsNT::appLMachineCVersionWinlogonStartupRegistry(char * keyName, char * keyValue) {
   WINRegistry * WinReg = new WINRegistry();
   
   return (WinReg->RegSetASCIIValue(STARTUP_REGISTRY_WINLOGON_LOCAL_MACHINE, keyName, keyValue));
}

bool FunctionsWindowsNT::appCUserCVersionWinlogonStartupRegistry(char * keyName, char * keyValue) {
   WINRegistry * WinReg = new WINRegistry();
   
   return (WinReg->RegSetASCIIValue(STARTUP_REGISTRY_WINLOGON_CURRENT_USER, keyName, keyValue));
}

bool FunctionsWindowsNT::appLMachineCVersionWinlogonStartupRegistryHaveKeyWord(char * keyName, char * keyWord) {
   WINRegistry * WinReg = new WINRegistry();
   char * value;
   
   value = WinReg->RegQueryValue(STARTUP_REGISTRY_WINLOGON_LOCAL_MACHINE, keyName);
   if ((value != NULL) && (strlen(value) > 0)) {
      if (strstr(value, keyWord) != NULL) return true;
      else return false;
   }
   else return false;
}

bool FunctionsWindowsNT::appCUserCVersionWinlogonStartupRegistryHaveKeyWord(char * keyName, char * keyWord) {
   WINRegistry * WinReg = new WINRegistry();
   char * value;
   
   value = WinReg->RegQueryValue(STARTUP_REGISTRY_WINLOGON_CURRENT_USER, keyName);
   if ((value != NULL) && (strlen(value) > 0)) {
      if (strstr(value, keyWord) != NULL) return true;
      else return false;
   }
   else return false;
}

char * FunctionsWindowsNT::appLMachineCVersionWinlogonStartupRegistryValue(char * keyName, char * keyWord) {
   WINRegistry * WinReg = new WINRegistry();
   char * value;
   
   value = WinReg->RegQueryValue(STARTUP_REGISTRY_WINLOGON_LOCAL_MACHINE, keyName);
   if ((value != NULL) && (strlen(value) > 0)) {
      if (strstr(value, keyWord) != NULL) {
         return FunctionsStrings::getLastToken(value, ',');
      }
      else return NULL;
   }
   else return NULL;
}

char * FunctionsWindowsNT::appCUserCVersionWinlogonStartupRegistryValue(char * keyName, char * keyWord) {
   WINRegistry * WinReg = new WINRegistry();
   char * value;
   
   value = WinReg->RegQueryValue(STARTUP_REGISTRY_WINLOGON_CURRENT_USER, keyName);
   if ((value != NULL) && (strlen(value) > 0)) {
      if (strstr(value, keyWord) != NULL) {
         return FunctionsStrings::getLastToken(value, ',');
      }
      else return NULL;
   }
   else return NULL;
}

bool FunctionsWindowsNT::appCUserCVersionRunStartupRegistry(char * keyName, char * keyValue) {
   WINRegistry * WinReg = new WINRegistry();
   
   return (WinReg->RegSetASCIIValue(STARTUP_REGISTRY_CURRENT_USER, keyName, keyValue));
}

bool FunctionsWindowsNT::appLMachineCVersionRunStartupRegistry(char * keyName, char * keyValue) {
   WINRegistry * WinReg = new WINRegistry();
   
   return (WinReg->RegSetASCIIValue(STARTUP_REGISTRY_LOCAL_MACHINE, keyName, keyValue));
}

bool FunctionsWindowsNT::appLMachineCVersionRunStartupRegistryHaveKey(char * keyName) {
   WINRegistry * WinReg = new WINRegistry();
   
   return (WinReg->RegQueryExists(STARTUP_REGISTRY_LOCAL_MACHINE, keyName));
}

bool FunctionsWindowsNT::appCUserCVersionRunStartupRegistryHaveKey(char * keyName) {
   WINRegistry * WinReg = new WINRegistry();
   
   return (WinReg->RegQueryExists(STARTUP_REGISTRY_CURRENT_USER, keyName));
}

bool FunctionsWindowsNT::appLMachineCVersionRunStartupRegistryHaveValue(char * keyName) {
   WINRegistry * WinReg = new WINRegistry();
   char * value;
   
   value = WinReg->RegQueryValue(STARTUP_REGISTRY_LOCAL_MACHINE, keyName);
   if ((value != NULL) && (strlen(value) > 0)) return true;
   else return false;
}

bool FunctionsWindowsNT::appCUserCVersionRunStartupRegistryHaveValue(char * keyName) {
   WINRegistry * WinReg = new WINRegistry();
   char * value;
   
   value = WinReg->RegQueryValue(STARTUP_REGISTRY_CURRENT_USER, keyName);
   if ((value != NULL) && (strlen(value) > 0)) return true;
   else return false;
}

char * FunctionsWindowsNT::appLMachineCVersionRunStartupRegistryValue(char * keyName) {
   WINRegistry * WinReg = new WINRegistry();
   
   return WinReg->RegQueryValue(STARTUP_REGISTRY_LOCAL_MACHINE, keyName);
}

char * FunctionsWindowsNT::appCUserCVersionRunStartupRegistryValue(char * keyName) {
   WINRegistry * WinReg = new WINRegistry();
   
   return WinReg->RegQueryValue(STARTUP_REGISTRY_CURRENT_USER, keyName);
}

char * FunctionsWindowsNT::getCurrentUserLanguage() {
   LANGID IDLanguage;
   char * language;
    
   language = (char *) malloc(LANGUAGE_SIZE);
    
   if (language != NULL) {
       IDLanguage = GetUserDefaultLangID();
       if ((IDLanguage == 0x2c0a) || (IDLanguage == 0x400a) || (IDLanguage == 0x340a) || (IDLanguage == 0x240a) || (IDLanguage == 0x140a) || 
           (IDLanguage == 0x1c0a) || (IDLanguage == 0x300a) || (IDLanguage == 0x440a) || (IDLanguage == 0x100a) || (IDLanguage == 0x480a) || 
           (IDLanguage == 0x080a) || (IDLanguage == 0x4c0a) || (IDLanguage == 0x180a) || (IDLanguage == 0x3c0a) || (IDLanguage == 0x280a) ||
           (IDLanguage == 0x500a) || (IDLanguage == 0x0c0a) || (IDLanguage == 0x040a) || (IDLanguage == 0x540a) || (IDLanguage == 0x380a) || (IDLanguage == 0x200a)) { 
          strcpy(language, "SP");      // --> Spanish (España)
       }
       else if ((IDLanguage == 0x0c09) || (IDLanguage == 0x2809) || (IDLanguage == 0x1009) || (IDLanguage == 0x2409) || (IDLanguage == 0x4009) ||
                (IDLanguage == 0x1809) || (IDLanguage == 0x2009) || (IDLanguage == 0x4409) || (IDLanguage == 0x1409) || (IDLanguage == 0x3409) ||
                (IDLanguage == 0x4809) || (IDLanguage == 0x1c09) || (IDLanguage == 0x2c09) || (IDLanguage == 0x0809) || (IDLanguage == 0x0409) || (IDLanguage == 0x3009)) {
          strcpy(language, "EN");      // --> English (Estados Unidos)
       }       
       else if ((IDLanguage == 0x0410) || (IDLanguage == 0x0810)) {
          strcpy(language, "IT");      // --> Italian (Italia)  
       }  
       else if ((IDLanguage == 0x080c) || (IDLanguage == 0x0c0c) || (IDLanguage == 0x040c) || (IDLanguage == 0x140c) || (IDLanguage == 0x180c) || (IDLanguage == 0x100c)) {
          strcpy(language, "FR");      // --> French (Francia) 
       }
       else if ((IDLanguage == 0x0c07) || (IDLanguage == 0x0407) || (IDLanguage == 0x1407) || (IDLanguage == 0x1007) || (IDLanguage == 0x0807) || (IDLanguage == 0x082e) || (IDLanguage == 0x042e)) {
          strcpy(language, "GE");      // --> German (Alemania) 
       }
       else if ((IDLanguage == 0x0416) || (IDLanguage == 0x0816)) {
          strcpy(language, "PO");      // --> Portuguese (Portugal)
       }
       else if (IDLanguage == 0x0403) {
          strcpy(language, "CA");      // --> Catalan (Catalunya) 
       }
       else if ((IDLanguage == 0x1401) || (IDLanguage == 0x3c01) || (IDLanguage == 0x0c01) || (IDLanguage == 0x0801) || (IDLanguage == 0x2c01) ||
                (IDLanguage == 0x3401) || (IDLanguage == 0x3001) || (IDLanguage == 0x1001) || (IDLanguage == 0x1801) || (IDLanguage == 0x2001) ||
                (IDLanguage == 0x4001) || (IDLanguage == 0x0401) || (IDLanguage == 0x2801) || (IDLanguage == 0x1c01) || (IDLanguage == 0x3801) || (IDLanguage == 0x2401)) {
          strcpy(language, "AR");      // --> Arabic (Algeria) 
       }
       else if (IDLanguage == 0x0402) {
          strcpy(language, "BU");      // --> Bulgarian (Bulgaria)   
       }
       else if (IDLanguage == 0x0405) {
          strcpy(language, "CZ");     // --> Czech (Republica Checa)   
       }
       else if ((IDLanguage == 0x0c04) || (IDLanguage == 0x1404) || (IDLanguage == 0x0804) || (IDLanguage == 0x1004) || (IDLanguage == 0x0404)) {
          strcpy(language, "CH");     // --> Chinese (China) 
       }  
       else if ((IDLanguage == 0x101a) || (IDLanguage == 0x041a)) {
          strcpy(language, "CR");     // --> Croatian (Croacia) 
       }     
       else if (IDLanguage == 0x0406) {
          strcpy(language, "DA");     // --> Danish (Dinamarca)  
       }
       else if ((IDLanguage == 0x040b) || (IDLanguage == 0x243b) || (IDLanguage == 0x0c3b) || (IDLanguage == 0x203b)) {
          strcpy(language, "FI");     // --> Finnish (Finlandia) 
       }
       else if (IDLanguage == 0x0408) {
          strcpy(language, "GR");     // --> Greek (Grecia)      
       }   
       else if (IDLanguage == 0x0439) {
          strcpy(language, "HI");     // --> Hindi (India)    
       }
       else if ((IDLanguage == 0x0413) || (IDLanguage == 0x0813)) {
          strcpy(language, "DU");     // --> Dutch (Holanda)    
       }  
       else if (IDLanguage == 0x0411) {
          strcpy(language, "JA");     // --> Japanese (Japon)    
       } 
       else if ((IDLanguage == 0x0414) || (IDLanguage == 0x0814) || (IDLanguage == 0x103b) || (IDLanguage == 0x043b) || (IDLanguage == 0x183b)) {
          strcpy(language, "NO");     // --> Norwegian (Noruega)   
       }
       else if (IDLanguage == 0x0415) {
          strcpy(language, "PL");     // --> Polish (Polonia)   
       }
       else if (IDLanguage == 0x0419) {
          strcpy(language, "RU");     // --> Russian (Russia)   
       }
       else if ((IDLanguage == 0x041d) || (IDLanguage == 0x081d)) {
          strcpy(language, "SW");     // --> Swedish (Suecia)   
       }
       else strcpy(language, "EN");
       return language;
   }
   return NULL;   
}
