#include <iostream>

#define MAX_INT_LEN 5

#pragma once

using namespace std;
   
class FunctionsStrings {
   public:
      static const char * getExtension(const char * filename);
      static const char * eraseExtension(char * filename);
      static char * replaceCharacters(char * str, const char replace, const char newReplace[]);
      static char * removeSpecialCharacters(const char * str);
      static char * eraseLastToken(char * str, const char delimiter);
      static char * getToken(char * str, const char delimiter, int index);
      static char * getLastToken(char * str, const char delimiter);
      static char * getFirstToken(char * str, const char delimiter);
      static char * cleanStr(char * str, const char delimiter);
      static long numCharactersInString(char * str, const char patern);
      static char * castingHexToString(char * hexSentence);
      static char castingHexToChar(char * hexSentence);
      static bool getXMLvalue(char * strXML, char * buffer, unsigned int bufferLen);
      static bool setXMLvalue(char * strXML, char * newStrXML);
      static char * replaceXMLvalue(char * strXML, char * newStrXML);
      static wchar_t * castingCharToWideChar(const char * str);
      static char * castingWideCharToChar(const wchar_t * wcStr);
      static char * timeSerial(int hour, int min);
      static char * int2char(int value);
};
