#include "Piano.hpp"
// #include <thread>
#include "Async.hpp"


int main() {

  DISPLAY::init();


  Thread t([]{
    Piano p;
    p.mainloop();
  });
    

  // KEYBOARD::wait_key(VK_ESCAPE);

  // std::set<int,Played<Sound>> s1;

  // system("pause");

  return 0;
  
}

