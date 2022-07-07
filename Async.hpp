#ifndef __ASYNC_HPP
#define __ASYNC_HPP

#include <functional>
#include <chrono>
#include <list>
#include <atomic>
#include <future>

#if __cplusplus > 201703L
  #define CAPTURE  =,this
#else
  #define CAPTURE  =
#endif

using Flag = std::atomic<bool>;
#define FROM_NOW(t) std::chrono::duration_cast<std::chrono::milliseconds>(t-std::chrono::steady_clock::now()).count()


class Thread : public std::future<void> {
  static std::list<Thread*> threads;
  Flag alive_flag;
public:
  Thread() {
    threads.push_back(this);

  }
  Thread(const Thread &) = delete;
  Thread(Thread&&) = delete;
  Thread& operator=(Thread&& other) {
    alive_flag = other.alive_flag.load();
    std::future<void>::operator=(std::move(other));
    return *this;
  }
  Thread(std::function<void()> f): std::future<void>{std::async(f)}, alive_flag(true) {
    threads.push_back(this);
  }
  ~Thread() {
    threads.remove(this);
    kill();
  }
  void stop() {
    alive_flag = false;
  }
  
  /// @warning 
  /// you must call this function in the derived class's destructor, 
  /// if the thread function visit the derived class's member variables
  void kill() {
    if(valid()) {
      stop();
      wait();
    }
  }
  bool is_alive() {
    return alive_flag;
  }
  bool sleep(int interval) {
    if(interval>0)
      return sleep_until(std::chrono::steady_clock::now()+std::chrono::milliseconds(interval));
    return is_alive();
  }
  bool sleep_until(std::chrono::time_point<std::chrono::steady_clock> t) {
    while(is_alive() && std::chrono::steady_clock::now() < t);
    return is_alive();
  }
  bool done() {
    return status()==std::future_status::ready;
  }
  std::future_status status() {
    return std::future<void>::wait_for(std::chrono::milliseconds(0));
  }
};

std::list<Thread*> Thread::threads;

class Loop : public Thread {
public:
  Loop(std::function<void()> f, unsigned int interval=0) :
    Thread([CAPTURE]() {
      // while(is_alive()) {
      //   f();
      //   sleep(interval);
      // }
      auto next = std::chrono::steady_clock::now();
      while (sleep_until(next+=std::chrono::milliseconds(interval))) f(); 
    }) {}
};


class Timer : public Thread {
public:
  Timer(std::function<void()> f, unsigned int interval) :
    Thread([CAPTURE]() { if(!sleep(interval)) return; f(); } ) {}
};


#endif 