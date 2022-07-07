#ifndef __SOUND_HPP
#define __SOUND_HPP

using BYTE = unsigned char;

#include <string>

class Pitch {
  BYTE pitch;
  Pitch(char c, int n, char f=' ') : Pitch{ (BYTE)(21 + 2*(c-'A') - (c>'E' ? 2 : c>'B' ? 1 : 0) + 12 * n + (f == '#' ? 1 : f == 'b' ? -1 : 0)) } {}
public:
  Pitch(int pitch) : pitch{ (BYTE)pitch } {}
  Pitch(const char* s) : Pitch{ std::string(s) } {}
  Pitch(std::string s) : Pitch{ (s.length() == 3 ? s[1] : s[0]),(s.length() == 3 ? s[2] : s[1]) - '0',(s.length() == 3 ? s[0] : ' ')} {}
  operator BYTE() const { return pitch; }
  Pitch& operator += (int n) { pitch += n; return *this; }
  friend int operator-(const Pitch& a, const Pitch& b) { return a.pitch - b.pitch; }
};

class Volume {
  BYTE volume;
public:
  Volume(BYTE volume) : volume{ volume } {}
  Volume(const char* s) : Volume(std::string(s)) {}
  Volume(std::string s) : Volume{ (BYTE)(s=="pppp" ? 8 : s=="ppp" ? 20 : s=="pp" ? 31 : s=="p" ? 42 : s=="mp" ? 53 : s=="mf" ? 64 : s=="f" ? 80 : s=="ff" ? 96 : s=="fff" ? 112 : s=="ffff" ? 127 : 0) } {}
  operator BYTE() const { return volume; }
};

using Channel = BYTE;

class Instrument {
  BYTE instrument;
public:
  Instrument(int instrument) : instrument(instrument) {}
  Instrument& operator++() { ++instrument %= 1<<7; return *this; }
  Instrument& operator--() { --instrument %= 1<<7; return *this; }
  operator BYTE() const { return instrument; }
};

struct Sound {
  Pitch pitch = 0;
  Volume volume = "f";
  Channel channel = 0;
  Instrument instrument = 0;
  // bool operator<(const Note& other) const { return pitch < other.pitch; }
};



#endif // __SOUND_HPP