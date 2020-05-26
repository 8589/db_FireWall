#include "utils.h"
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char **argv){
	int uiFd = getTCPClient_e("127.0.0.1", 6667);
	char buf[10];

	if(argv[1][0] == '0')
		send(uiFd, "\2\0\0\0\1\0", sizeof("\2\0\0\0\1\0"), 0);
	else
		send(uiFd, "\2\0\0\0\1\1", sizeof("\2\0\0\0\1\1"), 0);

	recv(uiFd, buf, 10, 0);
}