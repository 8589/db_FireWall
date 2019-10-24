#ifndef _BASE_SQL_PARSER_H_
#define _BASE_SQL_PARSER_H_

#include <FlexLexer.h>
#include <iostream> 
#include <string> 
#include <cstdio>
using namespace std;

class base_sql_parser:public yyFlexLexer
{
	char content[4096];
	int _now;
	int _end;
public:
	base_sql_parser(const char *sql, int _size);
	virtual int LexerInput(char* buf, int max_size);
	virtual void LexerError(const char* msg);
	virtual ~base_sql_parser() {}
};

#endif
