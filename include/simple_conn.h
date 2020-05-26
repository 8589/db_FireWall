#ifndef _SIMPLE_CONN_H_
#define _SIMPLE_CONN_H_

#include <cstring>
#include <vector>
#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
#include "config.h"
#include "simple_comm.h"

class simple_conn{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

public:
	simple_conn(const char server[], const char user[], const char passwd[], const char database[], unsigned int port = 0);
	simple_conn(){}

	void close(){
		mysql_close(conn);
	}

	void connect_to(const char server[],const char user[],const char passwd[],const char database[], unsigned int port = 0);

	std::vector< std::vector<std::string> > query(const std::string &q);

};


#endif






