#include <windows.h>
#include <strings.h>
#include <malloc.h>
#include <winreg.h>

#define SIZE_BUFFER                  100
#define DEFAULT_BINARY_DATA_SIZE     -1
#pragma once

/* ################################## DATA TYPE ##################################

 REG_BINARY               - Binary data in any form.
 REG_DWORD                - A 32-bit number.
 REG_DWORD_LITTLE_ENDIAN  - A 32-bit number in little-endian format.
 REG_DWORD_BIG_ENDIAN     - A 32-bit number in big-endian format.
 REG_EXPAND_SZ            - A null-terminated string that contains unexpanded references to environment variables.
 REG_LINK                 - A null-terminated Unicode string that contains the target path of a symbolic link that 
                          was created by calling the RegCreateKeyEx function with REG_OPTION_CREATE_LINK.
 REG_MULTI_SZ             - A sequence of null-terminated strings, terminated by an empty string (\0).
                          ex -> String1\0String2\0String3\0LastString\0\0
 REG_NONE                 - No defined value type.
 REG_QWORD                - A 64-bit number.
 REG_QWORD_LITTLE_ENDIAN  - A 64-bit number in little-endian format.
 REG_SZ                   - A null-terminated string.

##################################################################################*/

class WINRegistry {

   private:
      HKEY keyHandle;
      char * tempValue;
      char ** arrayKeys;
      long numElems;
      
      long RegOpen(char * key, REGSAM attr = KEY_ALL_ACCESS);
      long SearchEngine(char * key, const char * value, int mode);
      void Clear();
      void ClearEnum();
      
   public:
      WINRegistry();
      ~WINRegistry() { delete [] tempValue; }  
      
      bool RegQueryExists(char * key);       
      bool RegQueryExists(char * key, const char * value);
      char * RegQueryValue(char * key, const char * value);
      bool RegSetBINARYValue(char * key, const char * value, const char * data, int size = -1);
      bool RegSetDWORDValue(char * key, const char * value, const char * data);
      bool RegSetASCIIValue(char * key, const char * value, const char * data);
      bool RegCreateKey(char * key, char * newKeyName);
      char ** RegQueryEnumeration(char * key);
      char ** RegQueryEnumerationSoftware();
      char ** RegQueryEnumerationCurrentUserSoftware();
      char ** RegQueryEnumerationUninstallSoftware();
      long getCountElems() { return numElems; }
};
