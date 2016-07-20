#include "P2P.h"

#pragma once

using namespace std;

class P2PEmule : public P2P {
      
   public:
      P2PEmule(const char * rootFolderPath, const char * incomingFolderPath);
      ~P2PEmule() { delete [] incomingFolderFullPath; }
      
      bool scanListeningPort();
};
