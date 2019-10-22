#include "simple_conn.h"


int main()
{
	simple_conn s_conn;
	s_conn.connect_to("127.0.0.1", "root", "123456","test");
	std::vector<std::vector<string> > ans;
	ans = s_conn.query("select * from test_char;");
	for(int i=0;i<ans.size();i++){
		for(int j=0;j<ans[i].size();j++){
			printf("%s\n", ans[i][j].c_str());
		}
		printf("\n");
	}
	s_conn.close();
}