#include "Piano.hpp"

int main() {

  DISPLAY::init();
  
  Piano p;

  using SOUND = Played<Displayed<Sound>>;
  using NOTE = Played<Displayed<Note>>;
  using TRACK = Played<Displayed<Track>>;
   
  
  Track t11{
    Note(120,"D4","p",1),
    Note(120,"bB4","p",1),
    Note(120,"G3","p",1),
    Note(120,"G4","p",1),
  }, t12{
    Note(120,"D4","p",1),
    Note(120,"bB4","p",1),
    Note(120,"A4","p",1),
    Note(120,"A5","p",1),
  }, t13{
    Note(120,"D4","p",1),
    Note(120,"C4","p",1),
    Note(120,"A4","p",1),
    Note(120,"A5","p",1),
  }, t14{
    Note(120,"D4","p",1),
    Note(120,"C4","p",1),
    Note(120,"A4","p",1),
    Note(120,"G4","p",1),  
  }, t15{
    Note(120,"#F4","p",1),
    Note(120,"C4","p",1),
    Note(120,"A4","p",1),
    Note(120,"A5","p",1),
  },
  
    t21{
    Note(240,"G3","p",2),
    Note(240),
    Note(240,"bB5","p",2),
    Note(240),
  }, t22{
    Note(240,"F3","p",2),
    Note(240),
    Note(240,"bB5","p",2),
    Note(240),
  }, t23{
    Note(240,"bE3","p",2),
    Note(240),
    Note(240,"bB5","p",2),
    Note(240),
  }, t24{
    Note(240,"D3","p",2),
    Note(240),
    Note(360,"C5","mp",2),
    Note(120,"bB5","mp",2),
    Note(240,"A5","mp",2),
    Note(240),
    Note(360,"D4","mp",2),
    Note(120,"#F4","mp",2),
  }, t25{
    Note(240),
    Note(120,"bB5","p",2),
  },
    t31{
    Note(960,"G2","p",3),
  };

  



  TRACK t1 = (t11*4+t12*4+t11*4)+(t13+t14*2+t15);
  TRACK t2 = (t21*2+t22*2+t23*2)+(t24);



  KEYBOARD::wait_key(VK_ESCAPE);

  return 0;

}

