#include "Decorator.hpp"
#include "Keyboard.hpp"

int main() {
  
  using SOUND = Played<Sound>;
  // using SOUND = Displayed<Sound>;
  // using SOUND = Displayed<Played<Sound>>;
  
  // sound "la" lasts for 1 second.
  {
    SOUND s{"A4"}; 
    Sleep(1000);
  }


  using NOTE = Played<Note>;
  // using NOTE = Displayed<Note>;
  // using NOTE = Displayed<Played<Note>>;

  // note "do" lasts for 0.5 second.
  NOTE n{500u,"C4"}; 
  Sleep(1000);

  

  using TRACK = Played<Track>;
  // using TRACK = Displayed<Track>;
  // using TRACK = Displayed<Played<Track>>;

  TRACK t{"¡Ω÷ª¿œª¢.txt"};

  KEYBOARD().mainloop();
  
  return 0;
}