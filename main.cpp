#include "firewall.h"
#include "CJsonObject.hpp"
#include <mysql/mysql.h>
#include <thread>
#include <fstream>
#include <dlfcn.h>
#include <iostream>
#include <unistd.h>
using namespace std;
using namespace neb;

extern atomic<bool> is_learning;
extern int LOG_LEVEL;
extern std::string db_user;
extern std::string db_password;
extern std::string db_name;
extern int time_out;
extern int listen_queue_size;
extern int default_level;

#define TTY_PATH 			"/dev/tty"
#define STTY_CLOSE          "stty raw -echo -F "
#define STTY_OPEN           "stty -raw echo -F "
//关闭参数
//system(STTY_CLOSE TTY_PATH);
//打开参数
//system(STTY_OPEN TTY_PATH);

int test_db_config()
{
	logger log;
	MYSQL *conn = mysql_init(NULL);
	if (!mysql_real_connect(conn,"127.0.0.1",db_user.c_str(),db_password.c_str(),db_name.c_str(),0,NULL,CLIENT_MULTI_RESULTS)){
		log.error(mysql_error(conn));
		exit(1);
	}
}


int main(int argc, char** argv)
{
	
	logger log;
	ifstream fin("/etc/db_FireWall/db_FireWall.json");
	string json;
	string _s;
	while(getline(fin, _s))
	{
		json += _s;
	}

	CJsonObject oJson(json);

	int server_port;
	oJson.Get("server_port", server_port);
	if(server_port < 0 || server_port > 65535)
	{
		log.error("server_port should be in [0,65535]");
		exit(0);
	}

	int firewall_port;
	oJson.Get("firewall_port",firewall_port);
	if(firewall_port < 0 || firewall_port > 65535)
	{
		log.error("firewall_port should be in [0,65535]");
		exit(0);
	}

	int ui_comm_port;
	oJson.Get("ui_comm_port", ui_comm_port);
	if(ui_comm_port < 0 || ui_comm_port > 65535)
	{
		log.error("ui_comm_port should be in [0,65535]");
		exit(0);
	}

	if(server_port == firewall_port || server_port == ui_comm_port || firewall_port == ui_comm_port)
	{
		log.error("ports cannot be same!!");
		exit(0);
	}

	int _is_learning;
	oJson.Get("is_learning", _is_learning);
	is_learning.store(bool(_is_learning));

	oJson.Get("LOG_LEVEL", LOG_LEVEL);


	string user_key = "db_user";
	if(oJson.IsNull(user_key))
	{
		printf("user:");
		cin >> db_user;
	}else
	{
		oJson.Get("db_user", db_user);
	}
	string pwd_key = "db_password";
	if(oJson.IsNull(pwd_key))
	{
		system(STTY_CLOSE TTY_PATH);
		printf("password:");
		cin >> db_password;
		system(STTY_OPEN TTY_PATH);
		//db_password = string(getpass("password:"));
	}else
	{
		oJson.Get("db_password", db_password);
	}
	
	
	oJson.Get("db_name",db_name);
	test_db_config();
	if(argc == 1);
	else if(argc == 2 && !strcmp(argv[1], "init"))
	{
		naive_filter::init_db();
		exit(0);
	}else
	{
		fprintf(stderr, "<usage> : db_FireWall [init]\n");
		exit(2);
	}

	oJson.Get("time_out", time_out);
	oJson.Get("listen_queue_size", listen_queue_size);

	oJson.Get("default_level", default_level);

	thread t1([&,server_port, firewall_port, ui_comm_port](){
		firewall fw;
		logger log;
		log.high_debug("firewall start");
		fw.start_firewall(server_port, firewall_port);
	});
	
	thread t2([&,server_port, firewall_port, ui_comm_port](){
		firewall fw;
		logger log;
		log.high_debug("web_UI start");
		fw.comm_with_web_UI(server_port, ui_comm_port);
	});

	t2.join();
	t1.join();
	
	return 0;
}
