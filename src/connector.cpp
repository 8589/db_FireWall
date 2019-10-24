#include "connector.h"
#include <iostream>

using namespace std;



void connector::connect_to_db()
{
	(this->sc).connect_to("127.0.0.1",db_user.c_str(),db_password.c_str(),db_name.c_str());
}

/*
bool connector::query_rule_in_white_list(string user, string rule, string ip)
{
	vector<vector<string> > ans;
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	sprintf(query_sql, "select count(*) from white_list where rule=\"%s\" and ((level&8) or addr_ip=\"%s\") and ((level&16) or user=\"%s\") and (flag&1);", 
		rule.c_str(),ip.c_str(),user.c_str());
	ans = (this->sc).query(query_sql);
	if(atoi(ans[0][0].c_str()))
		return true;
	return false;
}
bool connector::query_rule_in_black_list(string user, string rule, string ip)
{
	vector<vector<string> > ans;
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	sprintf(query_sql, "select count(*) from white_list where rule=\"%s\" and ((level&8) or addr_ip=\"%s\") and ((level&16) or user=\"%s\") and (flag&2);", 
		rule.c_str(),ip.c_str(),user.c_str());
	ans = (this->sc).query(query_sql);
	if(atoi(ans[0][0].c_str()))
		return true;
	return false;
}
*/

bool connector::query_sql(string user, string sql, int level, string ip)
{
	vector<vector<string> > ans;
	char query_sql[BUFFSIZE];
	memset(query_sql, 0, sizeof(query_sql));
	sprintf(query_sql, "select count(*) from white_list where sql_cmd=\"%s\" and user=\"%s\" and (level&7)=%d and addr_ip=\"%s\";", 
		user.c_str(), sql.c_str(), level, ip.c_str());
	//printf("@@@%s\n", query_sql);
	ans = (this->sc).query(query_sql);
	return atoi(ans[0][0].c_str());
}



//flag=1 insert to white list
//flag=2 insert to black list
void connector::insert_to_a_list(string user, string sql, int level, string ip, int flag)
{
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	sprintf(query_sql, "update white_list set level=%d, flag=flag|%d where sql_cmd=\"%s\" and level&7=%d&7 and user=\"%s\" and addr_ip=\"%s\";",
		level, flag, sql.c_str(), level, user.c_str(), ip.c_str());
	this->remove_from_a_list(user, sql, ip, flag);
	//printf("%s\n", query_sql);
	(this->sc).query(query_sql);
}


//flag=1 remove from white list
//flag=2 remove from black list
void connector::remove_from_a_list(string user, string sql, string ip, int flag)
{
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	sprintf(query_sql, "update white_list set flag=flag&%d where sql_cmd=\"%s\" and user=\"%s\" and addr_ip=\"%s\";",
		3-flag, sql.c_str(), user.c_str(), ip.c_str());
	//printf("%s\n", query_sql);
	(this->sc).query(query_sql);
}




unsigned long long connector::hash(string user, string sql)
{
	return this->ELFhash(user.c_str()) + this->ELFhash(sql.c_str());
}

unsigned long long connector::ELFhash(const char* key)
{
	unsigned long long h=0;
    while(*key)
    {
        h = (h << 4) + *key++;
        unsigned long long g = h & 0xF000000000000000LL;
        if(g)
            h ^= g >> 24; 
        h &= ~g; 
    }
    return h % HASH_MOD;
}

void connector::close()
{
	(this->sc).close();
}

