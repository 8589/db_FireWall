#include "naive_filter.h"


bool naive_filter::is_legal_and_add_log(string user, string _sql, string ip)
{
	string sql;
	int res = false;
	vector<string> rule(this->sp->level_size(), string());
	this->sp->parse_sql(_sql, rule);
	if(this->query_is_legal(user, rule, ip))
	{
		return true;
	}
	this->add_illegal_query(user, _sql, ip);
	return false;
}

void naive_filter::add_white_list(string user, string _sql, string ip)
{
	string _sql_;
	this->sp->parse_sql(_sql, 0, _sql_);
	string rule;
	for(int i=0;i<this->sp->level_size();i++){
		rule.clear();
		this->sp->parse_sql(_sql, i, rule);
		this->add_white_list(user, _sql_, rule, i, ip);
	}
}

bool naive_filter::query_is_legal(string user, vector<string>& rule, string ip)
{
	vector<vector<string> > ans;
	string query_sql = "select count(*) from white_list where rule in (";
	for(int i=0;i<rule.size();i++)
	{
		if(i == rule.size()-1)
		{
			query_sql += "\"" + rule[i] + "\"";
		}
		else
		{
			query_sql += "\"" + rule[i] + "\",";
		}
	}
	query_sql += ") and ((level&8) or addr_ip=\"" + ip + "\") and ((level&16) or user=\"" + user + "\") and flag&1 and (not flag&2);";
	//printf("%s\n", query_sql.c_str());
	ans = (this->sc).query(query_sql.c_str());
	if(atoi(ans[0][0].c_str()))
		return true;
	return false;
}


void naive_filter::add_white_list(string user, string _sql, string rule, int level, string ip)
{
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	snprintf(query_sql, BUFFSIZE, "insert into white_list (user, sql_cmd, rule, level, addr_ip, flag) "
		"select \"%s\",\"%s\",\"%s\",%d,\"%s\",%d from DUAL where not exists("
		"select sql_cmd "
		"from white_list "
		"where user=\"%s\" and sql_cmd=\"%s\" and addr_ip=\"%s\" and level = %d);",
		user.c_str(), _sql.c_str(), rule.c_str(), level, ip.c_str(), (level==default_level)?1:0, user.c_str(), _sql.c_str(), ip.c_str(),level);
	(this->sc).query(query_sql);
}


void naive_filter::add_illegal_query(string user, string sql_cmd, string ip)
{
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	snprintf(query_sql, BUFFSIZE, "insert into illegal_query(user, query_sql, addr_ip) values(\"%s\",\"%s\",\"%s\");",user.c_str(), sql_cmd.c_str(), ip.c_str());
	(this->sc).query(query_sql);
}

void naive_filter::init_db()
{
	simple_conn ssc;
	ssc.connect_to("127.0.0.1",db_user.c_str(),db_password.c_str(),db_name.c_str());
	
	string sql_create_db = "create database if not exists LL_firewall;";
	ssc.query(sql_create_db.c_str());
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