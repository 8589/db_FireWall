#ifndef _NAIVE_FILTER_H_
#define _NAIVE_FILTER_H_

//#include "llheader.h"

#include "simple_conn.h"
#include "filter.h"
#include "connector.h"

using namespace std;

extern std::string db_user;
extern std::string db_password;
extern std::string db_name;
extern int default_level;

class naive_filter : public filter
{
private:
	simple_conn sc;

public:
	naive_filter(shared_ptr<sql_parser> _sp):filter(_sp){
		(this->sc).connect_to("127.0.0.1",db_user.c_str(),db_password.c_str(),db_name.c_str());
	}
	virtual ~naive_filter(){
		(this->sc).close();
	}

	virtual bool is_legal_and_add_log(string user, string _sql, string ip);

	virtual void add_white_list(string user, string _sql, string ip);

	bool query_is_legal(string user, vector<string>& rule, string ip);

	void add_white_list(string user, string _sql, string rule, int level, string ip);

	void add_illegal_query(string user, string sql, string ip);

	static void init_db();



};

#endif
