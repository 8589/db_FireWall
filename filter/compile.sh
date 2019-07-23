#!/bin/bash
g++ test_filter.cpp filter.cpp ../sql_parser/sql_parser.cpp ../sql_parser/lex.yy.cc ../connector/connector.cpp ../connector/simple_conn.cpp -o test_filter -I/usr/include/mysql -lmysqlclient
