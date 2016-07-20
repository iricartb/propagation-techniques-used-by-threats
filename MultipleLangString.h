#include <windows.h>
#include <vector>
#include <iterator>

#pragma once

using namespace std;

class MultipleLangString {
   private:
      char * strSP;
      char * strEN;
      char * strIT;
      char * strFR;
      char * strGE;
      char * strPO;
      char * strCA;
      char * strAR;
      char * strBU;
      char * strCZ;
      char * strCH;
      char * strCR;
      char * strDA;
      char * strFI;
      char * strGR;
      char * strHI;
      char * strDU;
      char * strJA;
      char * strNO;
      char * strPL;
      char * strRU;
      char * strSW;      
      std::vector<std::string> filenameStrList;

   public:
      MultipleLangString(const char * stringSP, const char * stringEN, const char * stringIT, const char * stringFR, const char * stringGE,  
                         const char * stringPO, const char * stringCA, const char * stringAR, const char * stringBU, const char * stringCZ,  
                         const char * stringCH, const char * stringCR, const char * stringDA, const char * stringFI, const char * stringGR,  
                         const char * stringHI, const char * stringDU, const char * stringJA, const char * stringNO, const char * stringPL,  
                         const char * stringRU, const char * stringSW, std::vector<std::string> filenameList);
                         
      char * getStrSP() { return strSP; }
      char * getStrEN() { return strEN; }
      char * getStrIT() { return strIT; }
      char * getStrFR() { return strFR; }
      char * getStrGE() { return strGE; }
      char * getStrPO() { return strPO; }
      char * getStrCA() { return strCA; }
      char * getStrAR() { return strAR; }
      char * getStrBU() { return strBU; }
      char * getStrCZ() { return strCZ; }
      char * getStrCH() { return strCH; }
      char * getStrCR() { return strCR; }
      char * getStrDA() { return strDA; }
      char * getStrFI() { return strFI; }
      char * getStrGR() { return strGR; }
      char * getStrHI() { return strHI; }
      char * getStrDU() { return strDU; }
      char * getStrJA() { return strJA; }
      char * getStrNO() { return strNO; }
      char * getStrPL() { return strPL; }
      char * getStrRU() { return strRU; }
      char * getStrSW() { return strSW; }   
      
      std::vector<std::string> getFilenameList() { return filenameStrList; }
};
