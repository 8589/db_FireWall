#include "../simple_comm/simple_comm.h"
#include "../filter/filter.h"
#include <vector>
#include <string>

using namespace std;

class db_comm
{
private:
	simple_comm server;
	int client_fd;
	int db_port;
	char buff[BUFFSIZE];
	logger log;
	string user;
	sockaddr_in client_addr;
	int mode;	//learing or prtection mode

public:
	//sql_comm();
	db_comm(int client_fd, int mysql_port, sockaddr_in _client_addr, int _mode):
		client_fd(client_fd),db_port(mysql_port),client_addr(_client_addr),mode(_mode){}
	~db_comm()
	{
		server.close_socket(this->client_fd, &(this->client_addr));
		server.close_socket();
	}

	void handle_db_connection();

	void login();

	int one_comm();

	int hanlde_illegal_query();

	int client_to_server();

	int server_to_client();

	int recv_a_packet(string& recv_msg, int _socket_);

	
};