#ifndef __MIDI_HPP
#define __MIDI_HPP
// #pragma comment(lib,"winmm.lib")
#include <Windows.h>
#include <stdexcept>

class Midi {
  union MIDIMSG {
    BYTE bytes[3];
    UINT data;
  };
  static HMIDIOUT handle;
public:
  Midi() {
    if (!handle)
      if(midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL))
        throw std::runtime_error("Could not open MIDI_handle output");
  }
  ~Midi() {
    if (handle) midiOutClose(handle);
  }
  template <typename ...T> 
  static MMRESULT output(T&& ...arg) {
    return midiOutShortMsg(handle,MIDIMSG{ BYTE(arg)... }.data);
  }
  template <typename ...T> 
  MMRESULT operator()(T&& ...arg) {
    return output(std::forward<T>(arg)...);
  }
};
HMIDIOUT Midi::handle = nullptr;


#include "Sound.hpp"

class MIDI : Midi {
public:
  static MMRESULT begin_note(Channel c, Pitch p, Volume v){
    return output(0x90|c,p,v);
  }
  static MMRESULT end_note(Channel c, Pitch p){
    return output(0x80|c,p);
  }
  static MMRESULT set_instrument(Channel c, Instrument i){
    return output(0xC0|c,i);
  }
  static MMRESULT set_volume(Channel c, Volume v){
    return output(0xB0|c,0x07,v);
  }
  static MMRESULT mute(Channel c) {
    return output(0xB0|c,0x7B);
  }
  MIDI& operator<<(Sound s) { 
    return begin_note(s.channel, s.pitch, s.volume), *this;
  }
  MIDI& operator>> (Sound s) { 
    return end_note(s.channel, s.pitch), *this;
  }
};

#endif // __MIDI_HPP