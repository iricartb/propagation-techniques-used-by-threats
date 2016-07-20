#include "FunctionsStrings.h"

const char * FunctionsStrings::getExtension(const char * filename) {
   int pos = 0;
   
   for(int i = 0; i < strlen(filename); i++) {
      if (*(filename + i) == '.') pos = i;
   }
   if (pos != 0) return (filename + pos);
   return NULL;  
}

const char * FunctionsStrings::eraseExtension(char * filename) {
   int pos = 0;
   
   for(int i = 0; i < strlen(filename); i++) {
      if (*(filename + i) == '.') pos = i;
   }
   if (pos != 0) { *(filename + pos) = '\0'; return (filename); }
   return NULL;  
}

char * FunctionsStrings::eraseLastToken(char * str, const char delimiter) {
   int pos = 0;
   
   for(int i = 0; i < strlen(str); i++) {
      if (*(str + i) == delimiter) pos = i;
   }  
   if (pos != 0) *(str + pos + 1) = '\0'; 
   
   if (*(str) == '"') return (str + 1);
   else return str;     
}

char * FunctionsStrings::getToken(char * str, const char delimiter, int index) {
   int pos = 0, numElems = 0, count = 0;
   char * result;
   char * tempStr;
   bool fi = false;
   
   tempStr = (char *) malloc(strlen(str) + 1);
   if (tempStr != NULL) {
      strcpy(tempStr, str);
      FunctionsStrings::cleanStr(tempStr, delimiter);
   
      if (index > 0) {
         numElems = FunctionsStrings::numCharactersInString(tempStr, delimiter);
         numElems++;
      
         /* index € R[1, numElems]; */
         if (index <= numElems) {
            int i;
            for(i = 0; ((i < strlen(tempStr)) && (count != index)); i++) {
               if (*(tempStr + i) == delimiter) count++;
            }
         
            if ((count == index) && (index == 1)) {
               *(tempStr + i - 1) = '\x00';
               return tempStr;   
            }
            else if ((count == index) && (index != 1)) {
               *(tempStr + i - 1) = '\x00';
               for(int j = strlen(tempStr) - 1; ((j >= 0) && (fi == false)); j--) {
                  if (*(tempStr + j) == delimiter) { pos = j; fi = true; } 
               }   
               if (*(tempStr + pos + 1) != '\x00') { 
                  result = (char *) malloc(strlen(tempStr + pos + 1) + 1);
                  if (result != NULL) {
                     strcpy(result, tempStr + pos + 1);
                     free(tempStr);
                     return result;
                  }
                  else return NULL;
               }
               else return NULL;
            }
            else if ((count != index) && (index == 1)) {
               return tempStr;
            }
            else {
               for(int j = strlen(tempStr) - 1; ((j >= 0) && (fi == false)); j--) {
                  if (*(tempStr + j) == delimiter) { pos = j; fi = true; } 
               }
               if (*(tempStr + pos + 1) != '\x00') {
                  result = (char *) malloc(strlen(tempStr + pos + 1) + 1);
                  if (result != NULL) {
                     strcpy(result, tempStr + pos + 1);
                     free(tempStr);
                     return result;
                  }
                  else return NULL;               
               }
               else return NULL;                             
            }
         }
      }
   }
   return NULL;   
}

char * FunctionsStrings::removeSpecialCharacters(const char * str) {
   char specialChars[] = ",!@#$%^&*()-+[]{}<>?;'\"|\\/~`:";
   bool findSpecialChar;
   char * newStr = (char *) malloc(strlen(str) + 1);
   unsigned int count = 0;
   
   if (newStr != NULL) {
      for(int i = 0; i < strlen(str); i++) {
         findSpecialChar = false;
         for(int j = 0; j < sizeof(specialChars); j++) {
            if (*(str + i) == specialChars[j]) {
               findSpecialChar = true;
            }
         }
         if (!findSpecialChar) {
            *(newStr + count) = *(str + i);
            count++;
         }
      }
      *(newStr + count) = '\0';
   
      return newStr;
   }
   else return ((char *) str);
}

char * FunctionsStrings::cleanStr(char * str, const char delimiter) {
   bool ini = false, fi = false;
   
   if (FunctionsStrings::numCharactersInString(str, delimiter) > 0) {   
      for(int i = 0; ((i < strlen(str)) && (ini == false)); i++) {
         if (*(str + i) == delimiter) {
            int j;
            for(j = i + 1; j < strlen(str); j++) *(str + j - 1) = *(str + j);  
            *(str + j - 1) = '\x00';
            
            i--;
         }
         else ini = true;
      }
      for(int i = strlen(str) - 1; ((i >= 0) && (fi == false)); i--) {
         if (*(str + i) == delimiter) *(str + i) = '\x00';
         else fi = true;
      }
   }
   return str;
}

