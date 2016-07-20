#include "P2PBitComet.h"

P2PBitComet::P2PBitComet(const char * rootFolderPath, const char * incomingFolderPath) : P2P(rootFolderPath, incomingFolderPath) { }

bool P2PBitComet::scanListeningPort() { 
   FILE * stream;
   string configFile;
   char line[BUFFER_READ_LINE];
   char port[BUFFER_PORT_LEN];
   char wport[BUFFER_PORT_LEN];
   bool find = false;
   fpos_t pos;

   configFile = string(rootFolderFullPath) + string("BitComet.xml");
   if (FunctionsFiles::fileExists(configFile.c_str())) {
      stream = fopen(configFile.c_str(), "r+");
    
      if (stream != NULL) {

         while ((fgets(line, BUFFER_READ_LINE, stream) != NULL) && (find == false)) {
            if (strstr(line, "ListenPort") != NULL) { 
               if (FunctionsStrings::getXMLvalue(line, port, BUFFER_PORT_LEN)) {
                  listeningPort = (unsigned int) atol(port);
            
                  sprintf(wport, "%d", (listeningPort - 1));
                  fseek(stream, 0, SEEK_CUR);    
                  fgetpos(stream, &pos); pos = pos - strlen(line) - 1; fsetpos(stream, &pos);          
         
                  FunctionsStrings::setXMLvalue(line, wport);
                  fprintf(stream, "%s", line);
                  
                  fflush(stream);
                  find = true;
               }
            }
         }             
         fclose(stream);
      }      
   }
   return find;
}
