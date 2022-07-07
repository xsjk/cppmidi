#ifndef __STREAMS_HPP
#define __STREAMS_HPP

#include "Sound.hpp"


template<class T>
class StreamBase {
public:
  StreamBase& operator<<(const T&) { return *this; }
  StreamBase& operator>>(const T&) { return *this; }
};

template<class T,class S> class Stream;


// decorated
template<class T,class H>
class Decorated;
template<class T,class S, class H> 
class Stream<Decorated<T,H>,S> : public Stream<T,S> {};
// #define DEBUG

// play a sound by operator<< and stops the sound by operator>>
template <class H>
class Stream<Sound,H> {
protected:
  static H handle;
public:
  Stream<Sound,H>& operator<<(Sound n) { 
    #ifdef DEBUG
      Display::info("begin pitch:", n.pitch, "handle:", TYPENAME(handle));
    #endif
    handle << n; return *this; 
  }
  Stream<Sound,H>& operator>>(Sound n) { 
    #ifdef DEBUG
      Display::info("end pitch:", n.pitch, "handle:", TYPENAME(handle));
    #endif
    handle >> n; return *this; 
  }
};
template<class H> H Stream<Sound,H>::handle;


#include "Async.hpp"
#include "Note.hpp"
#include <map>

// add note by operator<< and play the note at once
// the note playing will end itself at certain time
template <class S>
class Stream<Note,S> : public Loop {
  static Stream<Sound, S> soundStream;
  std::map<std::chrono::steady_clock::time_point, Sound> notes;
  std::mutex mtx;
  void update() {
    std::list<std::chrono::steady_clock::time_point> finished;
    std::lock_guard<std::mutex> lock(mtx);
    if (notes.empty()) return;
    for(auto [time,sound]: notes) {
      if (time > std::chrono::steady_clock::now()) break;
      soundStream >> sound;
      finished.push_back(time);
    }
    for(const auto time: finished) 
      notes.erase(time);
  }
public:
  Stream<Note,S>() : Loop{std::bind(&Stream<Note,S>::update, this)} { }
  ~Stream<Note,S>() { kill(); }
  Stream<Note,S>& operator<<(const Note& note) {
    soundStream << note.sound;
    std::lock_guard<std::mutex> lock(mtx);
    notes.insert({
      std::chrono::steady_clock::now()+std::chrono::milliseconds(note.duration),
      note.sound
    });
    return *this;
  }
  Stream<Note,S>& operator>>(const Note&) { return *this; }
};
template<class S> Stream<Sound,S> Stream<Note,S>::soundStream;


#include <list>
#include "Track.hpp"

// add track by operator<< and start the track at once
template <class S>
class Stream<Track,S> : Loop {
  static Stream<Sound,S> soundStream;
  struct TrackInfo : public Track {
    std::chrono::steady_clock::time_point time;
    std::list<Note>::iterator current;
    TrackInfo(const Track& t) : Track(t) {
      current = begin();
      time = std::chrono::steady_clock::now() + std::chrono::milliseconds(current->duration);
      soundStream << current->sound;
    }
  };
  std::list<TrackInfo> tracks;
  std::mutex mtx;
  void update() {
    if (tracks.empty()) return;
    std::lock_guard<std::mutex> lock(mtx);
    for(auto it = tracks.begin(); it!=tracks.end();) {
      if (it->time < std::chrono::steady_clock::now()) {
        soundStream >> it->current->sound;
        if (++it->current == it->end()) {
          it = tracks.erase(it);
          continue;
        }
        it->time += std::chrono::milliseconds(it->current->duration);
        soundStream << it->current->sound;
      }
      ++it;
    }
  }
public:
  Stream<Track,S>() : Loop{std::bind(&Stream<Track,S>::update, this)} {}
  ~Stream<Track,S>() { kill(); }
  Stream<Track,S>& operator<<(const Track& t) {
    std::lock_guard<std::mutex> lock(mtx);
    tracks.emplace_back(t);
    return *this;
  }
  Stream<Track,S>& operator>>(const Track& n) { return *this; }
};
template<class S> Stream<Sound,S> Stream<Track,S>::soundStream;


#endif // __STREAMS_HPP