char * FunctionsStrings::getLastToken(char * str, const char delimiter) {
   int pos = 0;
   
   for(int i = 0; i < strlen(str); i++) {
      if (*(str + i) == delimiter) pos = i;
   }  
   
   if ((pos != 0) && (pos != (strlen(str) - 1))) return (str + pos + 1);
   return NULL;     
}

char * FunctionsStrings::getFirstToken(char * str, const char delimiter) {
   int pos = 0;
   char * result;
   
   for(int i = 0; ((i < (strlen(str))) && (pos == 0)); i++) {
      if (*(str + i) == delimiter) pos = i;
   }  
   
   if (pos != 0) { 
      result = (char *) malloc(pos + 1);
      if (result) {
         int j;
         for(j = 0; j < pos; j++) *(result + j) = *(str + j);
         *(result + j) = '\0';
         return result;
      }   
   }
   return NULL;     
}

char * FunctionsStrings::castingHexToString(char * hexSentence) {
   char value[2], a, b;
   char * result;
   int i = 0, c = 0;
   
   result = (char *) malloc(sizeof(char));
   for(i = 0; (i < (strlen(hexSentence)/2)); i++) {  
      a = hexSentence[i*2]; b = hexSentence[((i*2)+1)];
      sprintf(value, "%c%c", a, b);
      
      if ((a == 'C') && (b == '3')) {
         if (i+1 < (strlen(hexSentence)/2)) {
            a = hexSentence[(i+1)*2]; b = hexSentence[(((i+1)*2)+1)];

            if ((a == 'A') && (b == '0')) *(result + i - c) = 'à';       // CHAR -> à                     
            else if ((a == 'A') && (b == '1')) *(result + i - c) = 'á';  // CHAR -> á
            else if ((a == 'A') && (b == '8')) *(result + i - c) = 'è';  // CHAR -> è
            else if ((a == 'A') && (b == '9')) *(result + i - c) = 'é';  // CHAR -> é
            else if ((a == 'A') && (b == 'C')) *(result + i - c) = 'ì';  // CHAR -> ì
            else if ((a == 'A') && (b == 'D')) *(result + i - c) = 'í';  // CHAR -> í
            else if ((a == 'B') && (b == '2')) *(result + i - c) = 'ò';  // CHAR -> ò
            else if ((a == 'B') && (b == '3')) *(result + i - c) = 'ó';  // CHAR -> ó
            else if ((a == 'B') && (b == '9')) *(result + i - c) = 'ù';  // CHAR -> ù
            else if ((a == 'B') && (b == 'A')) *(result + i - c) = 'ú';  // CHAR -> ú   

            i++; c++;
         }     
      }
      else *(result + i - c) = castingHexToChar(value);
      result = (char *) realloc(result, sizeof(char) * ((i-c) + 2));
   }
   *(result + i - c) = '\0';
   return result;
}

char FunctionsStrings::castingHexToChar(char * hexSentence) {
   unsigned int val;
   unsigned int result;
   
   if (strlen(hexSentence) == 2) {
      for (int i = 0; i < 2; i++) {
         if (hexSentence[i] == 'A') val = 10;
         else if (hexSentence[i] == 'B') val = 11;
         else if (hexSentence[i] == 'C') val = 12;
         else if (hexSentence[i] == 'D') val = 13;
         else if (hexSentence[i] == 'E') val = 14;
         else if (hexSentence[i] == 'F') val = 15;
         else if ((hexSentence[i] >= 48) && (hexSentence[i] <= 57)) val = hexSentence[i] - 48;
         else return 0;
         
         if (i == 0) result = val * 16;
         else result += val; 
      }
      return (char) result;   
   }
   return 0;
}

bool FunctionsStrings::getXMLvalue(char * strXML, char * buffer, unsigned int bufferLen) {
   unsigned int posIni = 0;
   bool find = false;
   bool err = false;
   
   for (int i = 0; ((i < strlen(strXML)) && (find == false) && (err == false)); i++) {
      if (posIni == 0) {
         if (strXML[i] == '>') posIni = i;
      }
      else {
         if ((strXML[i] == '<') && ((i - 1) - (posIni + 1) >= 0)) { 
            int j;
            for(j = 0; ((j < bufferLen - 1) && (j <= ((i - 1) - (posIni + 1)))); j++) {
               buffer[j] = *(strXML + posIni + j + 1);
            }
            buffer[j] = '\x00';
            find = true; 
         }
         else if ((i - 1) - (posIni + 1) < 0) err = true;
      }
   }
   return find;
}

