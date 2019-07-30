#!/bin/bash

g++ main.cpp ./utils/cJSON.c ./utils/CJsonObject.cpp ./ui_comm/ui_comm.cpp ./config/config.cpp ./firewall/firewall.cpp ./simple_comm/simple_comm.cpp ./connector/connector.cpp ./connector/simple_conn.cpp ./logger/logger.cpp ./db_comm/db_comm.cpp ./filter/filter.cpp ./sql_parser/sql_parser.cpp ./sql_parser/lex.yy.cc ./sql_parser/FlexLexer.h -static -std=c++11 -o main -I/usr/include/mysql -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -lmysqlclient  -lz -lc -ldl 
