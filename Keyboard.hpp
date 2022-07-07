#ifndef __KEYBOARD_HPP
#define __KEYBOARD_HPP

#include "Async.hpp"
#include <set>
#include <Windows.h>
#include <list>


class KEYBOARD {
  static Loop thread;
  static std::set<int> pressed;
  static std::mutex mtx;
  static std::list<KEYBOARD*> keyboard_list;
protected:
  virtual void on_press(int c) {}
  virtual void on_release(int c) {}
  static void update() {
    for (int c = 0; c < 1 << 8; ++c) {
      if (GetAsyncKeyState(c) < 0) {
        if (!is_pressed(c)) {
          {
            // std::lock_guard<std::mutex> lk(mtx);
            pressed.insert(c);
          }
          for(auto k : keyboard_list) k->on_press(c);
        }
      } else if (is_pressed(c)) {
        {
          // std::lock_guard<std::mutex> lk(mtx);
          pressed.erase(c);
        }
        for(auto k : keyboard_list) k->on_release(c);
      }
    }
  }
public:
  static void wait_key(int c) {
    while (!is_pressed(c))
      thread.sleep(20);
  }
  KEYBOARD() {
    keyboard_list.push_back(this);
  }
  ~KEYBOARD() {
    keyboard_list.remove(this);
  }
  static bool is_pressed(int c) { 
    // std::lock_guard<std::mutex> lck(mtx);
    return pressed.find(c) != pressed.end(); 
  }
};
std::set<int> KEYBOARD::pressed;
std::mutex KEYBOARD::mtx;
std::list <KEYBOARD*> KEYBOARD::keyboard_list;
Loop KEYBOARD::thread{ &KEYBOARD::update };

#endif // #define __KEYBOARD_HPP