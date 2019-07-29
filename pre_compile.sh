#!/bin/bash

g++ pre_op_before_run.cpp ./utils/cJSON.c ./utils/CJsonObject.cpp ./ui_comm/ui_comm.cpp ./config/config.cpp ./firewall/firewall.cpp ./simple_comm/simple_comm.cpp ./connector/connector.cpp ./connector/simple_conn.cpp ./logger/logger.cpp ./db_comm/db_comm.cpp ./filter/filter.cpp ./sql_parser/sql_parser.cpp ./sql_parser/lex.yy.cc ./sql_parser/FlexLexer.h -std=c++11 -o pre_op_before_run -I/usr/include/mysql -lmysqlclient -lpthread
