#include <vector>
#include <iterator>
#include "MultipleLangString.h"
#include "Main.h"

#pragma once

class MultipleLangStringList {
   private:
      MultipleLangString ** mLangStrList;
      unsigned int numElems; 
      
   public:
      MultipleLangStringList();   
      
      bool addMultipleLangString(const char * strSP, const char * strEN, const char * strIT, const char * strFR, const char * strGE,
                                 const char * strPO, const char * strCA, const char * strAR, const char * strBU, const char * strCZ,
                                 const char * strCH, const char * strCR, const char * strDA, const char * strFI, const char * strGR,
                                 const char * strHI, const char * strDU, const char * strJA, const char * strNO, const char * strPL,
                                 const char * strRU, const char * strSW, std::vector<std::string> filenameList); 
      
      unsigned int getNumElems() { return numElems; }
      MultipleLangString ** getMultipleLangStringList() { return mLangStrList; }  
};
