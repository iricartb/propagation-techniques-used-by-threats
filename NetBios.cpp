#include "NetBios.h" 

NetBios::NetBios() { 
   numLocalElems = 0;
   numLocalPrinters = 0;
   numLocalFolders = 0;
   numLocalSpecialFolders = 0;
   
   numRemoteElems = 0;
   numRemotePrinters = 0;
   numRemoteFolders = 0;
   numRemoteSpecialFolders = 0;
}

bool NetBios::netShareDirectoryAdd(const char * directory, const char * shareName, const char * infoShare, const char * password) {   
   SHARE_INFO_2 shareInfo;
   NET_API_STATUS result = ERROR_ACCESS_DENIED;
   bool errPassword = false; 
   wchar_t * wcDirectory;
   wchar_t * wcShareName;
   wchar_t * wcInfoShare;
   wchar_t * wcPassword;
   
   if (FunctionsFiles::fileExists(directory)) {   
      wcShareName = FunctionsStrings::castingCharToWideChar(shareName);
   
      if (wcShareName != NULL) { 
         shareInfo.shi2_netname = (TCHAR *) wcShareName;
         shareInfo.shi2_type = STYPE_DISKTREE;
         
         wcInfoShare = FunctionsStrings::castingCharToWideChar(infoShare);        
            
         if(wcInfoShare != NULL) {
            shareInfo.shi2_remark = (TCHAR *) wcInfoShare;
            shareInfo.shi2_permissions = ACCESS_ALL;
            shareInfo.shi2_max_uses = (DWORD) -1;
            shareInfo.shi2_current_uses = 0;
               
            wcDirectory = FunctionsStrings::castingCharToWideChar(directory);      
      
            if (wcDirectory != NULL) {
               shareInfo.shi2_path = (TCHAR *) wcDirectory;
                  
               if (password != NULL) { 
                  wcPassword = FunctionsStrings::castingCharToWideChar(password);        
         
                  if(wcPassword != NULL) {
                     shareInfo.shi2_passwd = (TCHAR *) wcPassword;
                  }  
                  else errPassword = true;
               }         
               
               if (!errPassword) {
                  result = NetShareAdd(NULL, 2, (LPBYTE) &shareInfo, NULL);
                  if (password != NULL) free(wcPassword);
               }
               
               free(wcDirectory);
            }                    
            free(wcInfoShare);
         }  
         free(wcShareName);
      } 
   }
   if ((result == NERR_Success) || (result == NERR_DuplicateShare)) return true;
   return false;
}

PSHARE_INFO_2 NetBios::netShareEnumeration() {
   DWORD entriesRead;

   resetLocalInfoShare();
   if (NetShareEnum(NULL, 2, (LPBYTE *) &shareLocalInfoList, MAX_PREFERRED_LENGTH, &entriesRead, (DWORD *) &numLocalElems, NULL) == NERR_Success) {
      for (long i = 0; i < numLocalElems; i++) {
         if ((*(shareLocalInfoList + i)).shi2_type == STYPE_DISKTREE) {
            shareLocalInfoFoldersList = (PSHARE_INFO_2 *) realloc(shareLocalInfoFoldersList, ((numLocalFolders * sizeof(PSHARE_INFO_2)) + sizeof(PSHARE_INFO_2)));
            (*(shareLocalInfoFoldersList + numLocalFolders)) = (shareLocalInfoList + i); 
            numLocalFolders++;
         }
         else if ((*(shareLocalInfoList + i)).shi2_type == STYPE_PRINTQ) {
            shareLocalInfoPrintersList = (PSHARE_INFO_2 *) realloc(shareLocalInfoPrintersList, ((numLocalPrinters * sizeof(PSHARE_INFO_2)) + sizeof(PSHARE_INFO_2)));
            (*(shareLocalInfoPrintersList + numLocalPrinters)) = (shareLocalInfoList + i); 
            numLocalPrinters++;        
         }
         else {
            shareLocalInfoSpecialFoldersList = (PSHARE_INFO_2 *) realloc(shareLocalInfoSpecialFoldersList, ((numLocalSpecialFolders * sizeof(PSHARE_INFO_2)) + sizeof(PSHARE_INFO_2)));
            (*(shareLocalInfoSpecialFoldersList + numLocalSpecialFolders)) = (shareLocalInfoList + i); 
            numLocalSpecialFolders++; 
         } 
      }
      return shareLocalInfoList;    
   }
   else return NULL;
}

