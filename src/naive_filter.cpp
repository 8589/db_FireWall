#include "naive_filter.h"


bool naive_filter::is_legal_and_add_log(const string &user, const string &_sql, const string &ip)
{
	/*
	string sql;
	int res = false;
	vector<string> rule(this->sp->level_size(), string());
	this->sp->parse_sql(_sql, rule);
	//if(!(this->query_is_illegal(user, rule, ip)) && this->query_is_legal(user, rule, ip))
	if(this->query_is_legal(user, rule, ip))
	{
		return true;
	}
	this->add_illegal_query(user, _sql, ip);
	return false;
	*/
	if(is_legal(user, _sql, ip)){
		return 1;
	}
	log_illegal(user, _sql, ip);
	return 0;
}
 
bool naive_filter::is_legal(const string &user, const string &_sql, const string &ip){
	string sql;
	int res = false;
	vector<string> rule(this->sp->level_size(), string());
	this->sp->parse_sql(_sql, rule);
	//if(!(this->query_is_illegal(user, rule, ip)) && this->query_is_legal(user, rule, ip))
	return this->query_is_legal(user, rule, ip);
}

void naive_filter::log_illegal(const string &user, const string &_sql, const string &ip){
	this->add_illegal_query(user, _sql, ip);
}

bool naive_filter::query_is_illegal(const string &user, vector<string>& rule, const string &ip)
{
	vector<vector<string> > ans;
	string query_sql = "select id from white_list where rule in (";
	for(int i=0;i<rule.size();i++){
		if(i == rule.size()-1){
			query_sql += "\"" + rule[i] + "\"";
		}
		else{
			query_sql += "\"" + rule[i] + "\",";
		}
	}
	query_sql += ") and ((level&8) or addr_ip=\"" + ip + "\") and ((level&16) or user=\"" + user + "\") and flag&2 limit 1;";
	ans = (this->sc).query(query_sql.c_str());
	//if(atoi(ans[0][0].c_str()))
	if(ans.size()>0)
		return true;
	return false;
}

bool naive_filter::query_is_legal(const string &user, vector<string>& rule, const string &ip)
{
	vector<vector<string> > ans;
	string query_sql = "select id from white_list where rule in (";
	for(int i=0;i<rule.size();i++){
		if(i == rule.size()-1){
			query_sql += "\"" + rule[i] + "\"";
		}
		else{
			query_sql += "\"" + rule[i] + "\",";
		}
	}
	query_sql += ") and ((level&8) or addr_ip=\"" + ip + "\") and ((level&16) or user=\"" + user + "\") and flag&1 and (not flag&2) limit 1;";
	ans = (this->sc).query(query_sql.c_str());
	//if(atoi(ans[0][0].c_str()))
	if(ans.size()>0)
		return true;
	return false;
}


void naive_filter::add_white_list(const string &user, const string &_sql, const string &rule, int level, const string &ip, int flag)
{
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	snprintf(query_sql, BUFFSIZE, "insert into white_list (user, sql_cmd, rule, level, addr_ip, flag) "
		"select \"%s\",\"%s\",\"%s\",%d,\"%s\",%d from DUAL where not exists("
		"select sql_cmd "
		"from white_list "
		"where user=\"%s\" and sql_cmd=\"%s\" and addr_ip=\"%s\" and level = %d);",
		user.c_str(), _sql.c_str(), rule.c_str(), level, ip.c_str(), flag, user.c_str(), _sql.c_str(), ip.c_str(),level);
	(this->sc).query(query_sql);
}

void naive_filter::add_white_list(const string &user, int is_all_user, const string &_sql, const string &ip, int is_all_ip, int _default_level)
{
	string _sql_;
	this->sp->parse_sql(_sql, 0, _sql_);
	string rule;
	for(int i=0;i<this->sp->level_size();i++){
		rule.clear();
		this->sp->parse_sql(_sql, i, rule);
		this->add_white_list(user, _sql_, rule, i|(is_all_ip?8:0)|(is_all_user?16:0), ip, (i==_default_level)?1:0);
	}
}

void naive_filter::add_white_list(const string &user, const string &_sql, const string &ip)
{
	this->add_white_list(user, 0, _sql, ip, 0, default_level);
}


