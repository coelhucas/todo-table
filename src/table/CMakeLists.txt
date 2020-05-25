project(Table)
include(../shared_settings)

file(GLOB SOURCES "*.c")

add_library(table SHARED ${SOURCES})
