project(Http)
include(../shared_settings)

file(GLOB SOURCES "*.c")

add_library(http SHARED ${SOURCES})
