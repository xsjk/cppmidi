#ifndef __CONSOLE_HPP
#define __CONSOLE_HPP

#include <Windows.h>

class Console {
  static HANDLE handle;
  static CONSOLE_SCREEN_BUFFER_INFO info;
public:
  static void set_pos(short x, short y) {
    set_pos({x, y});
  }
  static void set_pos(COORD pos) {
    SetConsoleCursorPosition(handle, pos);
    // std::printf("\033[%d;%dH", pos.Y, pos.X);
  }
  static void set_color(WORD color=0x07) {
    SetConsoleTextAttribute(handle, color);
    // std::printf("\033[0;%dm", color+30);
  }
  static short get_width() {
    return get_info(), info.srWindow.Right - info.srWindow.Left + 1;
  }
  static COORD get_pos() {
    return get_info(), info.dwCursorPosition;
  }
  static const CONSOLE_SCREEN_BUFFER_INFO& get_info() {
    GetConsoleScreenBufferInfo(handle, &info);

    return info;
  }
  static void clear() {
    system("cls");
    std::printf("\033[2J");
  }
};
HANDLE Console::handle = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO Console::info;

#endif  // __CONSOLE_HPP