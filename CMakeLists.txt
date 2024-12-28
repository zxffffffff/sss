cmake_minimum_required(VERSION 3.11)
project(sss C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -m64 -std=c99 -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wshadow -Wpointer-arith -Wcast-qual")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat -Wformat-security -Werror=format-security")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-prototypes -Wmissing-prototypes")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FORTIFY_SOURCE=2 -fPIC -fno-strict-overflow")

set(SRCS hazmat.c randombytes.c sss.c tweetnacl.c)

add_library(sss STATIC ${SRCS})

# Force unrolling loops on hazmat.c
set_source_files_properties(hazmat.c PROPERTIES COMPILE_FLAGS "-funroll-loops")

add_executable(test_hazmat test_hazmat.c)
target_link_libraries(test_hazmat PRIVATE sss)

add_executable(test_sss test_sss.c)
target_link_libraries(test_sss PRIVATE sss)

add_custom_target(check
    COMMAND test_hazmat
    COMMAND test_sss
    DEPENDS test_hazmat test_sss
)

add_custom_target(clean-all
    COMMAND ${CMAKE_MAKE_PROGRAM} clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
)
