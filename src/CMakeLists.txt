add_library(table SHARED table.c)
add_executable(main main.c)
target_link_libraries(main curl)
target_link_libraries(main table)