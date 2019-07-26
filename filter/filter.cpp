#include "filter.h"
/*
bool filter::is_legal_and_add_log(string user, string _sql, string ip, bool flag)
{
	string sql, rule;
	this->parse_sql(_sql, sql, rule);
	bool result = false;
	(this->conn).connect_to_db();
	if((this->conn).query_rule(user, rule))
		result = true;
	else
		(this->conn).add_illegal_query(user, _sql, ip);
	(this->conn).close();
	return result;
}
*/
bool filter::is_legal_and_add_log(string user, string _sql, string ip)
{
	string sql, rule;
	for(int i=1;i<5;i++)
	{
		this->parse_sql(_sql, i, rule);	
		(this->conn).connect_to_db();
		if((this->conn).query_rule_in_black_list(user, rule, ip))
		{
			(this->conn).add_illegal_query(user, _sql, ip);
			(this->conn).close();
			return false;	
		}
		if((this->conn).query_rule_in_white_list(user, rule, ip))
			return true;
	}
	(this->conn).add_illegal_query(user, _sql, ip);
	(this->conn).close();
	return false;	
}


/*
void filter::add_white_list(string user, string _sql)
{
	string sql, rule;
	this->parse_sql(_sql, sql, rule);
	(this->conn).connect_to_db();
	if((this->conn).query_rule(user, rule))
		return;
	(this->conn).add_white_list(user, sql, rule);
	(this->conn).close();
}
void filter::add_white_list(string _sql, int level)
{
	string rule;
	this->parse_sql(_sql, level, rule);
	//printf("%s\n", rule.c_str());
	(this->conn).connect_to_db();
	if((this->conn).query_rule(rule))
		return;
	(this->conn).add_white_list(_sql, rule, level);
	(this->conn).close();
}
*/
void filter::add_white_list(string user, string _sql, int level, string ip)
{
	(this->conn).connect_to_db();
	if((this->conn).query_sql(user, _sql, level, ip))
	{
		(this->conn).close();
		return;
	}
	string rule;
	this->parse_sql(_sql, level, rule);
	//if the sql just is a table name or column name
	if(rule == "col" || rule == "tab")
	{
		(this->conn).close();
		return;
	}
	(this->conn).add_white_list(user, _sql, rule, level, ip);
	(this->conn).close();
}



void filter::add_white_list_n_times(string user, string _sql, int n, string ip)
{
	for(int i=1; i<=n; i++)
	{
		this->add_white_list(user, _sql, i, ip);
	}
}



/*
void filter::parse_sql(string _sql, string &sql, string &rule)
{
	sql.clear();
	rule.clear();
	sql_parser *sp = new sql_parser(_sql.c_str(), _sql.size());
	int ret;
	stringstream ss;
	while((ret = sp->yylex()))
	{
		if(ret != EMPTY)
			ss << ret;
		if(ret == NUMBER)	sql += "number";
		else if(ret == ID)		sql += "id";
		else if(ret == FUNCTION)	sql += "func";
		else if(ret == EMPTY)		sql +=	" " ;
		else			sql += sp->YYText();
		//printf("%d %s\n", ret, sp->YYText());
	}
		
	delete sp;
	//string sql;;
	ss >> rule;
	//return sql;
}
*/




void filter::parse_sql(string _sql, int level, string &rule)
{
	if(level == 1)
		this->parse_sql_level_1(_sql, rule);
	else if(level == 2)
		this->parse_sql_level_2(_sql, rule);
	else if(level == 3)
		this->parse_sql_level_3(_sql, rule);
	else if(level == 4)
		this->parse_sql_level_4(_sql, rule);
	else 
		this->parse_sql_level_1(_sql, rule);
}


void filter::parse_sql_level_1(string _sql, string &rule)
{
	rule.clear();
	sql_parser *sp = new sql_parser(_sql.c_str(), _sql.size());
	int ret;
	//stringstream ss;
	while((ret = sp->yylex()))
	{
		if(ret == EMPTY)
			rule += " ";			//ss << " " ;
			
		else
			rule += (sp->YYText());			//ss << (sp->YYText());
			
	}
	delete sp;
	//ss >> rule;
}

