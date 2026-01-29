#include "logging.hpp"

static int warnState = 0;
static int qErrState = 0;

static void setWarnState(int state)
{
	warnState = state;
}

int getWarnState()
{
	return warnState;
}

void resetWarnState()
{
	warnState = NULL;
}

static void setQErrState(int state)
{
	qErrState = state;
}

int getQErrState()
{
	return qErrState;
}

void resetQErrState()
{
	qErrState = NULL;
}

void warn(const int warnCode, const char* warnText)
{
	std::cerr << "\u001b[33m" << "WARN" << warnCode << ": " << warnText << "\u001b[37m" << "\n";
	setWarnState(warnCode);
}

void quietErr(const int errCode, const char* errText)
{
	std::cerr << "\u001b[35m" << "QERR" << errCode << ": " << errText << "\u001b[37m" << "\n";
	setQErrState(errCode);
}

void throwErr(const int errCode, const char* errText)
{
	std::cerr << "\u001b[31m" << "ERR" << errCode << ": " << errText << "\u001b[37m" << "\n";
	throw errCode;
}