#pragma once

#include "Console.hpp"
#include <list>
#include <iostream>

class Display : Console {
  static bool OutOfRange(COORD pos) {
    return pos.X >= get_width()-5;
  }
  class Block {
    WORD color;
    WORD length = 1;
    COORD pos;  // position of the tail
    void print_at(COORD pos) const {
      if(OutOfRange()) return;
      set_color(color);
      set_pos(pos);
      std::cout << 
      #ifdef _WIN32
        "\xa8\x80"
      #else
        "\xe2\x96\x88"
      #endif
      ;
    }
    void erase(COORD pos) const {
      set_pos(pos);
      std::cout << ' ';
    }
  public:
    Block() {
      set_color(color);
    }
    enum class Mode { APPEND, FORWARD } mode = Mode::APPEND;
    Block(WORD color, COORD pos) : color{ color }, pos{ pos } {}
    void forward()  {
      erase(tail());
      ++pos.X;
      print_at(head());
    }
    void append() {
      ++length;
      print_at(head());
    }
    bool OutOfRange() const { 
      return Display::OutOfRange(pos); 
    }
    bool operator==(const Block& b) const { 
      return pos.X == b.pos.X && pos.Y == b.pos.Y && color == b.color; 
    }
    bool operator<(Block& b) { 
      return pos.X < b.pos.X; 
    }
    COORD head() {
      return {short(pos.X+length-1), pos.Y};
    }
    COORD tail() {
      return pos;
    }
    SHORT max_x() const {
      return get_width()-5;
    }
  };
  std::list<Block> blocks;
public:
  static COORD origin;
  void update() {
    for(auto &b: blocks) b.forward();
    blocks.remove_if([](Block& b) { return b.OutOfRange(); });
  }
  void emplace(WORD c, COORD p) {
    auto it = std::find(blocks.begin(), blocks.end(), Block(c, p));
    if(it == blocks.end())
      blocks.emplace_back(c, p);
    else
      it->append();
  }
  void clear() {
    system("cls");
  }
  template <typename ... Args>
  static void info(Args && ... args) {
    Console::set_pos(0,0);
    Console::set_color(FOREGROUND_INTENSITY);
    (std::cout << ... << args) << std::endl;
  }
};

COORD Display::origin = {0,3};



#include "Async.hpp"
#include <set>

class DISPLAY : Loop {
  static Display handle;
  static unsigned int delay;
  static std::set<std::pair<Channel,Pitch>> displaying;
  void emplace_block(Channel c, Pitch p) {
    short d = p-Pitch("C1");
    handle.emplace((int(c) + 1)%16,{ short(10+Display::origin.X), short(d + d/12 + Display::origin.Y) });
  }
  static std::mutex mtx;
  void update() {
    if(!displaying.empty()) {
      std::lock_guard<std::mutex> lk(mtx);
      for (auto [c, p] : displaying)
        if (is_alive())
          emplace_block(c, p);
        else
          break;
    }
    handle.update();
  }
public:
  
  static void init() {
    Console::set_color();
    Console::set_pos(Display::origin);
    for (int i=1; i<=6; ++i) {
      for (auto s: {" C", "#C", " D", "bE", " E", " F", "F#", " G", "#G"})
        std::cout << s << i << std::endl;
      for (auto s: {" A", "bB", " B"})
        std::cout << s << i+1 << std::endl;
      std::cout << std::endl;
    }
  }
  template <typename ... Args>
  static void info(Args && ... args) {
    handle.info(std::forward<Args>(args)...);
  }
  static void clear() {
    handle.clear();
  }
  DISPLAY() : Loop{std::bind(&DISPLAY::update, this), delay} {}
  DISPLAY& operator<<(Sound s) {
    std::lock_guard<std::mutex> lk(mtx);
    if(s.pitch) displaying.insert({s.channel, s.pitch});
    return *this;
  }
  DISPLAY& operator>>(Sound s) {
    std::lock_guard<std::mutex> lk(mtx);
    if(s.pitch) displaying.erase({s.channel, s.pitch});
    return *this;
  }
};
Display DISPLAY::handle;
unsigned int DISPLAY::delay = 100;
std::set<std::pair<Channel,Pitch>> DISPLAY::displaying;
std::mutex DISPLAY::mtx;
