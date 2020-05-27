#include "cached_naive_filter.h"
#include <string>

using namespace std;

void cached_naive_filter::cache(){
	string querySql = "select rule, user, ip, level from white_list where flag=1";
	auto ans = sc.query(querySql);
	for(auto item : ans){
		int level = stoi(item[3]);
		//user is 1<<4, ip is 1<<3
		string user = (level & 16) ? "all" : item[1];
		string ip = (level & 8) ? "all" : item[2];
		white[item[0]].insert(make_pair(user, ip));
	}
}

bool cached_naive_filter::is_legal(const std::string &user, const string &_sql, const string &ip){
	int res = false;
	vector<string> rule(sp->level_size());
	sp->parse_sql(_sql, rule);
	for(auto ele : rule){
		auto ruleIter = white.find(ele);
		if(ruleIter != white.end() ){
			auto &st = ruleIter->second;
			if(st.find(make_pair(user, ip)) != st.end()|| st.find(make_pair("all", ip))  != st.end()||
				st.find(make_pair(user, "all"))  != st.end()|| st.find(make_pair("all", "all"))  != st.end() ){
				return 1;
			}
		}
	}
	return 0;
}
