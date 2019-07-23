#include <string.h>
#include <vector>
#include <mysql/mysql.h>
#include <stdio.h>
#include <string>


using namespace std;
class simple_conn{
	char server[32];
	char user[32];
	char passwd[32];
	char database[32];
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
public:
	simple_conn(const char server[],const char user[],const char passwd[],const char database[]);
	simple_conn(){}
	void close(){
		mysql_close(conn);
	}

	void connect_to(const char server[],const char user[],const char passwd[],const char database[]);

	vector< vector<string> > query(const char q[]);
};









