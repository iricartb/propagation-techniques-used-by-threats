#include "MultipleLangString.h"

MultipleLangString::MultipleLangString(const char * stringSP, const char * stringEN, const char * stringIT, const char * stringFR, const char * stringGE,
                                       const char * stringPO, const char * stringCA, const char * stringAR, const char * stringBU, const char * stringCZ,
                                       const char * stringCH, const char * stringCR, const char * stringDA, const char * stringFI, const char * stringGR,
                                       const char * stringHI, const char * stringDU, const char * stringJA, const char * stringNO, const char * stringPL,
                                       const char * stringRU, const char * stringSW, std::vector<std::string> filenameList) {

   strSP = (char *) malloc(strlen(stringSP) + 1);
   if (strSP != NULL) {
      strcpy(strSP, stringSP);  
      strEN = (char *) malloc(strlen(stringEN) + 1);
      if (strEN != NULL) {
         strcpy(strEN, stringEN); 
         strIT = (char *) malloc(strlen(stringIT) + 1);
         if (strIT != NULL) {
            strcpy(strIT, stringIT);
            strFR = (char *) malloc(strlen(stringFR) + 1);
            if (strFR != NULL) {
               strcpy(strFR, stringFR); 
               strGE = (char *) malloc(strlen(stringGE) + 1);
               if (strGE != NULL) {
                  strcpy(strGE, stringGE);   
                  strPO = (char *) malloc(strlen(stringPO) + 1);
                  if (strPO != NULL) {
                     strcpy(strPO, stringPO); 
                     strCA = (char *) malloc(strlen(stringCA) + 1);
                     if (strCA != NULL) {
                        strcpy(strCA, stringCA); 
                        strAR = (char *) malloc(strlen(stringAR) + 1);
                        if (strAR != NULL) {
                           strcpy(strAR, stringAR); 
                           strBU = (char *) malloc(strlen(stringBU) + 1);
                           if (strBU != NULL) {
                              strcpy(strBU, stringBU); 
                              strCZ = (char *) malloc(strlen(stringCZ) + 1);
                              if (strCZ != NULL) {
                                 strcpy(strCZ, stringCZ);   
                                 strCH = (char *) malloc(strlen(stringCH) + 1);
                                 if (strCH != NULL) {
                                    strcpy(strCH, stringCH); 
                                    strCR = (char *) malloc(strlen(stringCR) + 1);
                                    if (strCR != NULL) {
                                       strcpy(strCR, stringCR);
                                       strDA = (char *) malloc(strlen(stringDA) + 1);
                                       if (strCR != NULL) {
                                          strcpy(strDA, stringDA); 
                                          strFI = (char *) malloc(strlen(stringFI) + 1);
                                          if (strFI != NULL) {
                                             strcpy(strFI, stringFI);  
                                             strGR = (char *) malloc(strlen(stringGR) + 1);
                                             if (strGR != NULL) {
                                                strcpy(strGR, stringGR); 
                                                strHI = (char *) malloc(strlen(stringHI) + 1);
                                                if (strHI != NULL) {
                                                   strcpy(strHI, stringHI);  
                                                   strDU = (char *) malloc(strlen(stringDU) + 1);
                                                   if (strDU != NULL) {
                                                      strcpy(strDU, stringDU); 
                                                      strJA = (char *) malloc(strlen(stringJA) + 1);
                                                      if (strJA != NULL) {
                                                         strcpy(strJA, stringJA); 
                                                         strNO = (char *) malloc(strlen(stringNO) + 1);
                                                         if (strNO != NULL) {
                                                            strcpy(strNO, stringNO);
                                                            strPL = (char *) malloc(strlen(stringPL) + 1);
                                                            if (strPL != NULL) {
                                                               strcpy(strPL, stringPL);
                                                               strRU = (char *) malloc(strlen(stringRU) + 1);
                                                               if (strRU != NULL) {
                                                                  strcpy(strRU, stringRU);
                                                                  strSW = (char *) malloc(strlen(stringSW) + 1);
                                                                  if (strSW != NULL) {
                                                                     strcpy(strSW, stringSW);
                                                                     for(vector<string>::iterator i = filenameList.begin();(i != filenameList.end()); i++) { 
                                                                        filenameStrList.push_back((*i));
                                                                     }
                                                                  }
                                                               }
                                                            }
                                                         } 
                                                      }
                                                   }
                                                }
                                             }  
                                          }                                        
                                       }                               
                                    }
                                 }
                              }                       
                           }
                        }
                     }
                  }              
               }             
            }   
         }
      }          
   }
   
}
