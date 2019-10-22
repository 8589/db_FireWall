
%{
#include <cstdio>
#include <string>



using namespace std;
//operate word
#define CREATE 		1
#define DROP 		2
#define USE 		3
#define INSERT		4
#define SELECT 		5
#define UPDATE		6
#define DELETE		7
#define ALTER		8
#define SHOW		9

//num type
#define TINY		10
#define SMALLINT	11
#define MEDIUMINT	12
#define INT 		13
#define	INTEGER		14
#define	BIGINT		15
#define	FLOAT		16
#define	DOUBLE		17
#define DECIMAL		18

//data type
#define DATE 		19
#define TIME		20
#define YEAR		21
#define DATETIME	22
#define	TIMESTAMP	23

//string type
#define CHAR		24
#define	VARCHAR		25
#define TINYBLOB	26
#define TINYTEXT	27
#define BLOB		28
#define TEXT 		29
#define MEDIUMBLOB	30
#define	MEDIUMTEXT	31
#define LONGBLOB	32
#define LONGTEXT	33


//other
#define DATABASE 	34
#define DATABASES	35
#define TABLE		36
#define TABLES		37
#define COLUMNS		38


#define INTO		39
#define VALUES		40
#define FROM 		41
#define WHERE		42
#define LIMIT		43
#define OFFSET		44
#define GROUP		45
#define HAVING		46
#define ORDER		47
#define UNION		48
#define JOIN		49
#define MODIFY		50


#define SET 		51
#define LIKE		52
#define ALL			53
#define DISTINCT	54


#define ASC			55
#define DESC		56
#define IS 			57
#define NLL			58
#define NOT 		59
#define NLL_EQUAL	60


#define AND			61
#define OR 			62
#define _DEFAULT_	63


#define INNER		64
#define LEFT		65
#define RIGHT		66
#define BY			67
#define TO 			68
#define FIRST		69
#define AFTER		70
#define CHANGE		71
#define INDEX		72
#define ON			73
#define UNIQUE		74
#define PRIMARY		75
#define KEY			76
#define TEMPORARY	77
#define AUTO_INCREMENT	78
#define IN 			79


//operator
#define L_BRACKET	80
#define R_BRACKET	81
#define EMPTY		82
#define COMMA		83
#define STAR		84
#define EQUAL		85
#define NOT_EQUAL	86
#define MORE		87
#define LESS		88
#define MORE_EQUAL	89
#define LESS_EQUAL	90
#define PERCENT_MARK	91
#define UNDERLINE	92
#define L_MID_BRACKET	93
#define R_MID_BRACKET	94
#define XOR			95
#define ADD			96
#define DOLLAR		97
#define STICK		98
#define L_BIG_BRACKET	99
#define R_BIG_BRACKET	100


#define _BEGIN_		101
#define START		102
#define TRANSACTION	103
#define	COMMIT		104
#define WORK		105
#define ROLLBACK	106
#define SAVEPOINT	107
#define RELEASE		108
#define READ		109
#define UNCOMMITTED	110
#define COMMITTED	111
#define REPEATABLE	112
#define SERIALIZABLE	113
#define	AUTOCOMMIT	114
#define DIV			115
#define MOD 		116
#define BETWEEN		117
#define REGEXP		118
#define	RLIKE		119
#define L_MOVE		120
#define R_MOVE		121
#define THEN		122
#define CASE		123

#define NUMBER		124
#define ID 			125
#define FUNCTION	126
#define SEMICOLON	127
#define SUB			128
#define QUESTION_MARK	129
#define BACKSLASH	130
#define STRING 		131
#define DOT			132

#define IGNORE		133
#define DUPLICATE	134
#define WHEN		135
#define END			136
#define AS 			137
#define ELSE 		138
#define CONSTRAIN 	139
#define FOREIGN		140
#define REFERENCES 	141
#define CASCADE 	142
#define GRANT 		143
#define PRIVILEGES 	144
#define IDENTIFIED 	145
#define WITH 		146
#define OPTION 		147
#define IF 			148
#define EXISTS 		149
#define RENAME 		150
#define DESCRIBE 	151
#define EXPLAIN 	152
#define VIEW 		153
#define FLUSH 		154
#define FOR 		155
#define REVOKE 		156
#define AT 			157


#define TAB_DOT_COL	158
#define OUTER 		159

#define GRANTS 		160



#define OTHER 		999


%}


