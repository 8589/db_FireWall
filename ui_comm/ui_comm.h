#include "../simple_comm/simple_comm.h"

class ui_comm
{
private:
	int client_fd;
	simple_comm server;
	char buff[BUFFSIZE];
public:
	ui_comm(int _client):client_fd(_client){};

	void handle_ui_connection();

	int recv_a_packet(string& recv_msg, int _socket_)

	
};