void naive_filter::add_illegal_query(const string &user, const string &sql_cmd, const string &ip)
{
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	snprintf(query_sql, BUFFSIZE, "insert into illegal_query(user, query_sql, addr_ip) values(\"%s\",\"%s\",\"%s\");",user.c_str(), sql_cmd.c_str(), ip.c_str());
	(this->sc).query(query_sql);
}


void naive_filter::parse(const string &_sql, std::vector<string> &v)
{
	v.clear();
	v.resize((this->sp)->level_size(), string());

	this->sp->parse_sql(_sql, v);

}
void naive_filter::insert_to_db(const string &_sql, const string &user, int is_all_user, const string &ip, int is_all_ip)
{
	this->add_white_list(user, is_all_user, _sql, ip, is_all_ip, -1);
}

void naive_filter::remove_from_list(const string &_sql, int level, const string &user, const string &ip, int which_list)
{
	string _sql_;
	this->sp->parse_sql(_sql, 0, _sql_);
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	snprintf(query_sql, BUFFSIZE, "update white_list set flag=flag^%d where sql_cmd=\"%s\" and user=\"%s\" and addr_ip=\"%s\" and level&7=%d;",
		(1<<(which_list)), _sql_.c_str(), user.c_str(), ip.c_str(), level);
	(this->sc).query(query_sql);
}

void naive_filter::delete_from_db(const string &_sql, const string &user, const string &ip)
{
	string _sql_;
	this->sp->parse_sql(_sql, 0, _sql_);
	string query_sql = "delete from white_list where sql_cmd=\"" + _sql_ +"\" and user=\"" + user + "\" and addr_ip=\"" + ip + "\";";
	//printf("%s\n", query_sql.c_str());
	(this->sc).query(query_sql.c_str());
}

void naive_filter::add_to_list(const string &_sql, int level, const string &user, const string &ip, int which_list)
{
	string _sql_;
	this->sp->parse_sql(_sql, 0, _sql_);
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	snprintf(query_sql, BUFFSIZE, "update white_list set flag=flag|%d where sql_cmd=\"%s\" and user=\"%s\" and addr_ip=\"%s\" and level&7=%d;",
		(1<<(which_list)), _sql_.c_str(), user.c_str(), ip.c_str(), level);
	(this->sc).query(query_sql);
}
void naive_filter::add_to_list2(const string &_sql, int level, const string &user, const string &ip, int which_list)
{
	string _sql_;
	this->sp->parse_sql(_sql, 0, _sql_);
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	snprintf(query_sql, BUFFSIZE, "update white_list set flag=flag|%d, level=level|%d where sql_cmd=\"%s\" and user=\"%s\" and addr_ip=\"%s\" and level&7=%d;",
		(1<<(which_list)), level&24, _sql_.c_str(), user.c_str(), ip.c_str(), level&7);
	(this->sc).query(query_sql);
}

void naive_filter::init_db()
{
	simple_conn ssc;
	ssc.connect_to("127.0.0.1",db_user.c_str(),db_password.c_str(),db_name.c_str());
	
	//string sql_create_db = "create database if not exists LL_firewall;";
	//ssc.query(sql_create_db.c_str());
	string sql_delete_white_list = "drop table if exists white_list;";
	ssc.query(sql_delete_white_list.c_str());
    string sql_create_white_list = "create table if not exists white_list ("
			"id int not null auto_increment primary key,"
			"user varchar(30) not null,"
			"sql_cmd varchar(2048) not null,"
			"rule varchar(2048) not null,"
			"level int not null,"
			"addr_ip varchar(30) not null,"
			"flag int not null,"
			"index(rule(512)),"
			"index(sql_cmd(512)));";
    ssc.query(sql_create_white_list.c_str());

    string sql_delete_illegal_query = "drop table if exists illegal_query;";
	ssc.query(sql_delete_illegal_query.c_str());
    string sql_create_illegal_query = "create table if not exists illegal_query("
			"id int not null auto_increment primary key,"
			"user varchar(30) not null, "
			"query_sql varchar(2048) not null," 
			"addr_ip varchar(30) not null, "
			"query_time datetime not null default now());";
	ssc.query(sql_create_illegal_query.c_str());
	ssc.close();
}
