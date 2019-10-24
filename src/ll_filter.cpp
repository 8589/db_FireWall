#include "ll_filter.h"


/*
bool naive_filter::is_legal_and_add_log(string user, string _sql, string ip)
{
	string sql;
	int res = false;
	vector<string> rule(4, string());

	for(int i=1;i<5;i++)
	{
		this->parse_sql(_sql, i, rule[i-1]);
	}
	if((this->conn).query_is_legal(user, rule, ip))
	{
		return true;
	}
	(this->conn).add_illegal_query(user, _sql, ip);
	return false;
}


void naive_filter::add_white_list(string user, string _sql, string ip)
{
	string _sql_;
	this->parse_sql(_sql, 1, _sql_);
	string rule;
	for(int i=1;i<=4;i++){
		rule.clear();
		this->parse_sql(_sql, i, rule);
		(this->conn).add_white_list(user, _sql_, rule, i, ip);
	}
}
*/