PSHARE_INFO_1 NetBios::netShareEnumeration(const char * server) {
   NET_API_STATUS res;
   DWORD entriesRead;
   wchar_t * wcServer;
   DWORD size_buffer = INIT_SIZE_BUFFER;
    
   wcServer = FunctionsStrings::castingCharToWideChar(server);
   if (wcServer != NULL) {
      resetRemoteInfoShare();
      
      do {
         res = NetShareEnum(wcServer, 1, (LPBYTE *) &shareRemoteInfoList, size_buffer, &entriesRead, (DWORD *) &numRemoteElems, NULL);
         if ((res == ERROR_MORE_DATA) || (res == NERR_BufTooSmall)) {
            NetApiBufferFree(shareRemoteInfoList);
            size_buffer += INIT_SIZE_BUFFER; 
         }
      }
      while ((res == ERROR_MORE_DATA) || (res == NERR_BufTooSmall));

      free(wcServer);
      if (res == NERR_Success) {
         for (long i = 0; i < numRemoteElems; i++) {
            if ((*(shareRemoteInfoList + i)).shi1_type == STYPE_DISKTREE) {
               shareRemoteInfoFoldersList = (PSHARE_INFO_1 *) realloc(shareRemoteInfoFoldersList, ((numRemoteFolders * sizeof(PSHARE_INFO_1)) + sizeof(PSHARE_INFO_1)));
               (*(shareRemoteInfoFoldersList + numRemoteFolders)) = (shareRemoteInfoList + i); 
               numRemoteFolders++;
            }
            else if ((*(shareRemoteInfoList + i)).shi1_type == STYPE_PRINTQ) {
               shareRemoteInfoPrintersList = (PSHARE_INFO_1 *) realloc(shareRemoteInfoPrintersList, ((numRemotePrinters * sizeof(PSHARE_INFO_1)) + sizeof(PSHARE_INFO_1)));
               (*(shareRemoteInfoPrintersList + numRemotePrinters)) = (shareRemoteInfoList + i); 
               numRemotePrinters++;        
            }
            else {
               shareRemoteInfoSpecialFoldersList = (PSHARE_INFO_1 *) realloc(shareRemoteInfoSpecialFoldersList, ((numRemoteSpecialFolders * sizeof(PSHARE_INFO_1)) + sizeof(PSHARE_INFO_1)));
               (*(shareRemoteInfoSpecialFoldersList + numRemoteSpecialFolders)) = (shareRemoteInfoList + i); 
               numRemoteSpecialFolders++; 
            } 
         }      
         return shareRemoteInfoList;
      }
   }
   return NULL;
}

PSHARE_INFO_2 * NetBios::getShareLocalPrintersList() {
   if (numLocalPrinters > 0) {
      return shareLocalInfoPrintersList;   
   }
   return NULL;
}

PSHARE_INFO_2 * NetBios::getShareLocalFoldersList() {
   if (numLocalFolders > 0) {
      return shareLocalInfoFoldersList;   
   }
   return NULL;
}

PSHARE_INFO_2 * NetBios::getShareLocalSpecialFoldersList() {
   if (numLocalSpecialFolders > 0) {
      return shareLocalInfoSpecialFoldersList;   
   }
   return NULL;
}

PSHARE_INFO_1 * NetBios::getShareRemotePrintersList() {
   if (numRemotePrinters > 0) {
      return shareRemoteInfoPrintersList;   
   }
   return NULL;
}

PSHARE_INFO_1 * NetBios::getShareRemoteFoldersList() {
   if (numRemoteFolders > 0) {
      return shareRemoteInfoFoldersList;   
   }
   return NULL;
}

