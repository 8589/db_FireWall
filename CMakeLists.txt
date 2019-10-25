
cmake_minimum_required(VERSION 3.1)
  

PROJECT(db_FireWall CXX)

add_compile_options(-std=c++11)

set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}")

aux_source_directory(. db_FireWall_SRC)

#execute_process(COMMAND bash ../mm)

include_directories(
	./include
	/usr/include/mysql
	)

link_directories(./lib)
add_subdirectory(src)

add_executable(db_FireWall ${db_FireWall_SRC})
target_link_libraries(db_FireWall CJsonObject base_sql_parser  db_firewall pthread mysqlclient z c dl)

