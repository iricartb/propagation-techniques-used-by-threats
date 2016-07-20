#include "MultipleLangStringList.h"

MultipleLangStringList::MultipleLangStringList() {
   mLangStrList = (MultipleLangString **) malloc(sizeof(MultipleLangString *));
   numElems = 0;
}

bool MultipleLangStringList::addMultipleLangString(const char * strSP, const char * strEN, const char * strIT, const char * strFR, const char * strGE,
                                                   const char * strPO, const char * strCA, const char * strAR, const char * strBU, const char * strCZ,
                                                   const char * strCH, const char * strCR, const char * strDA, const char * strFI, const char * strGR,
                                                   const char * strHI, const char * strDU, const char * strJA, const char * strNO, const char * strPL,
                                                   const char * strRU, const char * strSW, std::vector<std::string> filenameList) {
   bool err = false;
   
   if (numElems > 0) {
      mLangStrList = (MultipleLangString **) realloc(mLangStrList, sizeof(MultipleLangString *) * (numElems + 1));  
      if (mLangStrList == NULL) err = true;
   }
   if (!err) {
      *(mLangStrList + numElems) = (MultipleLangString *) malloc(sizeof(MultipleLangString));
      if (*(mLangStrList + numElems) != NULL) {
         *(mLangStrList + numElems) = new MultipleLangString(strSP, strEN, strIT, strFR, strGE, strPO, strCA, strAR, strBU, strCZ, strCH, strCR, strDA, strFI, strGR, strHI, strDU, strJA, strNO, strPL, strRU, strSW, filenameList);
         numElems++;
         return true;
      }
   }
   return false;  
}
