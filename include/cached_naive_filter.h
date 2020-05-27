#ifndef __CACHED_NAIVE_FILTER_H__
#define __CACHED_NAIVE_FILTER_H__


#include "naive_filter.h"
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <set>



class cached_naive_filter : public naive_filter{
private:
	using string = std::string;

	std::unordered_map< string, std::set< std::pair<std::string, std::string> > > white;


public:
	cached_naive_filter(std::shared_ptr<sql_parser> _sp) : naive_filter(_sp){
		cache();
	};
	virtual ~cached_naive_filter(){};

	virtual bool is_legal(const std::string &user, const std::string &_sql, const std::string &ip);


	void cache();
	
};


#endif