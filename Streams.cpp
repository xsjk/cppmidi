#include "Streams.hpp"
#include "Keyboard.hpp"

int main() {


  Stream<Sound, MIDI> soundStream;
  // Stream<Sound, DISPLAY> stream;
  soundStream << Sound{"A4"};  // starts playing
  Sleep(1000);
  soundStream >> Sound{"A4"};  // stops playing

  Stream<Note, MIDI> noteStream;
  // Stream<Note, DISPLAY> stream2;
  noteStream << Note{500u, "C4"};  // starts playing and automatically stops after 0.5 seconds
  Sleep(1000);

  Stream<Track, MIDI> trackStream;
  // Stream<Track, DISPLAY> stream3;
  trackStream << Track{"两只老虎.txt"};  // starts playing


  KEYBOARD::wait_key(VK_ESCAPE);

}