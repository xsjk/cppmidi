#include "Piano.hpp"

int main() {

  DISPLAY::init();
  
  Piano p;

  using SOUND = Played<Displayed<Sound>>;
  using NOTE = Played<Displayed<Note>>;
  using TRACK = Played<Displayed<Track>>;
   
  
  Track t11{
    Note(130,"D4","p",1),
    Note(130,"bB4","p",1),
    Note(130,"G3","p",1),
    Note(130,"G4","p",1),
  }, t12{
    Note(130,"D4","p",1),
    Note(130,"bB4","p",1),
    Note(130,"A4","p",1),
    Note(130,"A5","p",1),
  }, t13{
    Note(130,"D4","p",1),
    Note(130,"C4","p",1),
    Note(130,"A4","p",1),
    Note(130,"A5","p",1),
  }, t14{
    Note(130,"D4","p",1),
    Note(130,"C4","p",1),
    Note(130,"A4","p",1),
    Note(130,"G4","p",1),  
  }, t15{
    Note(130,"#F4","p",1),
    Note(130,"C4","p",1),
    Note(130,"A4","p",1),
    Note(130,"A5","p",1),
  },
  
    t21{
    Note(260,"G3","p",2),
    Note(260),
    Note(260,"bB5","p",2),
    Note(260),
  }, t22{
    Note(260,"F3","p",2),
    Note(260),
    Note(260,"bB5","p",2),
    Note(260),
  }, t23{
    Note(260,"bE3","p",2),
    Note(260),
    Note(260,"bB5","p",2),
    Note(260),
  }, t24{
    Note(260,"D3","p",2),
    Note(260),
    Note(390,"C5","mp",2),
    Note(130,"bB5","mp",2),
    Note(260,"A5","mp",2),
    Note(260),
    Note(390,"D4","mp",2),
    Note(130,"#F4","mp",2),
  };

  



  TRACK t1 = (t11*4+t12*4+t11*4)+(t13+t14*2+t15);
  TRACK t2 = (t21*2+t22*2+t23*2)+(t24);



  p.mainloop();

  return 0;

}

