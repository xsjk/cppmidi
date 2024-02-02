#ifndef __SOUND_HPP
#define __SOUND_HPP

using BYTE = unsigned char;

#include <string>

class Pitch {
  BYTE pitch;
  Pitch(char c, int n, char f=' ') : Pitch{ (BYTE)(21 + 2*(c-'A') - (c>'E' ? 2 : c>'B' ? 1 : 0) + 12 * n + (f == '#' ? 1 : f == 'b' ? -1 : 0)) } {}
public:
  Pitch() : pitch{0} {}
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

class Channel {
  BYTE channel;
public:
  Channel(int c) : channel{ BYTE(c) } {}
  operator BYTE() const { return channel; }
};

class Instrument {
  BYTE instrument: 7;
public:
  Instrument(int instrument) : instrument(instrument) {}
  Instrument& operator++() { ++instrument; return *this; }
  Instrument& operator--() { --instrument; return *this; }
  operator BYTE() const { return instrument; }
  operator std::string() const {
    switch (instrument) {
      case 0: return "Acoustic Grand Piano";
      case 1: return "Bright Acoustic Piano";
      case 2: return "Electric Grand Piano";
      case 3: return "Honky-tonk Piano";
      case 4: return "Electric Piano 1";
      case 5: return "Electric Piano 2";
      case 6: return "Harpsichord";
      case 7: return "Clavi";
      case 8: return "Celesta";
      case 9: return "Glockenspiel";
      case 10: return "Music Box";
      case 11: return "Vibraphone";
      case 12: return "Marimba";
      case 13: return "Xylophone";
      case 14: return "Tubular Bells";
      case 15: return "Dulcimer";
      case 16: return "Drawbar Organ";
      case 17: return "Percussive Organ";
      case 18: return "Rock Organ";
      case 19: return "Church Organ";
      case 20: return "Reed Organ";
      case 21: return "Accordion";
      case 22: return "Harmonica";
      case 23: return "Tango Accordion";
      case 24: return "Acoustic Guitar (nylon)";
      case 25: return "Acoustic Guitar (steel)";
      case 26: return "Electric Guitar (jazz)";
      case 27: return "Electric Guitar (clean)";
      case 28: return "Electric Guitar (muted)";
      case 29: return "Overdriven Guitar";
      case 30: return "Distortion Guitar";
      case 31: return "Guitar harmonics";
      case 32: return "Acoustic Bass";
      case 33: return "Electric Bass (finger)";
      case 34: return "Electric Bass (pick)";
      case 35: return "Fretless Bass";
      case 36: return "Slap Bass 1";
      case 37: return "Slap Bass 2";
      case 38: return "Synth Bass 1";
      case 39: return "Synth Bass 2";
      case 40: return "Violin";
      case 41: return "Viola";
      case 42: return "Cello";
      case 43: return "Contrabass";
      case 44: return "Tremolo Strings";
      case 45: return "Pizzicato Strings";
      case 46: return "Orchestral Harp";
      case 47: return "Timpani";
      case 48: return "String Ensemble 1";
      case 49: return "String Ensemble 2";
      case 50: return "SynthStrings 1";
      case 51: return "SynthStrings 2";
      case 52: return "Choir Aahs";
      case 53: return "Voice Oohs";
      case 54: return "Synth Voice";
      case 55: return "Orchestra Hit";
      case 56: return "Trumpet";
      case 57: return "Trombone";
      case 58: return "Tuba";
      case 59: return "Muted Trumpet";
      case 60: return "French Horn";
      case 61: return "Brass Section";
      case 62: return "SynthBrass 1";
      case 63: return "SynthBrass 2";
      case 64: return "Soprano Sax";
      case 65: return "Alto Sax";
      case 66: return "Tenor Sax";
      case 67: return "Baritone Sax";
      case 68: return "Oboe";
      case 69: return "English Horn";
      case 70: return "Bassoon";
      case 71: return "Clarinet";
      case 72: return "Piccolo";
      case 73: return "Flute";
      case 74: return "Recorder";
      case 75: return "Pan Flute";
      case 76: return "Blown Bottle";
      case 77: return "Shakuhachi";
      case 78: return "Whistle";
      case 79: return "Ocarina";
      case 80: return "Lead 1 (square)";
      case 81: return "Lead 2 (sawtooth)";
      case 82: return "Lead 3 (calliope)";
      case 83: return "Lead 4 (chiff)";
      case 84: return "Lead 5 (charang)";
      case 85: return "Lead 6 (voice)";
      case 86: return "Lead 7 (fifths)";
      case 87: return "Lead 8 (bass + lead)";
      case 88: return "Pad 1 (new age)";
      case 89: return "Pad 2 (warm)";
      case 90: return "Pad 3 (polysynth)";
      case 91: return "Pad 4 (choir)";
      case 92: return "Pad 5 (bowed)";
      case 93: return "Pad 6 (metallic)";
      case 94: return "Pad 7 (halo)";
      case 95: return "Pad 8 (sweep)";
      case 96: return "FX 1 (rain)";
      case 97: return "FX 2 (soundtrack)";
      case 98: return "FX 3 (crystal)";
      case 99: return "FX 4 (atmosphere)";
      case 100: return "FX 5 (brightness)";
      case 101: return "FX 6 (goblins)";
      case 102: return "FX 7 (echoes)";
      case 103: return "FX 8 (sci-fi)";
      case 104: return "Sitar";
      case 105: return "Banjo";
      case 106: return "Shamisen";
      case 107: return "Koto";
      case 108: return "Kalimba";
      case 109: return "Bag pipe";
      case 110: return "Fiddle";
      case 111: return "Shanai";
      case 112: return "Tinkle Bell";
      case 113: return "Agogo";
      case 114: return "Steel Drums";
      case 115: return "Woodblock";
      case 116: return "Taiko Drum";
      case 117: return "Melodic Tom";
      case 118: return "Synth Drum";
      case 119: return "Reverse Cymbal";
      case 120: return "Guitar Fret Noise";
      case 121: return "Breath Noise";
      case 122: return "Seashore";
      case 123: return "Bird Tweet";
      case 124: return "Telephone Ring";
      case 125: return "Helicopter";
      case 126: return "Applause";
      case 127: return "Gunshot";
      default: return "Unknown Instrument";
    }
  }
};

struct Sound {
  Pitch pitch;
  Volume volume = "f";
  Channel channel = 0;
  Instrument instrument = 0;
};





#endif // __SOUND_HPP