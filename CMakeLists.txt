cmake_minimum_required(VERSION 3.5)

project(muhe264 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

set(SOURCES
    main.c
    mh_stream.c
    mh_log.c
    mh_test.c
    mh_type.c
    mh_mem.c
    mh_semantics.c
    mh_nal_unit.c)

set(HEADERS
    mh_stream.h
    mh_log.h
    mh_error.h
    mh_type.h
    mh_semantics.h
    mh_test.h
    mh_mem.h
    mh_nal_unit.h)

add_executable(muhe264 ${HEADERS} ${SOURCES})


install(TARGETS muhe264
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
