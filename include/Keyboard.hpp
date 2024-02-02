#ifndef __KEYBOARD_HPP
#define __KEYBOARD_HPP

#include "Async.hpp"
#include <set>
#include <Windows.h>
#include <list>


class KEYBOARD {
	DWORD cNumRead, fdwMode;
	INPUT_RECORD irInBuf[128];
  std::set<WORD> pressed;

	// Thread thread;

	HANDLE hStdin;
	DWORD fdwSaveOldMode;
protected:
	virtual void on_press(WORD) {}
	virtual void on_release(WORD) {}

	void ErrorExit(LPCSTR lpszMessage) {
		std::cerr << lpszMessage << std::endl;
		// Restore input mode on exit.
		SetConsoleMode(hStdin, fdwSaveOldMode);
		ExitProcess(0);
	}

	void KeyEventProc(KEY_EVENT_RECORD ker) {
		// std::cout << "Key event: " << ker.wVirtualKeyCode << std::endl;
		if (ker.bKeyDown) {
      pressed.emplace(ker.wVirtualKeyCode);
      on_press(ker.wVirtualKeyCode);
    } else {
      pressed.erase(ker.wVirtualKeyCode);
      on_release(ker.wVirtualKeyCode);
    }

	}

	void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr) {
		std::cout << "Resize event: ";
		std::cout << wbsr.dwSize.X << " columns by " << wbsr.dwSize.Y << " rows." << std::endl;
	}

public:
	KEYBOARD() {
		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		if (hStdin == INVALID_HANDLE_VALUE)
			ErrorExit("GetStdHandle");

		// Save the current input mode, to be restored on exit. 

		if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
			ErrorExit("GetConsoleMode");
		
		// std::cout << std::bitset<32>(fdwSaveOldMode) << std::endl;

		// Enable the window and mouse input events. 

		DWORD fdwMode = ENABLE_WINDOW_INPUT;
		// DWORD fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS;
		
		if (!SetConsoleMode(hStdin, fdwMode))
			ErrorExit("SetConsoleMode");

		// thread = Thread{ &KEYBOARD::mainloop, this };
	}
	~KEYBOARD() {
		// Restore input mode on exit.

		SetConsoleMode(hStdin, fdwSaveOldMode);
	}

  bool is_pressed(WORD c){
    return pressed.find(c) != pressed.end();
  }

	void mainloop() {
		while (true) {
			if(!ReadConsoleInput(
				hStdin,		// input buffer handle 
				irInBuf,	// buffer to read into 
				128,		// size of read buffer 
				&cNumRead	// number of records read 
			)) ErrorExit("ReadConsoleInput");

			for (DWORD i = 0; i < cNumRead; ++i) 
			switch (irInBuf[i].EventType) {
				case KEY_EVENT: // keyboard input 
          if (irInBuf[i].Event.KeyEvent.bKeyDown && irInBuf[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
            return;
          }
					KeyEventProc(irInBuf[i].Event.KeyEvent);
					break;

				case MOUSE_EVENT:
					break;

				case WINDOW_BUFFER_SIZE_EVENT:
					ResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
					break;

				case FOCUS_EVENT: 
          break;

				case MENU_EVENT:
					break;

				default:
					ErrorExit("Unknown event type");
					break;
			}
		}
	}

};

#endif // #define __KEYBOARD_HPP