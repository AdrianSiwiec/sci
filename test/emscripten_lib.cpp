#include <stdio.h>
#include <emscripten.h>
#include "commons.h"

extern "C" {
EMSCRIPTEN_KEEPALIVE
void sayHi(char * name) {
  cout<<"Hello there!"<<endl;
  cout<<"General "<<name<<endl;
}

EMSCRIPTEN_KEEPALIVE
int daysInWeek() {
  return 7;
}
}