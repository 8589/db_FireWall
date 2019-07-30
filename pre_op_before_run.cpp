#include "./connector/simple_conn.h"
#include <string>
#include "./utils/CJsonObject.hpp"
#include <fstream>

using namespace std;
using namespace neb;

extern std::string db_user;
extern std::string db_password;
extern std::string db_name;


int main()
{
	simple_conn s_conn;
	
	ifstream fin("/etc/db_FireWall/db_FireWall.json");
	string json;
	string _s;
	while(getline(fin, _s))
	{
		json += _s;
	}
	CJsonObject oJson(json);
	oJson.Get("db_user", db_user);
	oJson.Get("db_password", db_password);
	oJson.Get("db_name",db_name);

	string sql_create_db = "create database if not exists LL_firewall;";
	string sql_create_white_list = "create table if not exists white_list("
		"user varchar(30) not null,"
		"sql_cmd varchar(512) not null,"
		"rule varchar(512) not null,"
		"level int(5) not null,"
		"addr_ip varchar(30) not null,"
		"flag int(5) not null,"
		"primary key(user, sql_cmd, level, addr_ip));";
	string sql_create_illegal_query = "create table if not exists illegal_query("
		"user varchar(30) not null,"
		"query_sql varchar(512) not null,"
		"addr_ip varchar(30) not null,"
		"query_time datetime not null default now(),"
		"primary key(user,query_sql,addr_ip,query_time));";

	s_conn.connect_to("127.0.0.1", db_user.c_str(), db_password.c_str(),"");						
	std::vector<std::vector<string> > ans;
	s_conn.query(sql_create_db.c_str());
	s_conn.close();
	s_conn.connect_to("127.0.0.1", db_user.c_str(), db_password.c_str(), db_name.c_str());
	s_conn.query(sql_create_white_list.c_str());
	s_conn.query(sql_create_illegal_query.c_str());
	s_conn.close();
}