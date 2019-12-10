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

	virtual bool is_legal_and_add_log(const string &user, const string &_sql, const string &ip);

	virtual void add_white_list(const string &user, const string &_sql, const string &ip);

	bool query_is_illegal(const string &user, vector<string>& rule, const string &ip);
	bool query_is_legal(const string &user, vector<string>& rule, const string &ip);

	void add_white_list(const string &user, const string &_sql, const string &rule, int level, const string &ip, int flag);
	void add_white_list(const string &user, int is_all_user, const string &_sql, const string &ip, int is_all_ip, int _default_level);


	void add_illegal_query(const string &user, const string &sql_cmd, const string &ip);


	//operation
	void parse(const string &_sql, std::vector<string> &v);
	void insert_to_db(const string &_sql, const string &user, int is_all_user, const string &ip, int is_all_ip);
	void remove_from_list(const string &_sql, int level, const string &user, const string &ip, int which_list);
	void delete_from_db(const string &_sql, const string &user, const string &ip);
	void add_to_list(const string &_sql, int level, const string &user, const string &ip, int which_list);
	void add_to_list2(const string &_sql, int level, const string &user, const string &ip, int which_list);

	static void init_db();



};

#endif
