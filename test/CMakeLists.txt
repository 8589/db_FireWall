
cmake_minimum_required(VERSION 3.1)
  

PROJECT(test CXX)

add_compile_options(-std=c++11)

#aux_source_directory(. test_SRC)


include_directories(
	./include
	/usr/include/mysql
	)

link_directories(./lib)

add_executable(test_shared_ptr test_shared_ptr.cpp)
#add_executable(db_FireWall ${db_FireWall_SRC})
#target_link_libraries(db_FireWall CJsonObject sql_parser db_firewall pthread mysqlclient z c dl)

