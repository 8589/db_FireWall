#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <atomic>



extern std::atomic<bool> is_learning;



#define	MAX_MSG_SIZE 	16777215
#define MSGSIZE 		1024
#define BUFFSIZE		4096


#define COL 			2001
#define TAB 			2002



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

#define OTHER 		999



#endif
