#include "firewall.h"
#include "CJsonObject.hpp"
#include <mysql/mysql.h>
#include <thread>
#include <fstream>
#include <dlfcn.h>
#include <iostream>
#include <unistd.h>
#include <getopt.h>

#include "spdlog/spdlog.h"


using namespace std;
using namespace neb;



#define TTY_PATH 			"/dev/tty"
#define STTY_CLOSE          "stty raw -echo -F "
#define STTY_OPEN           "stty -raw echo -F "
//关闭参数
//system(STTY_CLOSE TTY_PATH);
//打开参数
//system(STTY_OPEN TTY_PATH);

int test_db_config()
{
	MYSQL *conn = mysql_init(NULL);
	if (!mysql_real_connect(conn,"127.0.0.1",db_user.c_str(),db_password.c_str(),db_name.c_str(),0,NULL,CLIENT_MULTI_RESULTS)){
		fwLogger->error(mysql_error(conn));
		exit(1);
	}
    mysql_close(conn);
}

void initLogger(){
    try{
        fwLogger = spdlog::rotating_logger_mt("fwLogger", "/etc/db_FireWall/db_FireWall.log", 1024 * 1024 * 128, 16);
    }catch (const spdlog::spdlog_ex& ex){
        std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
        exit(-1);
    }
}

void checkServerPort(int serverPort){
    if(server_port < 0 || server_port > 65535){
        fwLogger->error("server_port should be in [0,65535]");
        exit(-3);
    }
}

void checkFwPort(int fwPort){
    if(firewall_port < 0 || firewall_port > 65535){
        fwLogger->error("firewall_port should be in [0,65535]");
        exit(-4);
    }
    if(fwPort == server_port){
        fwLogger->error(("firewall_port and server_port are same!"));
        exit(-5);
    }
}

void checkUICommPort(int uiCommPort){
    if(uiCommPort < 0 || uiCommPort > 65535){
        fwLogger->error("ui_comm_port should be in [0,65535]");
        exit(-6);
    }
    if(uiCommPort == server_port){
        fwLogger->error(("ui_comm_port and server_port are same!"));
        exit(-7);
    }
    if(uiCommPort == firewall_port){
        fwLogger->error(("ui_comm_port and firewall_port are same!"));
        exit(-8);
    }
}

void checkBuffSize(int buffSize){
    if(buffSize <= 0 || buffSize >= (1<<24)){
        fwLogger->error("buffsize is too big(>16M) or too small(<0)");
        exit(-9);
    }
}

CJsonObject getJson(const std::string configFile){
    std::ifstream fin(configFile);
    string config;
    if(!fin.is_open()){
        fwLogger->error("Cannot open config file");
        exit(-2);
    }
    string str_;
    while(getline(fin, str_)){
        config += str_;
    }
    return CJsonObject(config);
}

void getDBConfig(const CJsonObject &oJson){
    if(db_user.empty()){
        if(oJson.IsNull("db_user")){
            fwLogger->error("db_user cannot be empty!");
            exit(-11);
        }else{
            oJson.Get("db_user", db_user);
        }
    }

    if(db_password.empty()){
        if(oJson.IsNull("db_password")){
            std::cout << "input db_password:" << std::endl;
            system(STTY_CLOSE TTY_PATH);
            cin >> db_password;
            system(STTY_OPEN TTY_PATH);
        }else{
            oJson.Get("db_password", db_password);
        }
    }

    if(db_name.empty()){
        if(oJson.IsNull("db_name")){
            fwLogger->error("db_name cannot be empty!");
            exit(-12);
        }else{
            oJson.Get("db_name", db_name);
        }
    }
    test_db_config();
}


void setLogLevel(const std::string logLevel){
    if(logLevel == "trace"){
        fwLogger->set_level(spdlog::level::trace);
    }else if(logLevel == "debug"){
        fwLogger->set_level(spdlog::level::debug);
    }else if(logLevel == "info"){
        fwLogger->set_level(spdlog::level::info);
    }else if(logLevel == "warn"){
        fwLogger->set_level(spdlog::level::warn);
    }else if(logLevel == "error"){
        fwLogger->set_level(spdlog::level::err);
    }else if(logLevel == "critical"){
        fwLogger->set_level(spdlog::level::critical);
    }else{
        fwLogger->set_level(spdlog::level::off);
    }
}

void readConfigFile(const CJsonObject &oJson){

    oJson.Get("server_port", server_port);
    checkServerPort(server_port);

    oJson.Get("firewall_port", firewall_port);
    checkFwPort(firewall_port);

    oJson.Get("ui_comm_port", ui_comm_port);
    checkUICommPort(ui_comm_port);

    int isLearning_;
    oJson.Get("is_learning", isLearning_);
    is_learning.store(isLearning_);

    oJson.Get("LOG_LEVEL", LOG_LEVEL);
    setLogLevel(LOG_LEVEL);

    oJson.Get("time_out", time_out);
    oJson.Get("listen_queue_size", listen_queue_size);

    oJson.Get("default_level", default_level);
    oJson.Get("buffsize", buffsize);

    checkBuffSize(buffsize);

    oJson.Get("is_log_illegal_query", is_log_illegal_query);

}


int parseCMDParameter(int argc, char **argv){
    int opt;
    int isInit = 0;
    const char *optstring = "u:p:d:i";
    while( (opt = getopt(argc, argv, optstring)) != -1 ){
        switch (opt){
            case 'u' : db_user = optarg; break;
            case 'p' : db_password = optarg; break;
            case 'd' : db_name = optarg; break;
            case 'i' : isInit = 1; break;
            default : break;
        }
    }
    return isInit;
}

int main(int argc, char** argv)
{

    initLogger();

    int isInit = parseCMDParameter(argc, argv);


    CJsonObject oJson;

    if(db_user.empty() || db_password.empty() || db_name.empty()){
        oJson = getJson("/etc/db_FireWall/db_FireWall.json");
        getDBConfig(oJson);
    }

    if(isInit){
        naive_filter::init_db();
        return 0;
    }
    
    readConfigFile(oJson);

	thread t1(startFireWall);
	
	thread t2([](){
		firewall fw;
		fw.comm_with_web_UI(server_port, ui_comm_port);
	});

    fwLogger->info("database firewall start");

	t2.join();
	t1.join();
	
	return 0;
    
}
