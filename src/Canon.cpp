#include "Piano.hpp"

int main() {

  DISPLAY::init();
  
  Piano p;
  
  Played<Displayed<Track>> t[4] = {
    "��ũ1.txt", 
    "��ũ2.txt", 
    "��ũ3.txt", 
    "��ũ4.txt"
  };

  p.mainloop();

  return 0;

}

