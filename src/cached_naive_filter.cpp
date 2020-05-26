#include "cached_naive_filter.h"
#include <string>

using namespace std;

void cached_naive_filter::cache(){
	string querySql = "select rule, user, ip, level, flag from white_list where flag=1";
	auto ans = sc.query(querySql);
	for(auto item : ans){
		int level = stoi(item[3]);
		int flag = stoi(item[4]);
		//user is 1<<4, ip is 1<<3
		string user = (level & 16) ? "all" : item[1];
		string ip = (level & 8) ? "all" : item[2];
		if(flag == 1){
			white[item[0]][user].insert(ip);
		}
	}
}

bool cached_naive_filter::is_legal(const std::string &user, const string &_sql, const string &ip){
	int res = false;
// auto start_time = std::chrono::steady_clock::now();
	vector<string> rule(sp->level_size());
	sp->parse_sql(_sql, rule);
// auto end_time = std::chrono::steady_clock::now();
// auto duration = std::chrono::duration_cast<chrono::microseconds>(end_time - start_time);
// std::cout << "The exec time parse_sql() is : "<< duration.count() << "ms" << std::endl;
	//if(!(query_is_illegal(user, rule, ip)) && query_is_legal(user, rule, ip))
	for(auto ele : rule){
		auto ruleIter = white.find(ele);
		if(ruleIter != white.end()){
			auto userIter = ruleIter->second.find(user);
			if(userIter != ruleIter->second.end()){
				return userIter->second.find(ip) != userIter->second.end();
			}
		}
	}
	return 0;
//	return query_is_legal(user, rule, ip);
}
