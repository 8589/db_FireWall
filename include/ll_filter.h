#ifndef _LLFILTER_H_
#define _LLFILTER_H_

//#include "llheader.h"

#include "connector.h"
#include "filter.h"

using namespace std;

/*
class llfilter : public filter
{
	
private:
	connector conn;
	
public:
	llfilter(shared_ptr<sql_parser> _sp):filter(_sp){
		(this->conn).connect_to_db();
	}
	

public:
	virtual ~llfilter(){
		
	}

	virtual bool is_legal_and_add_log(string user, string _sql, string ip);
	
	virtual void add_white_list(string user, string _sql, int level, string ip);

};

*/
#endif
