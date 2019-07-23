#!/bin/bash
g++ test_simple_conn.cpp simple_conn.cpp -o test_simple_conn -I/usr/include/mysql -lmysqlclient
