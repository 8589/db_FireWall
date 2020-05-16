#include "naive_sql_parser.h"

#define NUM_SIG		"n\1um"
#define STR_SIG		"s\1tr"
#define COL_SIG		"c\1ol"
#define TAB_SIG		"t\1ab"

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
		}else if(ret == QUOTE){
			rule += "\\\"";
		}else if(ret == BACKSLASH){
			rule += "\\\\";
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
		else if(ret == NUMBER)		rule += NUM_SIG;	//ss << "num";
		else if(ret == STRING) 		rule += STR_SIG;	//ss << "str";
		else if(ret == QUOTE){
			rule += "\\\"";
		}else if(ret == BACKSLASH){
			rule += "\\\\";
		}else
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
		else if(ret == NUMBER)		rule += NUM_SIG;	//ss << "num";
		else if(ret == STRING) 		rule += STR_SIG;	//ss << "str";
		else if(ret == QUOTE){
			rule += "\\\"";
		}else if(ret == BACKSLASH){
			rule += "\\\\";
		}else if(ret == ID)
		{
			if(last==DESC || last==TABLE || last==FROM || last==INTO || last==UPDATE || last==ON || last==-1 || last==VIEW || last==REFERENCES
				||(last==COMMA && llast==TAB) || last == JOIN || (last==ID&&llast==TAB))
			{
				rule +=(sp->YYText());				//ss << (sp->YYText());
				llast = TAB;
			}
			else
			{
				rule += COL_SIG;					//ss << "col";
				llast = COL;
			}
		}
		else if(ret == STAR)
		{
			if(last == SELECT)
			{
				rule += COL_SIG;					//ss << "col";
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
		else if(ret == NUMBER)		rule += NUM_SIG;	//ss << "num";
		else if(ret == STRING) 		rule += STR_SIG;	//ss << "str";
		else if(ret == QUOTE){
			rule += "\\\"";
		}else if(ret == BACKSLASH){
			rule += "\\\\";
		}else if(ret == ID)
		{
			if(last==DESC || last==TABLE || last==FROM || last==INTO || last==UPDATE || last==ON || last==-1 || last==VIEW || last==REFERENCES
				|| (last==COMMA && llast==TAB) || last == JOIN || (last==ID&&llast==TAB))
			{
				rule += TAB_SIG;					// ss << "tab";
				llast = TAB;
			}
			else
			{
				rule += COL_SIG;					//ss << "col";
				llast = COL;
			}
		}
		else if(ret == STAR)
		{
			if(last == SELECT)
			{
				rule += COL_SIG;					//ss << "col";
				llast = COL;
			}
			else
				rule +=(sp->YYText());				//ss << (sp->YYText());
		}
		else if(ret == TAB_DOT_COL)
		{
			rule += string(TAB_SIG)+"."+COL_SIG;		//"tab.col";
		}
		else
			rule +=(sp->YYText());				//ss << (sp->YYText());
		if(ret != EMPTY)
			last = ret;
	}
	//ss >> rule;
}
