#include "firewall/firewall.h"
#include <thread>

int main()
{
	
	thread t1([](){
		firewall fw;
		printf("firewall start\n");
		fw.start_firewall();
	});
	
	thread t2([](){
		firewall fw;
		printf("web_UI start\n");
		fw.comm_with_web_UI();
	});
	t2.join();
	t1.join();
	
	return 0;
}


