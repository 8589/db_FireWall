#include "sql_parser.h"
#include "FlexLexer.h"
#include <string>
#include <cstdio>
using namespace std;


int main()
{
	string sql = "INSERT INTO students (id, class_id, name, gender, score) VALUES (1, 1, 'aa', 'F', 99) ON DUPLICATE KEY UPDATE name='aa', gender='F', score=99;";
	cout << sql << endl;
	sql_parser *sp = new sql_parser(sql.c_str(), sql.size());
	int ret;
	while((ret = sp->yylex()))
		printf("%d %s\n", ret, sp->YYText());
	delete sp;
	return 0;
}



/*
int main()
{
	yyFlexLexer * yf = new yyFlexLexer();
	int ret;
	while((ret = yf->yylex()))
		printf("%d %s\n",ret, yf->YYText());
	delete yf;
	return 0;
}
*/