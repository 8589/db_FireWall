#include "naive_filter.h"

#include <spdlog/fmt/fmt.h>
#include <iostream>

bool naive_filter::is_legal_and_add_log(const string &user, const string &_sql, const string &ip)
{
	if(is_legal(user, _sql, ip)){
		return 1;
	}
	log_illegal(user, _sql, ip);
	return 0;
}

bool naive_filter::is_legal(const string &user, const string &_sql, const string &ip){
	int res = false;
	vector<string> rule(sp->level_size());
	sp->parse_sql(_sql, rule);
	
	//if(!(query_is_illegal(user, rule, ip)) && query_is_legal(user, rule, ip))
	return query_is_legal(user, rule, ip);
}

void naive_filter::log_illegal(const string &user, const string &_sql, const string &ip){
	this->add_illegal_query(user, _sql, ip);
}



bool naive_filter::query_is_illegal(const string &user, vector<string>& rule, const string &ip)
{
	vector< vector<string> > ans;
	string query_sql = fmt::format(
		"select id from white_list where rule in (\"{}\",\"{}\",\"{}\",\"{}\")"
		"and ((level&8) or addr_ip=\"{}\") and ((level&16) or user=\"{}\") and flag&2 limit 1;",
		rule[0], rule[1], rule[2], rule[3], ip, user
		);
	fwLogger->debug(query_sql);
	ans = sc.query(query_sql.c_str());
	if(ans.size() > 0)
		return true;
	return false;
}

bool naive_filter::query_is_legal(const string &user, vector<string>& rule, const string &ip)
{
	vector< vector<string> > ans;
	string query_sql = fmt::format(
		"select id from white_list where rule in (\"{}\",\"{}\",\"{}\",\"{}\")"
		"and ((level&8) or addr_ip=\"{}\") and ((level&16) or user=\"{}\") and flag=1 limit 1;",
		rule[0], rule[1], rule[2], rule[3], ip, user
		);


	fwLogger->debug(query_sql);
	ans = sc.query(query_sql.c_str());
	if(ans.size() > 0)
		return true;
	return false;
}


void naive_filter::add_white_list(const string &user, const string &_sql, const string &rule, int level, const string &ip, int flag)
{
	string query_sql = fmt::format(
		"insert into white_list (user, sql_cmd, rule, level, addr_ip, flag)"
		"select \"{}\",\"{}\",\"{}\",{},\"{}\",{} from DUAL where not exists("
		"select sql_cmd from white_list "
		"where user=\"{}\" and sql_cmd=\"{}\" and addr_ip=\"{}\" and level = {});",
		user, _sql, rule, level, ip, flag, user, _sql, ip, level
		);
	fwLogger->debug("add_white_list : {}", query_sql);
	sc.query(query_sql);
}

void naive_filter::add_white_list(const string &user, int is_all_user, const string &_sql, const string &ip, int is_all_ip, int _default_level)
{
	string _sql_;
	sp->parse_sql(_sql, 0, _sql_);
	string rule;
	for(int i=0;i<this->sp->level_size();i++){
		rule.clear();
		sp->parse_sql(_sql, i, rule);
		add_white_list(user, _sql_, rule, i|(is_all_ip?8:0)|(is_all_user?16:0), ip, (i==_default_level)?1:0);
	}
}

void naive_filter::add_white_list(const string &user, const string &_sql, const string &ip)
{
	add_white_list(user, 0, _sql, ip, 0, default_level);
}


void naive_filter::add_illegal_query(const string &user, const string &sql_cmd, const string &ip)
{
	std::string query_sql = fmt::format(
		"insert into illegal_query(user, query_sql, addr_ip) values(\"{}\",\"{}\",\"{}\");",
		user, sql_cmd, ip
		);
	sc.query(query_sql);
}


void naive_filter::parse(const string &_sql, std::vector<string> &v)
{
	v.clear();
	v.resize(sp->level_size(), string());
	sp->parse_sql(_sql, v);

}
void naive_filter::insert_to_db(const string &_sql, const string &user, int is_all_user, const string &ip, int is_all_ip)
{
	this->add_white_list(user, is_all_user, _sql, ip, is_all_ip, -1);
}

void naive_filter::remove_from_list(const string &_sql, int level, const string &user, const string &ip, int which_list)
{
	string _sql_;
	this->sp->parse_sql(_sql, 0, _sql_);

	std::string query_sql = fmt::format(
		"update white_list set flag=flag^{} where sql_cmd=\"{}\" and user=\"{}\" and addr_ip=\"{}\" and level&7={};",
		1<<which_list, _sql_, user, ip, level
		);
	sc.query(query_sql);
}

void naive_filter::delete_from_db(const string &_sql, const string &user, const string &ip)
{
	string _sql_;
	sp->parse_sql(_sql, 0, _sql_);
	std::string query_sql = fmt::format(
		"delete from white_list where sql_cmd=\"{}\" and user=\"{}\" and addr_ip=\"{}\";",
		_sql_, user, ip
		);
	sc.query(query_sql.c_str());
}

void naive_filter::add_to_list(const string &_sql, int level, const string &user, const string &ip, int which_list)
{
	string _sql_;
	this->sp->parse_sql(_sql, 0, _sql_);

	std::string query_sql = fmt::format(
		"update white_list set flag=flag|{} where sql_cmd=\"{}\" and user=\"{}\" and addr_ip=\"{}\" and level&7={};",
		1<<which_list, _sql_, user, ip, level
		);

	sc.query(query_sql);
}
void naive_filter::add_to_list2(const string &_sql, int level, const string &user, const string &ip, int which_list)
{
	string _sql_;
	this->sp->parse_sql(_sql, 0, _sql_);

	std::string query_sql = fmt::format(
		"update white_list set flag=flag|{}, level=level|{} where sql_cmd=\"{}\" and user=\"{}\" and addr_ip=\"{}\" and level&7={};",
		1<<which_list, level&24, _sql_, user, ip, level&7
		);

	sc.query(query_sql);
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
