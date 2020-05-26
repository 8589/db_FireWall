#include "config.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <memory>

std::atomic<bool> is_learning(0);

std::string LOG_LEVEL = "info";
std::string db_user;
std::string db_password;
std::string db_name;
int time_out = 60;
int listen_queue_size = 1024;
int default_level=0;
int server_port = 3306;
int firewall_port = 6666;
int ui_comm_port = 6667;

std::shared_ptr<spdlog::logger> fwLogger;

int buffsize = 1024;
int is_log_illegal_query = 1;