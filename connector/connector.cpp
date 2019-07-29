#include "connector.h"
#include <iostream>

using namespace std;



void connector::connect_to_db()
{
	(this->sc).connect_to("127.0.0.1","root","123456","LL_firewall");
}

/*
bool connector::query_rule(string user, string rule)
{
	//unsigned long long hash_code = this->hash(user.c_str(), rule.c_str());
	vector<vector<string> > ans;
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	//cout << hash_code << endl;
	sprintf(query_sql, "select user, rule from white_list where rule='%s';", rule.c_str());
	ans = (this->sc).query(query_sql);
	for(int i=0; i < ans.size();i++){
		if(ans[i][0] == user && ans[i][1] == rule){
			return true;
		}
	}
	return false;
}
bool connector::query_rule(string rule)
{
	vector<vector<string> > ans;
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	sprintf(query_sql, "select rule from white_list where rule=\"%s\";", rule.c_str());
	ans = (this->sc).query(query_sql);
	return ans.size();
}
*/

bool connector::query_rule_in_white_list(string user, string rule, string ip)
{
	vector<vector<string> > ans;
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	sprintf(query_sql, "select count(*) from white_list where rule=\"%s\" and ((level&4) or addr_ip=\"%s\") and ((level&8) or user=\"%s\") and (flag&1);", 
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
	sprintf(query_sql, "select count(*) from white_list where rule=\"%s\" and ((level&4) or addr_ip=\"%s\") and ((level&8) or user=\"%s\") and (flag&2);", 
		rule.c_str(),ip.c_str(),user.c_str());
	ans = (this->sc).query(query_sql);
	if(atoi(ans[0][0].c_str()))
		return true;
	return false;
}

bool connector::query_sql(string user, string sql, int level, string ip)
{
	vector<vector<string> > ans;
	char query_sql[BUFFSIZE];
	memset(query_sql, 0, sizeof(query_sql));
	sprintf(query_sql, "select count(*) from white_list where user=\"%s\" and sql_cmd=\"%s\" and (level&7)=%d and addr_ip=\"%s\";", 
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



/*
void connector::add_white_list(string user, string sql, string rule)
{
	//unsigned long long hash_code = this->hash(user.c_str(), rule.c_str());
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	sprintf(query_sql, "insert into white_list values('%s','%s','%s');",user.c_str(), sql.c_str(), rule.c_str());
	(this->sc).query(query_sql);
}

void connector::add_white_list(string _sql, string rule, int level)
{
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	sprintf(query_sql, "insert into white_list values(\"%s\",\"%s\", %d);",_sql.c_str(), rule.c_str(), level);
	(this->sc).query(query_sql);
}
*/
void connector::add_white_list(string user, string _sql, string rule, int level, string ip)
{
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	sprintf(query_sql, "insert into white_list values(\"%s\",\"%s\",\"%s\",%d,\"%s\",%d);",
		user.c_str(), _sql.c_str(), rule.c_str(), level, ip.c_str(), 0);
	//printf("###%s\n", query_sql);
	(this->sc).query(query_sql);
}


void connector::add_illegal_query(string user, string sql_cmd, string ip)
{
	char query_sql[BUFFSIZE];
	memset(query_sql,0,sizeof(query_sql));
	sprintf(query_sql, "insert into illegal_query(user, query_sql, addr_ip) values(\"%s\",\"%s\",\"%s\");",user.c_str(), sql_cmd.c_str(), ip.c_str());
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

