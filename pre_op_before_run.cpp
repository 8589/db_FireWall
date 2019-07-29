#include "./connector/simple_conn.h"


int main()
{
	simple_conn s_conn;
	
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

	s_conn.connect_to("127.0.0.1", "root", "123456","");						
	std::vector<std::vector<string> > ans;
	s_conn.query(sql_create_db.c_str());
	s_conn.close();
	s_conn.connect_to("127.0.0.1", "root", "123456","LL_firewall");	
	s_conn.query(sql_create_white_list.c_str());
	s_conn.query(sql_create_illegal_query.c_str());
	s_conn.close();
}