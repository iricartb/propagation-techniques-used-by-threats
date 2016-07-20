#include "P2P.h"

#pragma once

using namespace std;

class P2PLimeWire : public P2P {
      
   public:
      P2PLimeWire(const char * rootFolderPath, const char * incomingFolderPath);
      ~P2PLimeWire() { delete [] incomingFolderFullPath; }
      
      bool scanListeningPort();
};
