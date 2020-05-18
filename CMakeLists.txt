
cmake_minimum_required(VERSION 2.8)
  

PROJECT(db_FireWall CXX)

add_compile_options(-std=c++11)

#if you want to compile with -static, cancel these two comment
#set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}")
#set(CMAKE_CXX_FLAGS "-Wl,--whole-archive -lpthread -Wl,--no-whole-archive  ${CMAKE_CXX_FLAGS}")


aux_source_directory(. db_FireWall_SRC)

#execute_process(COMMAND bash ../mm)

include_directories(
#	/usr/local/mysql/include/
	${PROJECT_SOURCE_DIR}/include
	/usr/include/mysql
)

link_directories(
#	/usr/local/mysql/lib
	${PROJECT_SOURCE_DIR}/lib
)

add_subdirectory(src)
add_subdirectory(test)

#add_executable(db_FireWall ${db_FireWall_SRC})
add_executable(db_FireWall main.cpp)
target_link_libraries(db_FireWall CJsonObject base_sql_parser  db_firewall mysqlclient z c dl)
