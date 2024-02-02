#ifndef __PIANO_HPP
#define __PIANO_HPP

#include <map>
#include "Keyboard.hpp"
#include "Decorator.hpp"

class Piano : public KEYBOARD {
  bool sostenuto = false;

  Instrument instrument = 0;
  Channel channel = 0;

  std::map<WORD, std::string> v = {
    {VK_F1,"C5"},{VK_F2,"D5"},{VK_F3,"E5"},{VK_F4,"F5"},{VK_F5,"G5"},{VK_F6,"A6"},{VK_F7,"B6"},{VK_F8,"C6"},{VK_F9,"D6"},{VK_F10,"E6"},{VK_F11,"F6"},{VK_F12,"G6"},
    {'1',"C4"},{'2',"D4"},{'3',"E4"},{'4',"F4"},{'5',"G4"},{'6',"A5"},{'7',"B5"},{'8',"C5"},{'9',"D5"},{'0',"E5"},{VK_OEM_MINUS,"F5"},{VK_OEM_PLUS,"G5"},
    {'Q',"C3"},{'W',"D3"},{'E',"E3"},{'R',"F3"},{'T',"G3"},{'Y',"A4"},{'U',"B4"},{'I',"C4"},{'O',"D4"},{'P',"E4"},{VK_OEM_4,"F4"},{VK_OEM_6,"G4"},
    {'A',"C2"},{'S',"D2"},{'D',"E2"},{'F',"F2"},{'G',"G2"},{'H',"A3"},{'J',"B3"},{'K',"C3"},{'L',"D3"},{VK_OEM_1,"E3"},{VK_OEM_7,"F3"},
    {'Z',"C1"},{'X',"D1"},{'C',"E1"},{'V',"F1"},{'B',"G1"},{'N',"A2"},{'M',"B2"},{VK_OEM_COMMA,"C2"},{VK_OEM_PERIOD,"D2"},{VK_OEM_2,"E2"}
  };
  using DecoratedSound = Played<Displayed<Sound>>;
  std::map<WORD, DecoratedSound> m_sound;
  std::mutex mtx; // protect m_sound

  DecoratedSound get_sound(WORD c) {
    Sound n{ v[c], "f", channel };
    n.pitch += is_pressed(VK_SHIFT) - is_pressed(VK_CONTROL);
    return n;
  }

  void on_press(WORD c) override {
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
        {
          std::string name = std::string(instrument);
          Console::set_pos({5,0});
          std::cout << std::string(Console::get_width()-name.size()-5,' ') + name;
        }
        break;
      case VK_DELETE:
        m_sound.clear();
        MIDI::mute(channel);
        break;
    }
  }

  void on_release(WORD c) override {
    if (v.find(c) != v.end() && !sostenuto) {
      std::lock_guard<std::mutex> lock(mtx);
      m_sound.erase(c);
    }
  }

public:
  Piano(Channel c=0) : channel(c) {}
};

#endif