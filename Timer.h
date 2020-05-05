#pragma once
#include <windows.h>

class Timer {
public:
	Timer() {
		m_startCount = 0;
		QueryPerformanceFrequency(&m_unionCounter);
		m_frequency = m_unionCounter.QuadPart / 1000;
		Start();
	}
	void Start() {
		QueryPerformanceCounter(&m_unionCounter);
		m_startCount = (ULONGLONG)m_unionCounter.QuadPart;
	}
	ULONGLONG Elapse() {
		QueryPerformanceCounter(&m_unionCounter);
		return ((ULONGLONG)(m_unionCounter.QuadPart) - m_startCount) / m_frequency;
	}
	ULONGLONG GetStartTime() {
		return m_startCount;
	}

private:
	ULONGLONG		m_startCount, m_frequency;
	LARGE_INTEGER	m_unionCounter;
};