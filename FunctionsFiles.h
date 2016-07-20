#include <sys/stat.h>
#include <windows.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <iterator>
#include <list>
#include <ole2.h>
#include "FunctionsAssistant.h"
#include "FunctionsStrings.h"

#define BUFFER_READ_LINE                          500 
#define BINARY_TEXT_FILE_MODIFICABLE_POSITION     78
#pragma once

using namespace std;
   
class FunctionsFiles {
   public:
      static bool fileExists(const char * pathFilename);
      static bool removeFile(const char * pathFilename);
      static bool copyFile(const char * sourcePathFilename, const char * destinyPathFilename, bool overwrite = false);
      static bool copyFile(const char * sourcePathFilename, std::string destinyPathFilename, bool overwrite = false);
      static bool copyFileMultipleName(const char * sourcePathFilename, const char * destinyPath, std::list<std::string> filename);
      static bool appendTextToFile(const char * pathFilename, const char * text);
      static bool appendTextToFileAndErasePatern(const char * pathFilename, const char * text, const char * newText);
      static bool putTextToFile(const char * pathFilename, const char * text);
      static bool changeInformationFile(const char * pathFilename, const char * TitleName, const char * Author);
      static bool changeContentFile(const char * pathFilename, unsigned int base = 0);
      static long fileSize(const char * szFileName);
      static bool setFileAttributesSHR(const char * path);
      static bool setFileAttributesSH(const char * path);
      static bool setFileAttributesR(const char * path);
};
