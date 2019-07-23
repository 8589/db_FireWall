#!/bin/bash
g++ test_simple_comm_client.cpp simple_comm.cpp simple_comm.h ../logger/logger.h ../logger/logger.cpp -o test_simple_comm_client
