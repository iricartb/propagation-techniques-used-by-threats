#include "P2P.h"

#pragma once

using namespace std;

class P2PKazaa : public P2P {
      
   public:
      P2PKazaa(const char * rootFolderPath, const char * incomingFolderPath);
      ~P2PKazaa() { delete [] incomingFolderFullPath; }
      
      bool scanListeningPort();
};
