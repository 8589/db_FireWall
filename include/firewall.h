#ifndef _FIREWALL_H_
#define _FIREWALL_H_

#include "db_comm.h"
#include "ui_comm.h"
#include "config.h"
#include <thread>

using namespace std;

extern atomic<bool> is_learning;
extern int listen_queue_size;

class firewall
{
private:
	logger log;
public:

	void start_firewall(int db_server_port=3306, int firewall_port=6666);

	void comm_with_web_UI(int db_server_port=3306, int ui_comm_port=6667);

};

#endif
