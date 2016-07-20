#include <stdlib.h>
#include <time.h>

#pragma once

using namespace std;
   
class FunctionsAssistant {
   private:
      static unsigned int number;
      
   public:
      static int randomNumber(int min, int max);
      static unsigned int getNumber();
      static void setNumber(unsigned int value);
};
