add_library(table SHARED table.c)
add_library(http SHARED http.c)
add_executable(main main.c)
target_link_libraries(main curl table http)
