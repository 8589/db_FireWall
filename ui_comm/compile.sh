#!/bin/bash
g++ test_ui_comm.cpp ui_comm.cpp ../firewall/firewall.cpp ../simple_comm/simple_comm.cpp ../connector/connector.cpp ../connector/simple_conn.cpp ../logger/logger.cpp ../db_comm/db_comm.cpp ../filter/filter.cpp ../sql_parser/sql_parser.cpp ../sql_parser/lex.yy.cc ../sql_parser/FlexLexer.h -std=c++11 -o main -I/usr/include/mysql -lmysqlclient -lpthread
