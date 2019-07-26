#ifndef _FILTER_H_
#define _FILTER_H_

#include "../connector/connector.h"
#include "../sql_parser/sql_parser.h"
#include <string>
#include <sstream>
using namespace std;



class filter
{
private:
	connector conn;
public:

	bool is_legal_and_add_log(string user, string _sql, string ip);
	//bool is_legal_and_add_log(string user, string _sql, string ip, bool flag);
	

	//void add_white_list(string user, string _sql);
	//void add_white_list(string _sql, int level);
	void add_white_list(string user, string _sql, int level, string ip);

	void add_white_list_n_times(string user, string _sql, int n, string ip);


	//void parse_sql(string _sql, string &sql, string &rule);
	void parse_sql(string _sql, int level, string &rule);


	void parse_sql_level_1(string _sql, string &rule);
	void parse_sql_level_2(string _sql, string &rule);
	void parse_sql_level_3(string _sql, string &rule);
	void parse_sql_level_4(string _sql, string &rule);

};

#endif