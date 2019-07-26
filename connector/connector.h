#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "simple_conn.h"
#include "../config/config.h"
#include <vector>
#include <string>
using namespace std;


class connector
{
private:
	simple_conn sc;
	static const unsigned long long HASH_MOD = (1LL<<61)+7LL;

public:

	void connect_to_db();


	//bool query_rule(string user, string rule);
	//bool query_rule(string rule);
	bool query_rule_in_white_list(string user, string rule, string ip);
	bool query_rule_in_black_list(string user, string rule, string ip);

	bool query_sql(string user, string sql, int level, string ip);

	void insert_to_a_list(string user, string sql, int level, string ip, int flag);

	void remove_from_a_list(string user, string sql, string ip, int flag);

	//void add_white_list(string user, string sql, string rule);
	//void add_white_list(string _sql, string rule, int level);
	void add_white_list(string user, string _sql, string rule, int level, string ip);

	void add_illegal_query(string user, string sql, string ip);

	unsigned long long hash(string user, string sql);

	unsigned long long ELFhash(const char* key);

	void close();

};

#endif
