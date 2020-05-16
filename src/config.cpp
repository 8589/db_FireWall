#include "config.h"

std::atomic<bool> is_learning(0);

int LOG_LEVEL = 2;
std::string db_user;
std::string db_password;
std::string db_name;
int time_out = 60;
int listen_queue_size = 1024;
int default_level=0;
int server_port = 3306;
int firewall_port = 6666;
int ui_comm_port = 6667;


int buffsize = 1024;
int is_log_illegal_query = 1;