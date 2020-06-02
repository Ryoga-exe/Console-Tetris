#pragma once
#include <windows.h>

class Timer {
public:
	Timer() {
		m_startCount = 0;
		m_isPause = false;
		QueryPerformanceFrequency(&m_unionCounter);
		m_frequency = m_unionCounter.QuadPart / 1000;
		Start();
	}
	void Start() {
		m_isPause = false;
		QueryPerformanceCounter(&m_unionCounter);
		m_startCount = (ULONGLONG)m_unionCounter.QuadPart;
	}
	ULONGLONG Elapse() {
		if (m_isPause) return (m_pauseCount - m_startCount) / m_frequency;
		QueryPerformanceCounter(&m_unionCounter);
		return ((ULONGLONG)(m_unionCounter.QuadPart) - m_startCount) / m_frequency;
	}
	ULONGLONG GetStartTime() {
		return m_startCount;
	}
	bool Pause() {
		if (m_isPause) return true;
		QueryPerformanceCounter(&m_unionCounter);
		m_pauseCount = (ULONGLONG)m_unionCounter.QuadPart;
		m_isPause = true;
		return false;
	}
	bool Resume() {
		if (!m_isPause) return true;
		QueryPerformanceCounter(&m_unionCounter);
		m_startCount = (ULONGLONG)m_unionCounter.QuadPart - (m_pauseCount - m_startCount);
		m_isPause = false;
		return false;
	}
	bool SwitchPause() {
		if (m_isPause) {
			Resume();
			return false;
		}
		Pause();
		return true;
	}

private:
	ULONGLONG		m_startCount, m_frequency, m_pauseCount;
	LARGE_INTEGER	m_unionCounter;
	bool m_isPause;
};