PSHARE_INFO_1 * NetBios::getShareRemoteSpecialFoldersList() {
   if (numRemoteSpecialFolders > 0) {
      return shareRemoteInfoSpecialFoldersList;   
   }
   return NULL;
}

bool NetBios::netUserAdd(const char * username, const char * passwd, const char * homeDir, const char * infoAccount, const char * fullName, const char * infoUser) {
   USER_INFO_2 userInfo;
   NET_API_STATUS result = ERROR_ACCESS_DENIED;
   bool errHomeDir = false, errFullName = false; 
   wchar_t * wcUsername;
   wchar_t * wcPassword;
   wchar_t * wcHomeDir;
   wchar_t * wcInfoAccount;
   wchar_t * wcFullName;
   wchar_t * wcInfoUser;
   
   wcUsername = FunctionsStrings::castingCharToWideChar(username);
   
   if (wcUsername != NULL) { 
      userInfo.usri2_name = wcUsername;
      
      wcPassword = FunctionsStrings::castingCharToWideChar(passwd);
      
      if (wcPassword != NULL) {
         userInfo.usri2_password = wcPassword;
         userInfo.usri2_priv = USER_PRIV_USER;
         
         if (homeDir == NULL) userInfo.usri2_home_dir = NULL; 
         else {
            wcHomeDir = FunctionsStrings::castingCharToWideChar(homeDir);
           
            if (wcHomeDir != NULL) {
               userInfo.usri2_home_dir = wcHomeDir;     
            }
            else errHomeDir = true;
         }
         
         if (!errHomeDir) {
            wcInfoAccount = FunctionsStrings::castingCharToWideChar(infoAccount);
            if (wcInfoAccount != NULL) {
               userInfo.usri2_comment = wcInfoAccount;    
               userInfo.usri2_flags = UF_SCRIPT | UF_DONT_EXPIRE_PASSWD;
               userInfo.usri2_script_path = NULL;
               userInfo.usri2_auth_flags = 0;
               
               if (fullName == NULL) userInfo.usri2_full_name = NULL;
               else {
                  wcFullName = FunctionsStrings::castingCharToWideChar(infoAccount);
                  
                  if (wcFullName != NULL) {
                     userInfo.usri2_full_name = wcFullName;   
                  }
                  else errFullName = true;
               }
               
               if (!errFullName) {
                  wcInfoUser = FunctionsStrings::castingCharToWideChar(infoUser);     
                  
                  if (wcInfoUser != NULL) {
                     userInfo.usri2_usr_comment = wcInfoUser;
                     userInfo.usri2_parms = NULL;
                     userInfo.usri2_workstations = NULL;
                     userInfo.usri2_acct_expires = TIMEQ_FOREVER;
                     userInfo.usri2_max_storage = USER_MAXSTORAGE_UNLIMITED;
                     userInfo.usri2_logon_hours = NULL;
                     userInfo.usri2_logon_server = NULL;
                     userInfo.usri2_country_code = 0;
                     userInfo.usri2_code_page = 0;
                                  
                     DWORD param_err = 0;
                     result = NetUserAdd(NULL, 2, (LPBYTE) &userInfo, &param_err);
                     free(wcInfoUser);   
                  }                   
                  if (fullName != NULL) free(wcFullName);
               }
               free(wcInfoAccount);
            }
            if (homeDir != NULL) free(wcHomeDir);
         }
         free(wcPassword);  
      }
      free(wcUsername);
   }
   if (result == NERR_Success) return true;
   return false;   
}

bool NetBios::netUserDel(const char * username) {
   NET_API_STATUS result = ERROR_ACCESS_DENIED; 
   wchar_t * wcUsername;
   
   wcUsername = FunctionsStrings::castingCharToWideChar(username);
   
   if (wcUsername != NULL) { 
      result = NetUserDel(NULL, wcUsername);
      free(wcUsername);
   }
   if (result == NERR_Success) return true;
   return false;
}

bool NetBios::netLocalGroupAdministratorsAddUser(const char * username) {
   char * adminGroupName;
   bool result = false;
   
   adminGroupName = getAdministratorsGroupName();
   if (adminGroupName != NULL) {
      result = netLocalGroupAddUser(username, adminGroupName);
      free(adminGroupName);
   }
   return result;
}

