#include "Piano.hpp"

int main() {

  DISPLAY::init();
  
  Piano p;
  
  KEYBOARD::wait_key(VK_ESCAPE);

  return 0;
  
}

