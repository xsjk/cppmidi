#ifndef __DECORATOR_HPP
#define __DECORATOR_HPP

#include <type_traits>
#include <utility>

template <class T, class H> class Decorated;
template <class T> struct is_Decorated : std::false_type {};
template <class T, class H> struct is_Decorated<Decorated<T, H>> : std::true_type {};
template <class T> inline constexpr bool is_Decorated_v = is_Decorated<T>::value;

// a decotator to start the Note while constructing the object and stop it when the object is destroyed.
template <class T, class H>
class Decorated : public T {
  mutable bool flag = false; // flag to indicate whether this object hold the lifecycle of the Note.
  static H stream;
public:
  Decorated(const Decorated& other) = delete;
  // template <class... A>
  template <class... A, typename std::enable_if_t<(... && !is_Decorated_v<A>)>* = nullptr>
  Decorated(A&&... a) : T{std::forward<A>(a)...}, flag{ true } {
    stream << *this;
  }
  Decorated(T&& s) : T{ std::move(s) }, flag{ true } {
    stream << *this;
  }
  
  ~Decorated() { 
    if (flag) stream >> *this;
  }
  Decorated(Decorated&& other) noexcept : T{ std::move(other) }, flag{ true } {
    other.flag = false; 
  }

  void replay() { 
    stream << *this;
    flag = true; 
  }
};
template <class T, class H> H Decorated<T, H>::stream;


#include "Streams.hpp"
#include "Display.hpp"
#include "Midi.hpp"

// a decotator to display the Note while constructing and stop when destroyed.
template <class T> using Displayed = Decorated<T, Stream<T,DISPLAY>>;

// a decotator to play the Note while constructing and stop when destroyed.
template <class T> using Played = Decorated<T, Stream<T,MIDI>>;



#endif // __DECORATOR_HPP
