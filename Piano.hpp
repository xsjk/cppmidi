#ifndef __PIANO_HPP
#define __PIANO_HPP

#include <map>
#include "Keyboard.hpp"
#include "Decorator.hpp"

class Piano : public KEYBOARD {
  bool sostenuto = false;

  Instrument instrument = 0;
  Channel channel = 0;

  std::map<int, std::string> v = {
    {VK_F1,"C1"},{VK_F2,"D1"},{VK_F3,"E1"},{VK_F4,"F1"},{VK_F5,"G1"},{VK_F6,"A2"},{VK_F7,"B2"},{VK_F8,"C2"},{VK_F9,"D2"},{VK_F10,"E2"},{VK_F11,"F2"},{VK_F12,"G2"},
    {'1',"C2"},{'2',"D2"},{'3',"E2"},{'4',"F2"},{'5',"G2"},{'6',"A3"},{'7',"B3"},{'8',"C3"},{'9',"D3"},{'0',"E3"},{VK_OEM_MINUS,"F3"},{VK_OEM_PLUS,"G3"},
    {'Q',"C3"},{'W',"D3"},{'E',"E3"},{'R',"F3"},{'T',"G3"},{'Y',"A4"},{'U',"B4"},{'I',"C4"},{'O',"D4"},{'P',"E4"},{VK_OEM_4,"F4"},{VK_OEM_6,"G4"},
    {'A',"C4"},{'S',"D4"},{'D',"E4"},{'F',"F4"},{'G',"G4"},{'H',"A5"},{'J',"B5"},{'K',"C5"},{'L',"D5"},{VK_OEM_1,"E5"},{VK_OEM_7,"F5"},
    {'Z',"C5"},{'X',"D5"},{'C',"E5"},{'V',"F5"},{'B',"G5"},{'N',"A6"},{'M',"B6"},{VK_OEM_COMMA,"C6"},{VK_OEM_PERIOD,"D6"},{VK_OEM_2,"E6"}
  };
  using DecoratedSound = Played<Displayed<Sound>>;
  std::map<int, DecoratedSound> m_sound;
  std::mutex mtx; // protect m_sound

  DecoratedSound get_sound(int c) {
    Sound n{ v[c], "f", channel };
    n.pitch += is_pressed(VK_LSHIFT) - is_pressed(VK_RSHIFT);
    return n;
  }

  void on_press(int c) override {
    std::lock_guard<std::mutex> lock(mtx);
    if (v.find(c) != v.end()) {
      if (m_sound.find(c) == m_sound.end())
        m_sound.insert({c, get_sound(c)}); // create the note and start it
      else
        m_sound.at(c).replay();
    } else switch(c){
      case VK_CAPITAL:
        if(sostenuto) for(auto it = m_sound.begin(); it != m_sound.end();)
          is_pressed(it->first) ? ++it : it = m_sound.erase(it);
        sostenuto = !sostenuto;
        break;
      case VK_TAB:
        MIDI::set_instrument(channel, is_pressed(VK_SHIFT) ? --instrument : ++instrument);
        break;      
      case VK_CONTROL:
        m_sound.clear();
        MIDI::mute(channel);
        break;
    }
  }

  void on_release(int c) override {
    if (v.find(c) != v.end() && !sostenuto) {
      std::lock_guard<std::mutex> lock(mtx);
      m_sound.erase(c);
    }
  }

public:
  Piano(Channel c=0) : channel(c) {}
};

#endif