bool NetBios::netLocalGroupAddUser(const char * username, const char * groupName) {
   LOCALGROUP_MEMBERS_INFO_3 membersInfo;
   NET_API_STATUS result = ERROR_ACCESS_DENIED; 
   wchar_t * wcUsername;
   wchar_t * wcGroupName;
   wchar_t * wcDomainAndName;
   char * computerName;
   
   wcUsername = FunctionsStrings::castingCharToWideChar(username);
   
   if (wcUsername != NULL) {
      
      wcGroupName = FunctionsStrings::castingCharToWideChar(groupName);
      if (wcGroupName != NULL) {
         
         computerName = FunctionsWindowsNT::getComputerName();
         if (computerName != NULL) {
            wcDomainAndName = FunctionsStrings::castingCharToWideChar((string(computerName) + string("\\") + string(string(username))).c_str());
            if (wcDomainAndName != NULL) {
               membersInfo.lgrmi3_domainandname = wcDomainAndName;
               result = NetLocalGroupAddMembers(NULL, wcGroupName, 3, (LPBYTE) &membersInfo, 1);   
               free(wcDomainAndName);
            }
            free(computerName);              
         }
         free(wcGroupName);
      }
      free(wcUsername);
   } 
   if (result == NERR_Success) return true;
   return false;
}

void NetBios::resetLocalInfoShare() {
   if (shareLocalInfoList != NULL) NetApiBufferFree(shareLocalInfoList);
   if (shareLocalInfoPrintersList != NULL) free(shareLocalInfoPrintersList);
   if (shareLocalInfoFoldersList != NULL) free(shareLocalInfoFoldersList);
   if (shareLocalInfoSpecialFoldersList != NULL) free(shareLocalInfoSpecialFoldersList);
   
   shareLocalInfoPrintersList = (PSHARE_INFO_2 *) malloc(0);
   shareLocalInfoFoldersList = (PSHARE_INFO_2 *) malloc(0);
   shareLocalInfoSpecialFoldersList = (PSHARE_INFO_2 *) malloc(0);
   numLocalPrinters = 0;
   numLocalFolders = 0;
   numLocalSpecialFolders = 0;
}

void NetBios::resetRemoteInfoShare() {
     
   if (shareRemoteInfoList != NULL) NetApiBufferFree(shareRemoteInfoList);
   if (shareRemoteInfoPrintersList != NULL) free(shareRemoteInfoPrintersList);
   if (shareRemoteInfoFoldersList != NULL)free(shareRemoteInfoFoldersList);
   if (shareRemoteInfoSpecialFoldersList != NULL) free(shareRemoteInfoSpecialFoldersList);
   
   shareRemoteInfoPrintersList = (PSHARE_INFO_1 *) malloc(0);
   shareRemoteInfoFoldersList = (PSHARE_INFO_1 *) malloc(0);
   shareRemoteInfoSpecialFoldersList = (PSHARE_INFO_1 *) malloc(0);
   numRemotePrinters = 0;
   numRemoteFolders = 0;
   numRemoteSpecialFolders = 0;
}

char * NetBios::getAdministratorsGroupName() {
   PLOCALGROUP_INFO_0 localGroupInfoList;
   DWORD totalEntries, currentEntry;
   char * groupName, * groupSID, * result;
   bool find = false;
   
   if (NetLocalGroupEnum(NULL, 0, (LPBYTE *) &localGroupInfoList, MAX_PREFERRED_LENGTH, &currentEntry, &totalEntries, NULL) == NERR_Success) {
      for (long i = 0; ((i < totalEntries) && (find == false)); i++) {
         groupName = FunctionsStrings::castingWideCharToChar((*(localGroupInfoList + i)).lgrpi0_name);
         if (groupName != NULL) {
            groupSID = FunctionsWindowsNT::groupToSID(groupName);
            if (groupSID != NULL) {
               if (atoi(FunctionsStrings::getLastToken(groupSID, '-')) == GROUP_ADMINISTRATORS_SID) {
                  result = (char *) malloc(strlen(groupName) + 1);
                  strcpy(result, groupName);
                  find = true;
               }
               free(groupSID);
            }
            free(groupName);
         }       
      }
      NetApiBufferFree(localGroupInfoList);
      
      if (find) return result;
   }
   return NULL;
}

