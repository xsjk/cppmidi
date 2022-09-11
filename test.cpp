#include <iostream>
#include <windows.h>
// #include <mmsystem.h>
#include <map>
#pragma comment(lib,"winmm.lib")
using namespace std;

/*
midi参数 音量<<16+声调<<8+乐器类型
音量范围 0x0-0x7f
乐器范围 0x90-0x9f 其中0x99为鼓类，其余皆为钢琴
音阶范围 0x0-7f 以下为中音的音调16进制参数对照表
(注意：以下数据仅为本人通过自己乐感来测试得出的，不一定正确，仅供参考)
Do:0x3c Re:0x3e Mi:0x40 Fa:0x41 So:0x43 La:0x45 Xi:0x47 Do+1:0x48
*/
enum Scale//这是群里的大仙帮我找到的音阶参数
{
	Rest = 0, C8 = 108, B7 = 107, A7s = 106, A7 = 105, G7s = 104, G7 = 103, F7s = 102, F7 = 101, E7 = 100,
	D7s = 99, D7 = 98, C7s = 97, C7 = 96, B6 = 95, A6s = 94, A6 = 93, G6s = 92, G6 = 91, F6s = 90, F6 = 89,
	E6 = 88, D6s = 87, D6 = 86, C6s = 85, C6 = 84, B5 = 83, A5s = 82, A5 = 81, G5s = 80, G5 = 79, F5s = 78,
	F5 = 77, E5 = 76, D5s = 75, D5 = 74, C5s = 73, C5 = 72, B4 = 71, A4s = 70, A4 = 69, G4s = 68, G4 = 67,
	F4s = 66, F4 = 65, E4 = 64, D4s = 63, D4 = 62, C4s = 61, C4 = 60, B3 = 59, A3s = 58, A3 = 57, G3s = 56,
	G3 = 55, F3s = 54, F3 = 53, E3 = 52, D3s = 51, D3 = 50, C3s = 49, C3 = 48, B2 = 47, A2s = 46, A2 = 45,
	G2s = 44, G2 = 43, F2s = 42, F2 = 41, E2 = 40, D2s = 39, D2 = 38, C2s = 37, C2 = 36, B1 = 35, A1s = 34,
	A1 = 33, G1s = 32, G1 = 31, F1s = 30, F1 = 29, E1 = 28, D1s = 27, D1 = 26, C1s = 25, C1 = 24, B0 = 23,
	A0s = 22, A0 = 21
};
enum Voice
{
	L1 = C3, L2 = D3, L3 = E3, L4 = F3, L5 = G3, L6 = A3, L7 = B3,
	M1 = C4, M2 = D4, M3 = E4, M4 = F4, M5 = G4, M6 = A4, M7 = B4,
	H1 = C5, H2 = D5, H3 = E5, H4 = F5, H5 = G5, H6 = A5, H7 = B5,
	LOW_SPEED = 500, MIDDLE_SPEED = 400, HIGH_SPEED = 300,
	_ = 0XFF
};
void Summer() {
	HMIDIOUT handle;
	midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
	int volume = 0x7f;
	int voice = 0x0;
	int sleep = 300;
	int summer[] = {
	H1,_,M5,M1,H1,H2,M3,_,H5,_,H1,H2,_,H3,_,H1,H2,_,H3,_,
	H1,_,M5,M1,H1,H2,M3,_,H5,_,H2,H2,_,H3,_,H2,H3,_,H3,_,
	H3,H2,H1,_,H1,M5,M6,M3,M5,_,H3,H2,H1,_,H1,M5,M6,M3,M5,_,
	H3,H2,H2,_,H2,H1,M7,_,M6,M7,M5,M6,_,_,
	M6,M5,M6,_,
	M5,H1,H2,H5,M5,H1,H2,M5,H5,_,
	M5,H1,H2,M5,H5,M5,H1,M5,H2,M5,H5,_,
	M5,H1,H2,H5,M5,H1,H2,M5,H5,_,
	M5,H1,H2,H5,M5,H1,M5,H2,M5,H5,_,

	H1,_,M5,M1,H1,H2,M5,H3,_,H5,M5,H2,H2,_,H3,_,H1,H2,_,H3,_,
	H1,_,M5,M1,H1,H2,M5,H3,_,H5,_,H2,H2,_,H3,_,H2,M3,_,H3,_,
	H3,H2,M3,H1,_,H1,M5,M6,M3,M5,_,H3,H2,M3,H1,_,H1,M5,M6,M3,M5,_,
	H3,H2,H2,_,H2,H1,M7,_,M6,M7,M5,M6,_,_,_,

	};
	for (auto i : summer) {
		if (i == LOW_SPEED || i == HIGH_SPEED || i == MIDDLE_SPEED) {
			sleep = i;
			continue;
		}
		if (i == _) {
			Sleep(300);
			continue;
		}
		voice = (volume << 16) + (i << 8) + 0x94;
		printf("%p\n", voice);
		midiOutShortMsg(handle, voice);
		Sleep(sleep);
	}
	midiOutClose(handle);
}
void Piano() {
	HMIDIOUT handle;
	midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
	std::map<char, int>v = {
		{'Z',C3},{'X',D3},{'C',E3},{'V',F3},{'B',G3},{'N',A3},{'M',B3},
		{'A',C4},{'S',D4},{'D',E4},{'F',F4},{'G',G4},{'H',A4},{'J',B4},
		{'Q',C5},{'W',D5},{'E',E5},{'R',F5},{'T',G5},{'Y',A5},{'U',B5},
	};
	printf("钢琴已开启，敲击键盘Q-U,A-J,Z-M\n");
	while (1) {
		for (char i = 'A'; i <= 'Z'; i++) {
			if (GetKeyState(i) < 0) {
				if (i == 'L') {
					midiOutClose(handle);
					return;
				}
				midiOutShortMsg(handle, (0x007f << 16) + (v[i] << 8) + 0x90);
				system("cls");
				printf("请按L键退出\n");
				printf("已按下%c键\n", i);
				while (GetKeyState(i) < 0)Sleep(100);
				
			}
		}
	}
}
int main()
{
	
	Summer();
	Piano();
	return 0;
}