void filter::parse_sql_level_2(string _sql, string &rule)
{
	rule.clear();
	sql_parser *sp = new sql_parser(_sql.c_str(), _sql.size());
	int ret;
	//stringstream ss;
	while((ret = sp->yylex()))
	{
		if(ret == EMPTY)			rule += " ";	//ss << " ";
		else if(ret == NUMBER)		rule += "num";	//ss << "num";
		else if(ret == STRING) 		rule += "str";	//ss << "str";
		else
			rule +=(sp->YYText());		//ss << (sp->YYText());
	}
	delete sp;
	//ss >> rule;
}

void filter::parse_sql_level_3(string _sql, string &rule)
{
	rule.clear();
	sql_parser *sp = new sql_parser(_sql.c_str(), _sql.size());
	int ret;
	int last = -1;
	int llast = -1;
	//stringstream ss;
	while((ret = sp->yylex()))
	{
		if(ret == EMPTY)			rule += " ";	//ss << " ";
		else if(ret == NUMBER)		rule += "num";	//ss << "num";
		else if(ret == STRING) 		rule += "str";	//ss << "str";
		else if(ret == ID)
		{
			if(last==DESC || last==TABLE || last==FROM || last==INTO || last==UPDATE || last==ON || last==-1 || last==VIEW || last==REFERENCES
				||(last==COMMA && llast==TAB) || last == JOIN || (last==ID&&llast==TAB))
			{
				rule +=(sp->YYText());				//ss << (sp->YYText());
				llast = TAB;
			}
			else
			{
				rule += "col";					//ss << "col";
				llast = COL;
			}
		}
		else if(ret == STAR)
		{
			if(last == SELECT)
			{
				rule += "col";					//ss << "col";
				llast = COL;
			}
			else
				rule +=(sp->YYText());				//ss << (sp->YYText());
		}
		else if(ret == TAB_DOT_COL)
		{
			string _str(sp->YYText());
			int i = 0;
			for(i=0;i<_str.size();i++)
				if(_str[i] == '.')
					break;
			rule += string(_str, 0, i);
			rule += ".col";
		}
		else
			rule +=(sp->YYText());				//ss << (sp->YYText());
		if(ret != EMPTY)
			last = ret;
	}
	delete sp;
	//ss >> rule;
}

void filter::parse_sql_level_4(string _sql, string &rule)
{
	rule.clear();
	sql_parser *sp = new sql_parser(_sql.c_str(), _sql.size());
	int ret;
	int last = -1;
	int llast = -1;
	//stringstream ss;
	while((ret = sp->yylex()))
	{
		if(ret == EMPTY)			rule += " ";	//ss << " ";
		else if(ret == NUMBER)		rule += "num";	//ss << "num";
		else if(ret == STRING) 		rule += "str";	//ss << "str";
		else if(ret == ID)
		{
			if(last==DESC || last==TABLE || last==FROM || last==INTO || last==UPDATE || last==ON || last==-1 || last==VIEW || last==REFERENCES
				|| (last==COMMA && llast==TAB) || last == JOIN || (last==ID&&llast==TAB))
			{
				rule += "tab";					// ss << "tab";
				llast = TAB;
			}
			else
			{
				rule += "col";					//ss << "col";
				llast = COL;
			}
		}
		else if(ret == STAR)
		{
			if(last == SELECT)
			{
				rule += "col";					//ss << "col";
				llast = COL;
			}
			else
				rule +=(sp->YYText());				//ss << (sp->YYText());
		}
		else if(ret == TAB_DOT_COL)
		{
			rule += "tab.col";
		}
		else
			rule +=(sp->YYText());				//ss << (sp->YYText());
		if(ret != EMPTY)
			last = ret;
	}
	delete sp;
	//ss >> rule;
}