%option yylineno 
%option noyywrap 
%option c++

%%
[ \t\n\r]+						{return EMPTY;}
\"(\\\"|[^"])*\"				{return STRING;}
\'(\\\'|[^'])*\'				{return STRING;}
[.] 							{return DOT;}
(?i:CREATE)						{return CREATE;}
(?i:GRANTS)						{return GRANTS;}
(?i:DROP)						{return DROP;}
(?i:USE)						{return USE;}
(?i:INSERT)						{return INSERT;}
(?i:SELECT)						{return SELECT;}
(?i:UPDATE)						{return UPDATE;}
(?i:DELETE)						{return DELETE;}
(?i:ALTER)						{return ALTER;}
(?i:SHOW)						{return SHOW;}

(?i:TINY)						{return TINY;}
(?i:SMALLINT)					{return SMALLINT;}
(?i:MEDIUMINT)					{return MEDIUMINT;}
(?i:INT)						{return INT;}
(?i:INTEGER)					{return INTEGER;}
(?i:BIGINT)						{return BIGINT;}
(?i:FLOAT)						{return FLOAT;}
(?i:DOUBLE)						{return DOUBLE;}
(?i:DECIMAL)					{return DECIMAL;}

(?i:DATE)						{return DATE;}
(?i:TIME)						{return TIME;}
(?i:YEAR)						{return YEAR;}
(?i:DATETIME)					{return DATETIME;}
(?i:TIMESTAMP)					{return TIMESTAMP;}


(?i:CHAR)						{return CHAR;}
(?i:VARCHAR)					{return VARCHAR;}
(?i:TINYBLOB)					{return TINYBLOB;}
(?i:TINYTEXT)					{return TINYTEXT;}
(?i:BLOB)						{return BLOB;}
(?i:TEXT)						{return TEXT;}
(?i:MEDIUMBLOB)					{return MEDIUMBLOB;}
(?i:MEDIUMTEXT)					{return MEDIUMTEXT;}
(?i:LONGBLOB)					{return LONGBLOB;}
(?i:LONGTEXT)					{return LONGTEXT;}


(?i:DATABASE)					{return DATABASE;}
(?i:DATABASES)					{return DATABASES;}
(?i:TABLE)						{return TABLE;}
(?i:TABLES)						{return TABLES;}
(?i:COLUMNS)					{return COLUMNS;}

(?i:INTO)						{return INTO;}
(?i:VALUES)						{return VALUES;}
(?i:FROM)						{return FROM;}
(?i:WHERE)						{return WHERE;}
(?i:LIMIT)						{return LIMIT;}
(?i:OFFSET)						{return OFFSET;}
(?i:GROUP)						{return GROUP;}
(?i:HAVING)						{return HAVING;}
(?i:ORDER)						{return ORDER;}
(?i:UNION)						{return UNION;}
(?i:JOIN)						{return JOIN;}
(?i:MODIFY)						{return MODIFY;}

(?i:SET)						{return SET;}
(?i:LIKE)						{return LIKE;}
(?i:ALL)						{return ALL;}
(?i:DISTINCT)					{return DISTINCT;}


(?i:ASC)						{return ASC;}
(?i:DESC)						{return DESC;}
(?i:IS)							{return IS;}
(?i:NULL)						{return NLL;}
(?i:NOT)|("!")					{return NOT;}
(?i:NLL_EQUAL)|("<==>")			{return NLL_EQUAL;}

(?i:AND)						{return AND;}
(?i:OR)							{return OR;}
(?i:DEFAULT)					{return _DEFAULT_;}

(?i:INNER)						{return INNER;}
(?i:LEFT)						{return LEFT;}
(?i:RIGHT)						{return RIGHT;}
(?i:BY)							{return BY;}
(?i:TO)							{return TO;}
(?i:FIRST)						{return FIRST;}
(?i:AFTER)						{return AFTER;}
(?i:CHANGE)						{return CHANGE;}
(?i:INDEX)						{return INDEX;}
(?i:ON)							{return ON;}
(?i:UNIQUE)						{return UNIQUE;}
(?i:PRIMARY)					{return PRIMARY;}
(?i:KEY)						{return KEY;}
(?i:TEMPORARY)					{return TEMPORARY;}
(?i:AUTO_INCREMENT)				{return AUTO_INCREMENT;}
(?i:IN)							{return IN;}

