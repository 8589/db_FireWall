#include "simple_conn.h"


simple_conn::simple_conn(const char server[],const char user[],const char passwd[],const char database[])
{
	strcpy(this->server,server);
	strcpy(this->user,user);
	strcpy(this->passwd,passwd);
	strcpy(this->database,database);

	conn = mysql_init(NULL);

	if (!mysql_real_connect(conn,server,user,passwd,database,0,NULL,CLIENT_MULTI_RESULTS)){
		//fprintf(stderr, "%s\n", mysql_error(conn));
		//exit(1);
		this->log.error(mysql_error(conn));
	}
}


void simple_conn::connect_to(const char server[],const char user[],const char passwd[],const char database[])
{
	strcpy(this->server,server);
	strcpy(this->user,user);
	strcpy(this->passwd,passwd);
	strcpy(this->database,database);

	conn = mysql_init(NULL);

	if (!mysql_real_connect(conn,server,user,passwd,database,0,NULL,CLIENT_MULTI_RESULTS))
	{
		//fprintf(stderr, "%s\n", mysql_error(conn));
		//exit(1);
		this->log.error(mysql_error(conn));
	}
}

vector< vector<string> > simple_conn::query(const char q[])
{
	vector< vector<string> > result;
	if (conn == NULL)
	{
		//printf("Cannot conncet to database!\n");
		//exit(1);
		this->log.error(q);
		this->log.error(mysql_error(conn));
	}

	if (mysql_query(conn,q))
	{
		//fprintf(stderr, "%s\n", mysql_error(conn));
		//exit(1);
		this->log.error(q);
		this->log.error(mysql_error(conn));
	}
	res = mysql_use_result(conn);
	if (res == NULL) return result;

	while ((row = mysql_fetch_row(res)) != NULL)
	{
		vector<string> tmprow;
		int fields_num = mysql_num_fields(res);
		for (int i = 0;i<fields_num;i++)
		{
			tmprow.push_back(row[i]);
		}
		result.push_back(tmprow);
	}
	mysql_free_result(res);
	return result;
}