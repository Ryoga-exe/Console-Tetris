#pragma once
#include <Windows.h>
#include <conio.h>
#include "Timer.h"
#include "Singleton.h"
#include "Keyboard.h"

enum {
	L_BLACK,	// ��P�x�E���F
	L_BLUE,		// ��P�x�E�F
	L_GREEN,	// ��P�x�E�ΐF
	L_CYAN,		// ��P�x�E���F
	L_RED,		// ��P�x�E�ԐF
	L_PURPLE,	// ��P�x�E���F
	L_YELLOW,	// ��P�x�E���F
	L_WHITE,	// ��P�x�E���F

	H_BLACK,	// ���P�x�E���F
	H_BLUE,		// ���P�x�E�F
	H_GREEN,	// ���P�x�E�ΐF
	H_CYAN,		// ���P�x�E���F
	H_RED,		// ���P�x�E�ԐF
	H_PURPLE,	// ���P�x�E���F
	H_YELLOW,	// ���P�x�E���F
	H_WHITE,	// ���P�x�E���F
};

struct FPS_t {
	Timer tm;
	int   count = 0;
	float currentFPS = 0;
	int N = 60, FPSRate = 60;
};

WORD GetColor(int foregroundColor, int backgroundColor);
WORD GetForegroundColor(WORD color);
WORD GetBackgroundColor(WORD color);

int  GetRand();
int  GetRand(int max);
int  GetRand(int min, int max);
COORD AddCoord(COORD coord1, COORD coord2);

class Console : public Singleton<Console> {
	Console();
	~Console();
	friend Singleton<Console>;
public:
	/* �ݒ�n */
	BOOL SetWindowSize(SHORT sizeX, SHORT sizeY);
	BOOL SetWindowBufferSize(SHORT sizeX, SHORT sizeY);
	BOOL SetWindowBufferSizeAuto();
	BOOL SetWindowAndBufferSize(SHORT sizeX, SHORT sizeY);
	BOOL SetCursorVisible(BOOL visible);
	BOOL SetCursorSize(DWORD size);
	BOOL SetCursorInvisible();
	BOOL SetCursorPostion(COORD coord);
	BOOL SetCursorPostion(SHORT x, SHORT y);
	BOOL SetTextColor(WORD color);
	void SetBGColor(WORD color);

	/* ���擾�n */
	SMALL_RECT GetWindowSize();
	void       GetWindowSize(SHORT &top, SHORT &bottom, SHORT &left, SHORT &right);
	BOOL  GetCursorVisible();
	DWORD GetCursorSize();
	COORD GetCursorPosition();
	void  GetCursorPosition(SHORT &x, SHORT &y);
	WORD  GetTextColor();
	/* ���C�������n */
	bool ProcessLoop();
	void SetFPSRate(int fps);
	float GetFPSRate();
	/* �L�[���͌n */
	int GetKeyEvent();
	/* �`��n */
	BOOL DrawPixel(SHORT x, SHORT y, WORD color);
	BOOL DrawBox(SHORT x, SHORT y, SHORT width, SHORT height, WORD color);
	BOOL Print(SHORT x, SHORT y, WORD color, const char *str);
	BOOL Printf(SHORT x, SHORT y, WORD color, const char *format, ...);



private:
	HANDLE GetHandle() {
		return m_flipScreen ? m_hConsoleStdOut : m_hConsoleStdOut2;
	}
	bool FlipScreen() {
		SetConsoleActiveScreenBuffer(GetHandle());
		m_flipScreen ^= 1;
		return false;
	}
	bool ClearScreen() {
		DWORD	dwNumberOfCharsWritten;
		FillConsoleOutputAttribute(GetHandle(), m_bgColor, m_csbi.dwMaximumWindowSize.X * m_csbi.dwMaximumWindowSize.Y, { 0, 0 }, &dwNumberOfCharsWritten);
		FillConsoleOutputCharacter(GetHandle(), ' ', m_csbi.dwMaximumWindowSize.X * m_csbi.dwMaximumWindowSize.Y, { 0, 0 }, &dwNumberOfCharsWritten);
		return false;
	}
	BOOL SetWindowSize(SMALL_RECT sr) {
		m_csbi.srWindow = sr;
		return SetConsoleWindowInfo(m_hConsoleStdOut, TRUE, &m_csbi.srWindow) | SetConsoleWindowInfo(m_hConsoleStdOut2, TRUE, &m_csbi.srWindow);
	}
	bool FPSUpdate() {
		if (m_fps.count == 0) {
			m_fps.tm.Start();
		}
		if (m_fps.count == m_fps.N) {
			m_fps.currentFPS = 1000.f / ((m_fps.tm.Elapse()) / (float)m_fps.N);
			m_fps.count = 0;
			m_fps.tm.Start();
		}
		m_fps.count++;
		return true;
	}
	void WaitForFPS() {
		int tookTime = (int)m_fps.tm.Elapse();
		int waitTime = m_fps.count * 1000 / m_fps.FPSRate - tookTime;
		if (waitTime > 0) {
			Sleep(waitTime);
		}
	}
	int  GetInputKey() {
		if (_kbhit() == 0) return KEY_NOT_INPUTED;
		int key = _getch();
		switch (key) {
		case 0x00:
		case 0xE0:
			switch (_getch())
			{
			case 0x48:
				return KEY_INPUT_UP;
			case 0x4B:
				return KEY_INPUT_LEFT;
			case 0x4D:
				return KEY_INPUT_RIGHT;
			case 0x50:
				return KEY_INPUT_DOWN;
			default:
				return KEY_UNDEFINED;
			}
			break;
		default:
			return key;
		}
	}

private:
	HANDLE                      m_hConsoleStdOut,
								m_hConsoleStdOut2;
	CONSOLE_SCREEN_BUFFER_INFO	m_csbi;
	CONSOLE_CURSOR_INFO	        m_cci;
	FPS_t m_fps;
	WORD  m_bgColor = GetColor(L_WHITE, L_BLACK);
	int   m_keyEvent;
	bool  m_flipScreen;
};