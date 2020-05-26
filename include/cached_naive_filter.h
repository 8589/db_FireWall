#ifndef __CACHED_NAIVE_FILTER_H__
#define __CACHED_NAIVE_FILTER_H__


#include "naive_filter.h"
#include <map>
#include <string>
#include <set>


class cached_naive_filter : public naive_filter{
private:

	std::map<std::string, std::map<std::string, std::set<std::string> > > white;
	//std::map<std::string, std::map<std::string, std::set<std::string> > > black;


public:
	cached_naive_filter(shared_ptr<sql_parser> _sp) : naive_filter(_sp){
		cache();
	};
	virtual ~cached_naive_filter(){};

	virtual bool is_legal(const std::string &user, const string &_sql, const string &ip);


	void cache();
	
};


#endif