bool NetBios::netSendMessage(const char * sender, const char * destiny, const char * messageText) {
   NET_API_STATUS result = ERROR_ACCESS_DENIED; 
   wchar_t * wcSender;
   wchar_t * wcDestiny;
   wchar_t * wcMessageText;

   wcDestiny = FunctionsStrings::castingCharToWideChar(destiny);
   
   if (wcDestiny != NULL) {
      wcMessageText = FunctionsStrings::castingCharToWideChar(messageText);
      
      if (wcMessageText != NULL) {
         if (sender == NULL) {                
            result = NetMessageBufferSend(NULL, wcDestiny, NULL, (BYTE *) wcMessageText, strlen(messageText) * sizeof(WCHAR)); 
         }
         else {
            wcSender = FunctionsStrings::castingCharToWideChar(sender);
            
            if (wcSender != NULL) {
               if (NetMessageNameAdd(NULL, wcSender) == NERR_Success) {
                  result = NetMessageBufferSend(NULL, wcDestiny, wcSender, (BYTE *) wcMessageText, strlen(messageText) * sizeof(WCHAR));
                  NetMessageNameDel(NULL, wcSender);
               }
               free(wcSender);
            }
         }                    
         free(wcMessageText);
      }         
      free(wcDestiny);
   }                     
   if (result == NERR_Success) return true;
   return false;
}

bool NetBios::netSchedulerTaskAdd(const char * time, int daysWeek, const char * command, bool interactive) {
   return netSchedulerRemoteTaskAdd(NULL, time, daysWeek, command, interactive);   
}

bool NetBios::netSchedulerRemoteTaskAdd(const char * server, const char * time, int daysWeek, const char * command, bool interactive) {
   AT_INFO infoAT;
   NET_API_STATUS result = ERROR_ACCESS_DENIED;
   DWORD IDJob;
   int htime, mtime;
   int counter;
   bool err = true;
   wchar_t * wcCommand;
   wchar_t * wcServer;
   
   counter = FunctionsStrings::numCharactersInString((char *) time, ':');
   if (counter == 1) {
      char * token = FunctionsStrings::getFirstToken((char *) time, ':');
      htime = atoi(token);
      free(token);
      if ((htime >= 0) && (htime <= 23)) {
         mtime = atoi(FunctionsStrings::getLastToken((char *) time, ':'));
         if ((mtime >= 0) && (mtime <= 59)) {
            infoAT.JobTime = (((htime * 3600) + (mtime * 60)) * 1000);
            err = false;
         }   
      }      
   }
   else if (counter == 0) {
      htime = atoi(time);
      if ((htime >= 0) && (htime <= 23)) {
         infoAT.JobTime = ((htime * 3600) * 1000);
         err = false;
      }                  
   }        
   
   if (!err) {         
      if (daysWeek <= 127) {
         infoAT.DaysOfMonth = 0;
         infoAT.DaysOfWeek = (UCHAR) daysWeek;
            
         if (daysWeek == TODAY) infoAT.Flags = JOB_ADD_CURRENT_DATE;
         else infoAT.Flags = JOB_RUN_PERIODICALLY;
         if (!interactive) infoAT.Flags |= JOB_NONINTERACTIVE;
         
         wcCommand = FunctionsStrings::castingCharToWideChar(command); 
         if (wcCommand != NULL) {
            infoAT.Command = wcCommand;
            
            if (server == NULL) result = NetScheduleJobAdd(NULL, (BYTE *) &infoAT, &IDJob); 
            else {
               wcServer = FunctionsStrings::castingCharToWideChar((string("\\\\") + string(server)).c_str()); 
               if (wcServer != NULL) {    
                  result = NetScheduleJobAdd(wcServer, (BYTE *) &infoAT, &IDJob);                               
               }
            }
            free(wcCommand);
            if (result == NERR_Success) return true;
         }
      }
   }
   return false;
}

