#include "P2P.h"

#pragma once

using namespace std;

class P2PAres : public P2P {
      
   public:
      P2PAres(const char * rootFolderPath, const char * incomingFolderPath);
      ~P2PAres() { delete [] incomingFolderFullPath; }
      
      bool scanListeningPort();
};
