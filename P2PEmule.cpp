#include "P2PEmule.h"

P2PEmule::P2PEmule(const char * rootFolderPath, const char * incomingFolderPath) : P2P(rootFolderPath, incomingFolderPath) { }

bool P2PEmule::scanListeningPort() { 
   FILE * stream;
   string configFile;
   char line[BUFFER_READ_LINE];
   char port[BUFFER_PORT_LEN];
   char wport[BUFFER_PORT_LEN];
   bool find = false;
   fpos_t pos;

   configFile = string(rootFolderFullPath) + string("config\\preferences.ini");
   if (FunctionsFiles::fileExists(configFile.c_str())) {
      stream = fopen(configFile.c_str(), "r+");
    
      if (stream != NULL) {

         while ((fgets(line, BUFFER_READ_LINE, stream) != NULL) && (find == false)) { 
            if ((line[0] == 'P') && (line[1] == 'o') && (line[2] == 'r') && (line[3] == 't') && (line[4] == '=')) {
               strcpy(port, (const char *) (line + 5));
               listeningPort = (unsigned int) atol(port);
               sprintf(wport, "%d\n", (listeningPort - 1));
               fseek(stream, 0, SEEK_CUR);    
               fgetpos(stream, &pos); pos = pos - 6 - strlen(port); fsetpos(stream, &pos);          
               if (strlen(port) == strlen(wport)) fprintf(stream, "Port=%d\n", (listeningPort - 1));
               else fprintf(stream, "Port=%d \n", (listeningPort - 1));
               fflush(stream);
               find = true;
            }
         }             
         fclose(stream);
      }      
   }
   return find;
}
