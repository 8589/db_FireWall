#ifndef _FILTER_H_
#define _FILTER_H_

#include <string>
#include <memory>
#include "sql_parser.h"

using std::string;
using std::shared_ptr;

class filter
{
private:
protected:
	shared_ptr<sql_parser> sp;
public:
	filter(shared_ptr<sql_parser> _sp):sp(_sp){}

	virtual ~filter(){}

	virtual bool is_legal_and_add_log(const string &user, const string &_sql, const string &ip){return 1;}

	virtual bool is_legal(const string &user, const string &_sql, const string &ip){return 1;}

	virtual void log_illegal(const string &user, const string &_sql, const string &ip){}
	
	virtual void add_white_list(const string &user, const string &_sql, const string &ip){}

};


#endif
