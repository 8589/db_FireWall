#ifndef _FIREWALL_H_
#define _FIREWALL_H_

#include "../db_comm/db_comm.h"
#include "../ui_comm/ui_comm.h"
#include "../config/config.h"
#include <thread>

using namespace std;

extern atomic<bool> is_learning;

class firewall
{
public:

	void start_firewall();


	void comm_with_web_UI();
};

#endif