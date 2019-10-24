#ifndef _FILTER_H_
#define _FILTER_H_

#include <string>
#include <memory>
#include "sql_parser.h"

using namespace std;

class filter
{
private:
protected:
	shared_ptr<sql_parser> sp;
public:
	filter(shared_ptr<sql_parser> _sp):sp(_sp){}

	virtual ~filter(){}

	virtual bool is_legal_and_add_log(string user, string _sql, string ip){return 1;}
	
	virtual void add_white_list(string user, string _sql, string ip){}

};


#endif
