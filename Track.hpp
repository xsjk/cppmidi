#ifndef __TRACK_HPP
#define __TRACK_HPP

#include <list>
#include <fstream>
#include <chrono>
#include <string>

#include "Note.hpp"
#include "Decorator.hpp"
#include "Async.hpp"


class Track : public std::list<Note> {
  static auto read(std::string filename) {
    std::list<Note> notes;
    std::ifstream file(filename);
    if (!file.is_open())
      throw std::runtime_error("Could not open file " + filename);
    /* Format:
        base_pitch base_duration
        channel
        instrument
        duration    pitch
        duration    pitch
        ...
        duration    pitch
     */
    int base, interval, channel, instrument, pause;
    std::string pitch;
    file >> base >> interval >> channel >> instrument;
    while (file >> pause >> pitch) {
      if (pitch == "_")
        notes.emplace_back(pause * interval);
      else 
        notes.emplace_back(pause * interval , Sound{ base + std::stoi(pitch), "f", (BYTE)channel, instrument });
    }
    return notes;
  }
public:
  Track(std::string filename) : std::list<Note>{read(filename)} {}
  Track(const char* filename) : Track(std::string(filename)) {}
};




#endif