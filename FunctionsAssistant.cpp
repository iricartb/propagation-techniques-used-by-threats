#include "FunctionsAssistant.h"

unsigned int FunctionsAssistant::number = 0;

int FunctionsAssistant::randomNumber(int min, int max) {
   int randNumber = -1;
   
   if (min < max) {     
      randNumber = (rand() % ((max + 1) - min)) + min;
   }
   else if (min == max) randNumber = min;
   return randNumber;
}

unsigned int FunctionsAssistant::getNumber() {
   FunctionsAssistant::number++;
   FunctionsAssistant::number %= 65535;
   
   return FunctionsAssistant::number;
}

void FunctionsAssistant::setNumber(unsigned int value) {
   FunctionsAssistant::number = value;
}
