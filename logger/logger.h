#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <cstdio>
#include <string>
using namespace std;
extern int LOG_LEVEL;
class logger
{
public:
	void error(string msg);

	void warning(string msg);

	void info(string msg);

	void high_debug(string msg);

	void debug(string msg);

};

#endif