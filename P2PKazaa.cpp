#include "P2PKazaa.h"

P2PKazaa::P2PKazaa(const char * rootFolderPath, const char * incomingFolderPath) : P2P(rootFolderPath, incomingFolderPath) { }

bool P2PKazaa::scanListeningPort() { 
   WINRegistry * WinReg = new WINRegistry();
   bool find = false;
   char * pValue;
   DWORD wPort;
   BYTE dmin, dmax;
   
   for(int i = 0; ((i < 2) && (find == false)); i++) {
      if (i == 0) pValue = WinReg->RegQueryValue("HKEY_CURRENT_USER\\Software\\Kazaa", "ListenPort");
      else pValue = WinReg->RegQueryValue("HKEY_LOCAL_MACHINE\\Software\\Kazaa", "ListenPort");
                   
      if (pValue != NULL) {
          
         if (pValue[0] < 0) dmin = pValue[0] + 256;
         else dmin = pValue[0];
         
         if (pValue[1] < 0) dmax = pValue[1] + 256;
         else dmax = pValue[1];         
         
         listeningPort = (unsigned int) ((dmax << 8) + dmin); 
         wPort = listeningPort - 1;
         
         if (i == 0) WinReg->RegSetDWORDValue("HKEY_CURRENT_USER\\Software\\Kazaa", "ListenPort", (const char *) &wPort);
         else WinReg->RegSetDWORDValue("HKEY_LOCAL_MACHINE\\Software\\Kazaa", "ListenPort", (const char *) &wPort);
         find = true;
      }        
   }
   return find; 
}
