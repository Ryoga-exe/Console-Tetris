#include "Console.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

WORD GetColor(int foregroundColor, int backgroundColor) {
	return foregroundColor + (backgroundColor << 4);
}
WORD GetForegroundColor(WORD color) {
	return color & 0x0F;
}
WORD GetBackgroundColor(WORD color) {
	return (color & 0xF0) >> 4;
}

int  GetRand() {
	return rand();
}
int  GetRand(int max) {
	return rand() % (max + 1);
}
int  GetRand(int min, int max) {
	return min + rand() % ((max - min) + 1);
}
COORD AddCoord(COORD coord1, COORD coord2) {
	return { coord1.X + coord2.X, coord1.Y + coord2.Y };
}

Console::Console() {
	HMENU hmenu = GetSystemMenu(GetConsoleWindow(), FALSE);
	RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
	m_hConsoleStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	m_hConsoleStdOut2 = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	m_flipScreen = true;
	GetConsoleScreenBufferInfo(m_hConsoleStdOut, &m_csbi);
	GetConsoleCursorInfo(m_hConsoleStdOut, &m_cci);
	SetTextColor(m_bgColor);
	srand((unsigned int)time(NULL));
}
Console::~Console() {
	if (m_hConsoleStdOut != INVALID_HANDLE_VALUE)
		CloseHandle(m_hConsoleStdOut);
	if (m_hConsoleStdOut2 != INVALID_HANDLE_VALUE)
		CloseHandle(m_hConsoleStdOut2);
	system("color");
}

BOOL Console::SetWindowSize(SHORT sizeX, SHORT sizeY) {
	m_csbi.srWindow = { 0, 0, sizeX * 2 - 1, sizeY - 1};
	return SetWindowSize(m_csbi.srWindow);
}
BOOL Console::SetWindowBufferSize(SHORT sizeX, SHORT sizeY) {
	m_csbi.dwMaximumWindowSize = { sizeX * 2, sizeY };
	return SetConsoleScreenBufferSize(m_hConsoleStdOut, m_csbi.dwMaximumWindowSize) | SetConsoleScreenBufferSize(m_hConsoleStdOut2, m_csbi.dwMaximumWindowSize);
}
BOOL Console::SetWindowBufferSizeAuto() {
	m_csbi.dwMaximumWindowSize = { m_csbi.srWindow.Right + 1, m_csbi.srWindow.Bottom + 1 };
	return SetConsoleScreenBufferSize(m_hConsoleStdOut, m_csbi.dwMaximumWindowSize) | SetConsoleScreenBufferSize(m_hConsoleStdOut2, m_csbi.dwMaximumWindowSize);
}
BOOL Console::SetWindowAndBufferSize(SHORT sizeX, SHORT sizeY) {
	m_csbi.srWindow = { 0, 0, sizeX * 2 - 1, sizeY - 1 };
	m_csbi.dwMaximumWindowSize = { m_csbi.srWindow.Right + 1, m_csbi.srWindow.Bottom + 1 };
	SetWindowSize(m_csbi.srWindow);
	SetConsoleScreenBufferSize(m_hConsoleStdOut, m_csbi.dwMaximumWindowSize);
	SetConsoleScreenBufferSize(m_hConsoleStdOut2, m_csbi.dwMaximumWindowSize);
	return SetWindowSize(m_csbi.srWindow);
}
BOOL Console::SetCursorVisible(BOOL visible) {
	m_cci.bVisible = visible;
	return SetConsoleCursorInfo(m_hConsoleStdOut, &m_cci) | SetConsoleCursorInfo(m_hConsoleStdOut2, &m_cci);
}
BOOL Console::SetCursorSize(DWORD size) {
	m_cci.dwSize = size;
	return SetConsoleCursorInfo(m_hConsoleStdOut, &m_cci) | SetConsoleCursorInfo(m_hConsoleStdOut2, &m_cci);
}
BOOL Console::SetCursorInvisible() {
	m_cci = { 1, FALSE };
	return SetConsoleCursorInfo(m_hConsoleStdOut, &m_cci) | SetConsoleCursorInfo(m_hConsoleStdOut2, &m_cci);
}
BOOL Console::SetCursorPostion(COORD coord) {
	m_csbi.dwCursorPosition = coord;
	return SetConsoleCursorPosition(m_hConsoleStdOut, m_csbi.dwCursorPosition) | SetConsoleCursorPosition(m_hConsoleStdOut2, m_csbi.dwCursorPosition);
}
BOOL Console::SetCursorPostion(SHORT x, SHORT y) {
	m_csbi.dwCursorPosition = { x, y };
	return SetCursorPostion(m_csbi.dwCursorPosition);
}
BOOL Console::SetTextColor(WORD color) {
	m_csbi.wAttributes = color;
	return SetConsoleTextAttribute(m_hConsoleStdOut, color) | SetConsoleTextAttribute(m_hConsoleStdOut2, color);
}
void Console::SetBGColor(WORD color) {
	m_bgColor = color;
}

