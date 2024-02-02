#ifndef __MIDI_HPP
#define __MIDI_HPP

#include <Windows.h>
#include <stdexcept>
#include <RtMidi.h>
#include <memory>

class Midi {
  union MIDIMSG {
    BYTE bytes[3];
    UINT data;
  };
  static std::unique_ptr<RtMidiOut> handle;
public:
  Midi() {
    if (!handle)
      try {
        handle = std::make_unique<RtMidiOut>(RtMidi::UNSPECIFIED);
        handle->openPort(0);
      }
      catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
      }
  }
  template <typename ...T>
  static void output(T&& ...arg) {
    uint8_t bytes[] = { BYTE(std::forward<T>(arg))... };
    handle->sendMessage(bytes, sizeof(bytes));
  }
  template <typename ...T>
  void operator()(T&& ...arg) {
    return output(std::forward<T>(arg)...);
  }
};
std::unique_ptr<RtMidiOut> Midi::handle = nullptr;


#include "Sound.hpp"

class MIDI : Midi {
public:
  static void begin_note(Channel c, Pitch p, Volume v) { output(0x90 | c, p, v); }
  static void end_note(Channel c, Pitch p) { output(0x80 | c, p); }
  static void set_instrument(Channel c, Instrument i) { output(0xC0 | c, i); }
  static void set_volume(Channel c, Volume v) { output(0xB0 | c, 0x07, v); }
  static void mute(Channel c) { output(0xB0 | c, 0x7B); }
  MIDI &operator<<(Sound s) { begin_note(s.channel, s.pitch, s.volume); return *this; }
  MIDI &operator>>(Sound s) { end_note(s.channel, s.pitch); return *this; }
};

#endif // __MIDI_HPP