#include "base_sql_parser.h"
#include <string>
#include <cstring>
using namespace std;
base_sql_parser::base_sql_parser(const char *sql, int _size)
{
	memset(this->content, 0, sizeof(this->content));
	memcpy(this->content, sql, _size);
	content[_size] = '\0';
    this->_now = 0;
    this->_end = _size;
}
int base_sql_parser::LexerInput(char* buf, int max_size)
{
	if(this->_now == this->_end)	return 0;
	else if(this->_end - this->_now >= max_size - 1)
	{
		memcpy(buf, this->content+this->_now, max_size-1);
		buf[max_size-1] = '\0';
		this->_now += (max_size-1);
		return max_size-1;
	}else
	{
		int _len = this->_end - this->_now;
		memcpy(buf, this->content+this->_now, _len);
		buf[_len] = '\0';
		this->_now = this->_end;
		return _len;
	}
	return 0;
}
void base_sql_parser::LexerError(const char* msg)
{
	cout<<"error:"<< msg << " at line:" << lineno() <<endl;
}