SMALL_RECT Console::GetWindowSize() {
	return m_csbi.srWindow;
}
void       Console::GetWindowSize(SHORT &top, SHORT &bottom, SHORT &left, SHORT &right) {
	top = m_csbi.srWindow.Top;
	bottom = m_csbi.srWindow.Bottom;
	left = m_csbi.srWindow.Left;
	right = m_csbi.srWindow.Right;
}
BOOL  Console::GetCursorVisible() {
	return m_cci.bVisible;
}
DWORD Console::GetCursorSize() {
	return m_cci.dwSize;
}
COORD Console::GetCursorPosition() {
	return m_csbi.dwCursorPosition;
}
void  Console::GetCursorPosition(SHORT &x, SHORT &y) {
	x = m_csbi.dwCursorPosition.X;
	y = m_csbi.dwCursorPosition.Y;
}
WORD  Console::GetTextColor() {
	return m_csbi.wAttributes;
}

bool Console::ProcessLoop() {
	m_keyEvent = GetInputKey();
	if (m_keyEvent == KEY_INPUT_ESC) return false;
	/*if (GetAsyncKeyState(VK_F1) & 1) {
		return false;
	}*/
	FlipScreen();
	FPSUpdate();
	WaitForFPS();
	
	ClearScreen();
	
	return true;
}
void Console::SetFPSRate(int fps) {
	m_fps.FPSRate = m_fps.N = fps;
	m_fps.tm.Start();
	m_fps.count = 0;
	m_fps.currentFPS = 0;
}
float Console::GetFPSRate() {
	return m_fps.currentFPS;
}

int Console::GetKeyEvent() {
	return m_keyEvent;
}

BOOL Console::DrawPixel(SHORT x, SHORT y, WORD color) {
	DWORD	dwNumberOfCharsWritten;
	return FillConsoleOutputAttribute(GetHandle(), color, 2, {x * 2, y}, &dwNumberOfCharsWritten);
}
BOOL Console::DrawBox(SHORT x, SHORT y, SHORT width, SHORT height, WORD color) {
	DWORD	dwNumberOfCharsWritten;
	BOOL ret = FALSE;
	for (SHORT i = 0; i < height || !ret; i++) {
		ret |= FillConsoleOutputAttribute(GetHandle(), color, width * 2, { x * 2, y + i }, &dwNumberOfCharsWritten);
	}
	return ret;
}
BOOL Console::Print(SHORT x, SHORT y, WORD color, const char *str) {
	DWORD dwNumberOfCharsWritten;
	SetConsoleCursorPosition(GetHandle(), { x, y });
	SetConsoleTextAttribute(GetHandle(), color);
	BOOL ret = WriteConsole(GetHandle(), str, strlen(str), &dwNumberOfCharsWritten, NULL);
	SetConsoleCursorPosition(GetHandle(), m_csbi.dwCursorPosition);
	SetConsoleTextAttribute(GetHandle(), m_csbi.wAttributes);
	return ret;
}
BOOL Console::Printf(SHORT x, SHORT y, WORD color, const char *format, ...) {
	char outBuf[512];
	va_list arg;
	va_start(arg, format);
	vsprintf_s(outBuf, format, arg);
	va_end(arg);
	return Print(x, y, color, outBuf);
}