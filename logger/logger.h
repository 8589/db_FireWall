#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <cstdio>
#include <string>
using namespace std;
#define LOG_LEVEL 2
class logger
{
public:
	void error(string msg);

	void warning(string msg);

	void high_debug(string msg);

	void debug(string msg);

	

};

#endif