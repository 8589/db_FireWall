#include "config.h"

std::atomic<bool> is_learning(0);

int LOG_LEVEL = 2;
std::string db_user;
std::string db_password;
std::string db_name;