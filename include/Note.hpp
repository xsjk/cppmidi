#ifndef __NOTE_HPP
#define __NOTE_HPP

#include "Sound.hpp"
#include "print.hpp"


struct Note {
  unsigned int duration;
  Sound sound;
  operator Sound() const { return sound; }
  operator bool() const { return sound.pitch!=0; }
  template<typename ...A>
  Note(unsigned int d=0, A&& ...args) : duration{d}, 
                                        sound{std::forward<A>(args)...} {}
};

#endif