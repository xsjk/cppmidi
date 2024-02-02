#include "Piano.hpp"

int main() {

  DISPLAY::init();
  
  Piano p;
  
  Played<Displayed<Track>> t[4] = {
    "卡农1.txt", 
    "卡农2.txt", 
    "卡农3.txt", 
    "卡农4.txt"
  };

  p.mainloop();

  return 0;

}

