#include "WINRegistry.h"

WINRegistry::WINRegistry() {
   tempValue = (char *) malloc(SIZE_BUFFER);
   numElems = 0;
}

bool WINRegistry::RegQueryExists(char * key) {
   if (this->RegOpen(key, KEY_READ) == 0) return true;
   return false;  
}

bool WINRegistry::RegQueryExists(char * key, const char * value) {
   if (SearchEngine(key, value, 0) == 0) return true;
   else return false;
}

char * WINRegistry::RegQueryValue(char * key, const char * value) {
   if (SearchEngine(key, value, 1) == 0) return tempValue;
   else return NULL;
}

bool WINRegistry::RegSetDWORDValue(char * key, const char * value, const char * data) {
   long result;
   
   if (this->RegOpen(key, KEY_SET_VALUE) == 0) {
      result = RegSetValueEx(keyHandle, value, 0, REG_DWORD, (LPBYTE) data, sizeof(DWORD));
      
      RegCloseKey(keyHandle);
      if (result == ERROR_SUCCESS) return true;
      return false;
   }
   return false;
}

bool WINRegistry::RegSetBINARYValue(char * key, const char * value, const char * data, int size) {
   long result;
   
   if (this->RegOpen(key, KEY_SET_VALUE) == 0) {
      if (size > 0) {
         result = RegSetValueEx(keyHandle, value, 0, REG_BINARY, (LPBYTE) data, size);
      }
      else result = RegSetValueEx(keyHandle, value, 0, REG_BINARY, (LPBYTE) data, strlen(data));
      
      RegCloseKey(keyHandle);
      if (result == ERROR_SUCCESS) return true;
      return false;
   }
   return false;
}

bool WINRegistry::RegCreateKey(char * key, char * newKeyName) {
   HKEY newKeyHandle;
   long result;
   
   if (this->RegOpen(key, KEY_WRITE) == 0) {
      result = RegCreateKeyEx(keyHandle, newKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &newKeyHandle, NULL);
      
      RegCloseKey(keyHandle);
      if (result == ERROR_SUCCESS) { RegCloseKey(newKeyHandle); return true; }
      return false;
   }
   return false;      
}

bool WINRegistry::RegSetASCIIValue(char * key, const char * value, const char * data) {
   long result;
   
   if (this->RegOpen(key, KEY_SET_VALUE) == 0) {
      result = RegSetValueEx(keyHandle, value, 0, REG_SZ, (LPBYTE) data, strlen(data) + 1);
      
      RegCloseKey(keyHandle);
      if (result == ERROR_SUCCESS) return true;
      return false;
   }
   return false;
}

void WINRegistry::Clear() { 
   if (tempValue != NULL) free(tempValue);
   tempValue = (char *) malloc(SIZE_BUFFER);
}

void WINRegistry::ClearEnum() {
   for(int i = 0; i < numElems; i++) free(*(arrayKeys + i));
   free(arrayKeys);
}

char ** WINRegistry::RegQueryEnumeration(char * key) {
   DWORD size = SIZE_BUFFER, acsize = SIZE_BUFFER;
   long ret;
   int i;
   
   if (this->RegOpen(key, KEY_READ) == 0) {
      this->ClearEnum();
      arrayKeys = (char **) malloc(sizeof(char *));
      *(arrayKeys) = (char *) malloc(size);
      
      for (i = 0; (ret != ERROR_NO_MORE_ITEMS); i++) {
         ret = RegEnumKeyEx(keyHandle, i, arrayKeys[i], &size, NULL, NULL, NULL, NULL);
         if (ret == ERROR_SUCCESS) { 
            arrayKeys = (char **) realloc(arrayKeys, ((i + 2) * sizeof(char *)));
            size = acsize;
            *(arrayKeys + (i + 1)) = (char *) malloc(size);
         }
         else if (ret == ERROR_MORE_DATA) {
            acsize += SIZE_BUFFER; size = acsize; 
            *(arrayKeys + i) = (char *) realloc(*(arrayKeys + i), size); 
            i--;  
         }
      }
      numElems = i - 1;
      RegCloseKey(keyHandle);
      return arrayKeys;
   }
   else return NULL;   
}

char ** WINRegistry::RegQueryEnumerationSoftware() {
   return RegQueryEnumeration("HKEY_LOCAL_MACHINE\\SOFTWARE");
}

char ** WINRegistry::RegQueryEnumerationCurrentUserSoftware() {
   return RegQueryEnumeration("HKEY_CURRENT_USER\\SOFTWARE");
}

char ** WINRegistry::RegQueryEnumerationUninstallSoftware() {
   return RegQueryEnumeration("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
}

long WINRegistry::RegOpen(char * key, REGSAM attr) {
   HKEY keyReg;    
   char regKey[strlen(key)];
   char * token, * parentKey, * subKey; 

   strcpy(regKey, key);
   token = strtok(regKey, "\\");
   if ((token != NULL) && (strcmp(key, regKey) != 0)) {
      parentKey = (char *) malloc(strlen(token) + 1); *(parentKey) = '\0';
      strcpy(parentKey, token);
        
      subKey = (char *) malloc(strlen(key) - strlen(token) + 1); *(subKey) = '\0';
      for(int i = 0;token != NULL; i++) {
        token = strtok(NULL, "\\");
        if (token != NULL) {
           if (i != 0) strcat(subKey, "\\"); 
           strcat(subKey, token); 
        }
      }
        
      if (strcmp(parentKey, "HKEY_CLASSES_ROOT") == 0) keyReg = HKEY_CLASSES_ROOT;
      else if (strcmp(parentKey, "HKEY_CURRENT_USER") == 0) keyReg = HKEY_CURRENT_USER;
      else if (strcmp(parentKey, "HKEY_LOCAL_MACHINE") == 0) keyReg = HKEY_LOCAL_MACHINE;
      else if (strcmp(parentKey, "HKEY_CURRENT_CONFIG") == 0) keyReg = HKEY_CURRENT_CONFIG;
      else if (strcmp(parentKey, "HKEY_USERS") == 0) keyReg = HKEY_USERS;
      else keyReg = (HKEY) -1;
         
      if (keyReg != ((HKEY) -1)) {
         if (RegOpenKeyEx(keyReg, subKey, 0, attr, &keyHandle) == ERROR_SUCCESS) { free(parentKey); free(subKey); return 0; }
      }
      else { free(parentKey); free(subKey); return -2; }
   }
   else return -1; 
}

long WINRegistry::SearchEngine(char * key, const char * value, int mode) {
   long result;
   DWORD sizeBuffer = SIZE_BUFFER;
     
   if (this->RegOpen(key, KEY_READ) == 0) {
      if (mode == 0) result = RegQueryValueEx(keyHandle, value, NULL, NULL, NULL, NULL);
      else {
         this->Clear();
         int i = 1;
         do {
            if (i > 1) {
               tempValue = (char *) realloc(tempValue, SIZE_BUFFER * i);
               sizeBuffer += SIZE_BUFFER;
            }
            
            result = RegQueryValueEx(keyHandle, value, NULL, NULL, (LPBYTE) tempValue, &sizeBuffer);
            i++;
         } while (result == ERROR_MORE_DATA);
      }
      RegCloseKey(keyHandle);
      return result;
   }
   else return -1;
}
