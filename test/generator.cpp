#include <cstdio>
#include <string>

using namespace std;

using namespace std;


int main(int argc, char **argv){
	int cnt = stoi(argv[1]);

	printf("use test;\n");

	// for(int i = 0;i < cnt; i ++){
	// 	printf("insert into t1 value(%d,%d,%d);\n", i,i,i);
	// }
	for(int i = 0;i < cnt; i++){
		printf("select * from t1;\n");
	}
	// for(int i = 0; i < cnt;i ++){
	// 	printf("delete from t1 where a = %d;\n", i);
	// }
}