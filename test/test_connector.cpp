#include "connector.h"
#include <iostream>
using namespace std;


int main()
{
	connector conn;
	conn.connect_to_db();
	//conn.add_white_list("root", "select * from white_list");
	cout << conn.query_rule("root", "select * from white_list") << endl;

	conn.add_illegal_query("aaa","select * from a");
	conn.close();
	return 0;
}