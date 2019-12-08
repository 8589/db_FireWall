aux_source_directory(. db_firewall_SRC)

#execute_process(COMMAND bash ../mm)

set(LIB_SRC ../../lib)

#add_library(db_firewall STATIC ${db_firewall_SRC})
#target_link_libraries(db_firewall CJsonObject base_sql_parser pthread mysqlclient z c dl)

#set(LIBRARY_OUTPUT_PATH ${LIB_SRC})

add_executable(testiii test.cpp)
target_link_libraries(testiii CJsonObject base_sql_parser  db_firewall mysqlclient z c dl)

add_executable(test1 test1.cpp)
target_link_libraries(test1 CJsonObject base_sql_parser  db_firewall mysqlclient z c dl)


add_executable(test2 test2.cpp)
target_link_libraries(test2 CJsonObject base_sql_parser  db_firewall mysqlclient z c dl)

add_executable(test3 test3.cpp)
target_link_libraries(test3 CJsonObject base_sql_parser  db_firewall mysqlclient z c dl)
