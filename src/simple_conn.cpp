#include "simple_conn.h"


simple_conn::simple_conn(const char server[],const char user[],const char passwd[],const char database[], unsigned int port){
	connect_to(server, user, passwd, database, port);
}

void simple_conn::connect_to(const char server[],const char user[],const char passwd[],const char database[], unsigned int port){

	conn = mysql_init(NULL);

	if (!mysql_real_connect(conn, server, user, passwd, database, port, NULL, CLIENT_MULTI_RESULTS)){
		fwLogger->warn("connect_to(), {}", mysql_error(conn));
	}
}

vector< vector<string> > simple_conn::query(const std::string &q){
	vector< vector<string> > result;
	if (conn == NULL){
		fwLogger->warn("query(), {}", mysql_error(conn));
		return result;
	}

	if (mysql_query(conn,q.c_str())){
		fwLogger->warn("mysql_query(), {}", mysql_error(conn));
		return result;
	}
	res = mysql_use_result(conn);
	if (res == NULL) return result;

	while ((row = mysql_fetch_row(res)) != NULL){
		vector<string> tmprow;
		int fields_num = mysql_num_fields(res);
		for (int i = 0;i<fields_num;i++){
			tmprow.push_back(row[i]);
		}
		result.push_back(tmprow);
	}
	mysql_free_result(res);
	return result;
}