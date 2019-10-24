aux_source_directory(. db_firewall_SRC)

#execute_process(COMMAND bash ../mm)

set(LIB_SRC ../../lib)

add_library(db_firewall STATIC ${db_firewall_SRC})
target_link_libraries(db_firewall CJsonObject base_sql_parser pthread mysqlclient z c dl)

set(LIBRARY_OUTPUT_PATH ${LIB_SRC})
