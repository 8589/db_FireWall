#ifndef _SQL_PARSER_H_
#define _SQL_PARSER_H_


#include <string>
#include <vector>


using namespace std;


class sql_parser
{
public:
	sql_parser(){};
	virtual ~sql_parser(){};

	virtual void parse_sql(string _sql, int level, string &rule){rule.clear();};

	virtual void parse_sql(string _sql, vector<string> &rule){rule.clear();};

	virtual int level_size(){return 1;}
	
};


#endif