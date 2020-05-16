#include "naive_sql_parser.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;


int main(){
	naive_sql_parser nsp;
	vector<string> res;
	string sql = "select * from t1 where name = \"a;\\\\";
	nsp.parse_sql(sql, res);
	for(auto ele : res){
		cout << ele << endl;
	}
	return 0;
}