bool NetBios::netSchedulerTaskAdd(const char * time, char * daysMonth, const char * command, bool interactive) {
   return netSchedulerRemoteTaskAdd(NULL, time, daysMonth, command, interactive);
}

bool NetBios::netSchedulerRemoteTaskAdd(const char * server, const char * time, char * daysMonth, const char * command, bool interactive) {
   AT_INFO infoAT;
   NET_API_STATUS result = ERROR_ACCESS_DENIED;
   DWORD IDJob;
   int htime, mtime;
   int counter, numElems, numDay;
   bool err = true;
   wchar_t * wcCommand;
   wchar_t * wcServer;
   
   counter = FunctionsStrings::numCharactersInString((char *) time, ':');
   if (counter == 1) {
      char * token = FunctionsStrings::getFirstToken((char *) time, ':');
      htime = atoi(token);
      free(token);
      if ((htime >= 0) && (htime <= 23)) {
         mtime = atoi(FunctionsStrings::getLastToken((char *) time, ':'));
         if ((mtime >= 0) && (mtime <= 59)) {
            infoAT.JobTime = (((htime * 3600) + (mtime * 60)) * 1000);
            err = false;
         }   
      }      
   }
   else if (counter == 0) {
      htime = atoi(time);
      if ((htime >= 0) && (htime <= 23)) {
         infoAT.JobTime = ((htime * 3600) * 1000);
         err = false;
      }                  
   }        
   
   if (!err) {         
      infoAT.DaysOfWeek = 0;
      infoAT.DaysOfMonth = 0;
      
      numElems = FunctionsStrings::numCharactersInString(daysMonth, ',') + 1;
      for(int i = 0; i < numElems; i++) {
         char * token = FunctionsStrings::getToken(daysMonth, ',', (i + 1));
         numDay = atoi(token);
         if ((numDay >= 1) && (numDay <= 31)) infoAT.DaysOfMonth += ((int) pow(2, (numDay - 1))); 
         free(token);
      }
            
      if (infoAT.DaysOfMonth == TODAY) infoAT.Flags = JOB_ADD_CURRENT_DATE;
      else infoAT.Flags = JOB_RUN_PERIODICALLY;
      if (!interactive) infoAT.Flags |= JOB_NONINTERACTIVE;
         
      wcCommand = FunctionsStrings::castingCharToWideChar(command); 
      if (wcCommand != NULL) {
         infoAT.Command = wcCommand;
            
         if (server == NULL) result = NetScheduleJobAdd(NULL, (BYTE *) &infoAT, &IDJob); 
         else {
            wcServer = FunctionsStrings::castingCharToWideChar((string("\\\\") + string(server)).c_str()); 
            if (wcServer != NULL) {    
               result = NetScheduleJobAdd(wcServer, (BYTE *) &infoAT, &IDJob);                               
            }
         }
         free(wcCommand);
         if (result == NERR_Success) return true;
      }
   }
   return false;
}

bool NetBios::netStartService(const char * serviceName) {
   SC_HANDLE SCHandle, servHandle;
   BOOL result = 0;
   
   SCHandle = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_CONNECT);
   if (SCHandle != NULL) {
      servHandle = OpenService(SCHandle, serviceName, SERVICE_ALL_ACCESS);
      if (servHandle != NULL) {
         result = StartService(servHandle, 0, NULL); 
         CloseServiceHandle(servHandle);
      }
      CloseServiceHandle(SCHandle);
   }
   return result;
}

bool NetBios::netStopService(const char * serviceName) {
   SERVICE_STATUS statusService;
   SC_HANDLE SCHandle, servHandle;
   BOOL result = 0;
   
   SCHandle = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_CONNECT);
   if (SCHandle != NULL) {
      servHandle = OpenService(SCHandle, serviceName, SERVICE_ALL_ACCESS);
      if (servHandle != NULL) {
         result = ControlService(servHandle, SERVICE_CONTROL_STOP, &statusService);
         CloseServiceHandle(servHandle);
      }
      CloseServiceHandle(SCHandle);
   }
   return result;
}

