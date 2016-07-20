#include "P2P.h"
#include "FunctionsStrings.h"

#pragma once

using namespace std;

class P2PBitComet : public P2P {
      
   public:
      P2PBitComet(const char * rootFolderPath, const char * incomingFolderPath);
      ~P2PBitComet() { delete [] incomingFolderFullPath; }
      
      bool scanListeningPort();
};
