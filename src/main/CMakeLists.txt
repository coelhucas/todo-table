project(Main)
include(../shared_settings)

file(GLOB SOURCES "*.c")
add_executable(main ${SOURCES})
target_link_libraries(main table curl json-c)