char * NetBios::netTime() {
   return netTime(NULL);
}

char * NetBios::netTime(const char * server) {
   TIME_OF_DAY_INFO * timeInfo;
   NET_API_STATUS result = ERROR_ACCESS_DENIED;
   wchar_t * wcServer;
   char * time;
   
   if (server == NULL) result = NetRemoteTOD(NULL, (BYTE **) &timeInfo); 
   else {
      wcServer = FunctionsStrings::castingCharToWideChar((string("\\\\") + string(server)).c_str()); 
      if (wcServer != NULL) {    
         result = NetRemoteTOD(wcServer, (BYTE **) &timeInfo);                               
      }
   }
   if (result == NERR_Success) {
      time = FunctionsStrings::timeSerial((*(timeInfo)).tod_hours, (*(timeInfo)).tod_mins - (*(timeInfo)).tod_timezone);
      NetApiBufferFree(timeInfo);
      return time;     
   }
   
   return NULL;
}

char * NetBios::netUseDirectoryAdd(const char * server, const char * shareName, const char * diskLetter, const char * password) {
   USE_INFO_1 useInfo;
   NET_API_STATUS result = ERROR_ACCESS_DENIED;
   wchar_t * wcDiskLetter;
   wchar_t * wcPassword;
   wchar_t * wcServerAndShareName;
   char letter;
   char * resultLetter;
   bool errLetter = true, errPassword = false;
   
   useInfo.ui1_asg_type = USE_DISKDEV;
   
   if (diskLetter != BEST_EFFORT) {
      resultLetter = (char *) malloc(3);
      letter = FunctionsWindowsNT::getFirstFreeDrive();
      
      if (letter != 0) {
         *(resultLetter) = letter;
         *(resultLetter + 1) = ':';
         *(resultLetter + 2) = '\x00';
         wcDiskLetter = FunctionsStrings::castingCharToWideChar(resultLetter);
         errLetter = false;
      }   
   }
   else { resultLetter = (char *) malloc(strlen(diskLetter) + 1); strcpy(resultLetter, diskLetter); wcDiskLetter = FunctionsStrings::castingCharToWideChar(diskLetter); errLetter = false; }
   
   if ((wcDiskLetter != NULL) && (!errLetter)) {
      useInfo.ui1_local = wcDiskLetter; 
      
      if (server == NULL) { 
         wcServerAndShareName = FunctionsStrings::castingCharToWideChar((string("\\\\127.0.0.1\\") + string(shareName)).c_str());   
      }
      else { wcServerAndShareName = FunctionsStrings::castingCharToWideChar((string("\\\\") + string(server) + string("\\") + string(shareName)).c_str()); }
      
      if (wcServerAndShareName != NULL) {
         useInfo.ui1_remote = wcServerAndShareName;
         
         if (password != NULL) {
            wcPassword = FunctionsStrings::castingCharToWideChar(password);
            if (wcPassword != NULL) useInfo.ui1_password = wcPassword;
            else errPassword = true;
         }
         else useInfo.ui1_password = L"";
         
         if (!errPassword) {
            result = NetUseAdd(NULL, 1, (BYTE *) &useInfo, NULL);   
          
            if (password != NULL) free(wcPassword);
         }
         free(wcServerAndShareName);
      }                     
      free(wcDiskLetter);  
   }
   
   if (result == NERR_Success) return resultLetter;
   free(resultLetter);
  
   return NULL;
}

bool NetBios::netUseDirectoryDel(const char * diskLetter) {
   NET_API_STATUS result = ERROR_ACCESS_DENIED;
   wchar_t * wcDiskLetter;
   
   wcDiskLetter = FunctionsStrings::castingCharToWideChar(diskLetter);
   if (wcDiskLetter != NULL) {
      result = NetUseDel(NULL, wcDiskLetter, USE_NOFORCE);
      free(wcDiskLetter);
   }
   if (result == NERR_Success) return true;
   return false;
}
