#ifndef __PRINT_HPP
#define __PRINT_HPP

#include <iostream>
#include <mutex>

class Print {
  std::mutex mutex;
public:
  template <typename ...A>
  void operator()(A&& ...args) {
    std::lock_guard<std::mutex> lock(mutex);
    ((std::cout << std::forward<A>(args) << ' '), ...) << std::endl;
  }
};
Print print;

#endif