bool FunctionsStrings::setXMLvalue(char * strXML, char * newStrXML) {
   unsigned int posIni = 0;
   bool find = false;
   bool err = false;
   
   if (strlen(newStrXML) > 0) {
      for (int i = 0; ((i < strlen(strXML)) && (find == false) && (err == false)); i++) {
         if (posIni == 0) {
            if (strXML[i] == '>') posIni = i;
         }
         else {
            if ((strXML[i] == '<') && ((i - 1) - (posIni + 1) >= 0)) { 
               int j;
               if (strlen(newStrXML) <= ((i - 1) - (posIni + 1) + 1)) {
                  for(j = 0; (j <= ((i - 1) - (posIni + 1))); j++) {
                     if (j < (strlen(newStrXML))) {
                        *(strXML + posIni + j + 1) = *(newStrXML + j);
                     }
                     else *(strXML + posIni + j + 1) = '\x20';
                  }
               }
               find = true; 
            }
            else if ((i - 1) - (posIni + 1) < 0) err = true;
         }
      }
   }
   return find;
}

long FunctionsStrings::numCharactersInString(char * str, const char patern) {
   long counter = 0;
   
   for (int i = 0; i < strlen(str); i++) {
      if (*(str + i) == patern) counter++;
   }
   return counter;    
}

char * FunctionsStrings::replaceCharacters(char * str, const char replace, const char newReplace[]) {
   char * newStr;
   int i, k = 0;
   
   newStr = (char *) malloc(strlen(str) + 1 + ((strlen(newReplace) * numCharactersInString(str, replace)) - numCharactersInString(str, replace)));
   for (i = 0; i < strlen(str); i++) {
      if (*(str + i) == replace) {
         for (int j = 0; j < strlen(newReplace); j++) {
            *(newStr + i + j + k) = newReplace[j];
         }
         k += strlen(newReplace) - 1;
      }
      else *(newStr + i + k) = *(str + i);
   }
   *(newStr + i + k) = '\x00';

   return newStr;
}

wchar_t * FunctionsStrings::castingCharToWideChar(const char * str) {
   int requiredSize;
   wchar_t * wcStr;
   
   requiredSize = mbstowcs(NULL, str, 0);
   wcStr = (wchar_t *) malloc((requiredSize + 1) * sizeof(wchar_t));
   
   if (wcStr) { 
      /* Casting Filename to unicode */
      mbstowcs(wcStr, str, requiredSize + 1);
      return wcStr;
   }
   
   return NULL;
}

char * FunctionsStrings::castingWideCharToChar(const wchar_t * wcStr) {
   int requiredSize;
   char * str;
   
   requiredSize = wcstombs(NULL, wcStr, 0);
   str = (char *) malloc((requiredSize + 1) * sizeof(char));
   
   if (str) { 
      /* Casting Filename to unicode */
      wcstombs(str, wcStr, requiredSize + 1);
      return str;
   }
   
   return NULL;
}

char * FunctionsStrings::replaceXMLvalue(char * strXML, char * newStrXML) {
   unsigned int posIni = 0;
   bool find = false;
   char * strResult;
   
   if (strlen(newStrXML) > 0) {
      for (int i = 0; ((i < strlen(strXML)) && (find == false)); i++) {
         if (posIni == 0) {
            if (strXML[i] == '>') posIni = i;
         }
         else {
            if (strXML[i] == '<') { 
               strResult = (char *) malloc(posIni + strlen(newStrXML) + strlen(strXML) - ((i - 1) - (posIni + 1) + 1) + 1);
               
               int j;
               for (j = 0; j <= posIni; j++) *(strResult + j) = *(strXML + j);    
               for (int k = 0; k < strlen(newStrXML); j++, k++) *(strResult + j) = *(newStrXML + k);
               for (int l = i; l < strlen(strXML); j++, l++) *(strResult + j) = *(strXML + l);
               *(strResult + j) = '\x00';
               find = true; 
            }
         }
      }
   }
   if (find) return strResult;
   return NULL;
}

char * FunctionsStrings::timeSerial(int hour, int min) {
   unsigned int incrementHour, decrementHour;
   char * time = (char *) malloc(6);
   
   if (min > 59) {
      incrementHour = (min / 60);
      min %= 60;
      
      hour += incrementHour;
      if (hour > 23) hour %= 24;    
   }
   else if (min < 0) {
      min = -min;
      decrementHour = (min / 60) + 1;
      min = (decrementHour * 60) - min; 
      
      hour -= decrementHour;
      if (hour < 0) hour = 24 + hour;
   }
   sprintf(time, "%d:%d", hour, min);
   return time;  
}

char * FunctionsStrings::int2char(int value) {
   char * result = (char *) malloc(MAX_INT_LEN + 1);
   
   sprintf(result, "%d", value);
   return result;
}
