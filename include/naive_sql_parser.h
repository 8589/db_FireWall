#ifndef _NAIVE_SQL_PARSER_H_
#define _NAIVE_SQL_PARSER_H_

#include "base_sql_parser.h"
#include "sql_parser.h"
#include "config.h"
#include <memory>
#include <vector>


using namespace std;

class naive_sql_parser : public sql_parser
{
private:
public:
	naive_sql_parser():sql_parser(){
		
	};
	virtual~naive_sql_parser(){

	};

	virtual void parse_sql(string _sql, int level, string &rule);

	virtual void parse_sql(string _sql, vector<string> &rule);

	virtual int level_size(){return 4;}


	void parse_sql_level_0(string _sql, string &rule);
	void parse_sql_level_1(string _sql, string &rule);
	void parse_sql_level_2(string _sql, string &rule);
	void parse_sql_level_3(string _sql, string &rule);
	
};


#endif