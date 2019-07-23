#include "filter.h"
#include <iostream>

using namespace std;
char str[BUFFSIZE];
int main()
{
	filter f;
	int n;
	cin >> n;
	getchar();
	while(n--)
	{
		gets(str);
		printf("%s\n", str);
		for(int i=1;i<5;i++)
		{
			string rule;
			f.parse_sql(string(str), i, rule);
			cout << "___________________________level " << i << "___________________________" << endl;
			cout << rule << endl; 
		}
		cout << endl << endl;
	}
	return 0;
}