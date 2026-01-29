#pragma once

#include <iostream>

int getWarnState();
void resetWarnState();
int getQErrState();
void resetQErrState();

inline void logMsg()
{
	std::cerr << std::endl;
}

template<typename T>
inline void logMsg(T obj, bool noMarker = false)
{
	std::cerr << (noMarker ? "" : "LOG: ") << obj << "\n";
}

template<typename T>
inline void logMsg(const char* name, T obj, bool noMarker = false)
{
	std::cerr << (noMarker ? "" : "LOG: ") << name << " = " << obj << "\n";
}

void warn(const int warnCode, const char* warnText);
void quietErr(const int errCode, const char* errText);
void throwErr(const int errCode, const char* errText);