#include "naive_sql_parser.h"



void naive_sql_parser::parse_sql(string _sql, int level, string &rule)
{
	if(level == 0)
		this->parse_sql_level_0(_sql, rule);
	else if(level == 1)
		this->parse_sql_level_1(_sql, rule);
	else if(level == 2)
		this->parse_sql_level_2(_sql, rule);
	else if(level == 3)
		this->parse_sql_level_3(_sql, rule);
	else 
		this->parse_sql_level_0(_sql, rule);
}

void naive_sql_parser::parse_sql(string _sql, vector<string> &rule)
{
	rule.clear();
	rule.resize(this->level_size());
	for(int i=0;i<this->level_size();i++)
	{
		this->parse_sql(_sql, i, rule[i]);
	}
}


void naive_sql_parser::parse_sql_level_0(string _sql, string &rule)
{
	rule.clear();
	shared_ptr<base_sql_parser> sp(new base_sql_parser(_sql.c_str(), _sql.size()));
	int ret;
	//stringstream ss;
	while((ret = sp->yylex()))
	{
		if(ret == EMPTY)
			rule += " ";			//ss << " " ;
		
		else if(ret == STRING)
		{
			string str(sp->YYText());
			string s;
			for(int i=0;i<str.size();i++)
			{
				if(str[i] == '\\')
					s += "\\\\";
				else if(str[i]=='\"')
					s +="\\\"";
				else
					s += str[i];
			}
			rule += s;
		}
		else
			rule += (sp->YYText());			//ss << (sp->YYText());
			
	}
	//ss >> rule;
}

void naive_sql_parser::parse_sql_level_1(string _sql, string &rule)
{
	rule.clear();
	shared_ptr<base_sql_parser> sp(new base_sql_parser(_sql.c_str(), _sql.size()));
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
	//ss >> rule;
}

void naive_sql_parser::parse_sql_level_2(string _sql, string &rule)
{
	rule.clear();
	shared_ptr<base_sql_parser> sp(new base_sql_parser(_sql.c_str(), _sql.size()));
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
	//ss >> rule;
}

void naive_sql_parser::parse_sql_level_3(string _sql, string &rule)
{
	rule.clear();
	shared_ptr<base_sql_parser> sp(new base_sql_parser(_sql.c_str(), _sql.size()));
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
	//ss >> rule;
}