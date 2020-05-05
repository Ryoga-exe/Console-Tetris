#pragma once

template <typename _T>
class Singleton {
protected:
	Singleton() {}
	virtual ~Singleton() {}
	Singleton(const Singleton& r) {}
	Singleton& operator=(const Singleton& r) {}
public:
	static _T* Instance() {
		static _T inst;
		return &inst;
	};
};