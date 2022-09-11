#ifndef __TRACK_HPP
#define __TRACK_HPP

#include <list>
#include <fstream>
#include <chrono>
#include <string>
#include <numeric>

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
  Track() = default;
  Track(std::string filename) : std::list<Note>{read(filename)} {}
  Track(const char* filename) : Track(std::string(filename)) {}
  Track(std::initializer_list<Note> notes) : std::list<Note>{notes} {}

  Track& operator*=(size_t n) {
    // copy the track n times
    Track t;
    for (size_t i = 0; i < n; ++i)
      t.insert(t.end(), begin(), end());
    return *this = t;
  }
  Track& operator+=(const Track& other) {
    insert(end(), other.begin(), other.end());
    return *this;
  }
  Track& operator+=(const Note& note) {
    // concatenate the track with a note
    push_back(note);
    return *this;
  }


  template <typename A>
  Track operator+(A a) const {
    return Track(*this) += std::forward<A>(a);
  }
  template <typename A>
  Track operator*(A a) const {
    return Track(*this) *= std::forward<A>(a);
  }

  auto duration() const {
    return std::accumulate(
      begin(), end(), 0, 
      [](unsigned int acc, const Note& note) {
        return acc + note.duration;
      }
    );
  }

};




#endif