(?i:IGNORE)						{return IGNORE;}
(?i:DUPLICATE)					{return DUPLICATE;}
(?i:WHEN)						{return WHEN;}
(?i:END)						{return END;}
(?i:AS)							{return AS;}
(?i:ELSE)						{return ELSE;}
(?i:CONSTRAIN)					{return CONSTRAIN;}
(?i:FOREIGN)					{return FOREIGN;}
(?i:REFERENCES)					{return REFERENCES;}
(?i:CASCADE)					{return CASCADE;}
(?i:GRANT)						{return GRANT;}
(?i:PRIVILEGES)					{return PRIVILEGES;}
(?i:IDENTIFIED)					{return IDENTIFIED;}
(?i:WITH)						{return WITH;}
(?i:OPTION)						{return OPTION;}
(?i:IF)							{return IF;}
(?i:EXISTS)						{return EXISTS;}
(?i:RENAME)						{return RENAME;}
(?i:DESCRIBE)					{return DESCRIBE;}
(?i:EXPLAIN)					{return EXPLAIN;}
(?i:VIEW)						{return VIEW;}
(?i:FLUSH)						{return FLUSH;}
(?i:FOR)						{return FOR;}
(?i:REVOKE)						{return REVOKE;}
(?i:OUTER)						{return OUTER;}
"@"								{return AT;}



"("								{return L_BRACKET;}
")"								{return R_BRACKET;}

","								{return COMMA;}
"*"								{return STAR;}
"="								{return EQUAL;}
("!=")|("<>")					{return NOT_EQUAL;}
">"								{return MORE;}
"<"								{return LESS;}
">="							{return MORE_EQUAL;}
"<="							{return LESS_EQUAL;}
"["								{return L_MID_BRACKET;}
"]"								{return R_MID_BRACKET;}
(?i:XOR)|("^")					{return XOR;}
(?i:ADD)|("+")					{return ADD;}
"$"								{return DOLLAR;}
"|"								{return STICK;}
"{"								{return L_BIG_BRACKET;}
"}"								{return R_BIG_BRACKET;}
"-"								{return SUB;}
"?"								{return QUESTION_MARK;}
"\\"							{return BACKSLASH;}

(?i:BEGIN)						{return _BEGIN_;}
(?i:START)						{return START;}
(?i:TRANSACTION)				{return TRANSACTION;}
(?i:COMMIT)						{return COMMIT;}
(?i:WORK)						{return WORK;}
(?i:ROLLBACK)					{return ROLLBACK;}
(?i:SAVEPOINT)					{return SAVEPOINT;}
(?i:RELEASE)					{return RELEASE;}
(?i:READ)						{return READ;}
(?i:UNCOMMITTED)				{return UNCOMMITTED;}
(?i:COMMITTED)					{return COMMITTED;}
(?i:REPEATABLE)					{return REPEATABLE;}
(?i:SERIALIZABLE)				{return SERIALIZABLE;}
(?i:AUTOCOMMIT)					{return AUTOCOMMIT;}
(?i:DIV)|("/")					{return DIV;}
(?i:MOD)|("%")					{return MOD;}
(?i:BETWEEN)					{return BETWEEN;}
(?i:REGEXP)						{return REGEXP;}
(?i:RLIKE)						{return RLIKE;}
"<<"							{return L_MOVE;}
">>"							{return R_MOVE;}
(?i:THEN)						{return THEN;}
(?i:CASE)						{return CASE;}
";"								{return SEMICOLON;}
[0-9]+|[0-9]+\.[0-9]*|[0-9]*\.[0-9]+							{return NUMBER;}
[a-zA-Z_][a-zA-Z0-9_]*"()"		{return FUNCTION;}
[a-zA-Z_][a-zA-Z0-9_]*\.[a-zA-Z_][a-zA-Z0-9_]* 	{return TAB_DOT_COL;}
[a-zA-Z_][a-zA-Z0-9_]*    		{return ID;}

.             		       		{return OTHER;}

%%
