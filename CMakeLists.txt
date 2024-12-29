cmake_minimum_required(VERSION 3.11)
project(sss C)

# 编译器标志 (通用 - 基础部分)
set(COMMON_CFLAGS -g -O2 -std=c99 -pedantic -D_FORTIFY_SOURCE=2)

if(MSVC)
    # MSVC 特定的设置
    # /W3 是常用的警告等级，/WX 将警告视为错误
    set(MSVC_COMMON_CFLAGS /W3 /WX)

    # 映射一些通用的警告到 MSVC 等价物 (并非所有都有直接对应)
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC") # 仅针对 C++ 编译器
        # /wd4100 : unreferenced formal parameter (对应 -Wunused-parameter)
        # /wd4101 : unreferenced local variable (对应 -Wunused-variable)
        # /wd4102 : unreferenced label (对应 -Wunused-label)
        # /wd4201 : nonstandard extension used : nameless struct/union (某些情况下有用)
        # /wd4702 : unreachable code (对应 -Wunreachable-code)
        # /wd4714 : marked as __forceinline not inlined (对应 -Winline)
        # ... 其他你认为重要的警告 ...
        list(APPEND MSVC_COMMON_CFLAGS /wd4100 /wd4101 /wd4102 /wd4201 /wd4702 /wd4714)
    endif()

    # 移除 GCC 特定的选项
    string(REPLACE "-m64" "" COMMON_CFLAGS ${COMMON_CFLAGS}) # 架构由配置控制
    string(REPLACE "-fPIC" "" COMMON_CFLAGS ${COMMON_CFLAGS}) # Windows 静态库默认就是 PIC
    string(REPLACE "-fno-strict-overflow" "" COMMON_CFLAGS ${COMMON_CFLAGS}) # MSVC 行为不同

    # 将 MSVC 特定的标志添加到最终的编译选项中
    list(APPEND COMMON_CFLAGS ${MSVC_COMMON_CFLAGS})

else() # GCC, Clang 等其他编译器

    list(APPEND COMMON_CFLAGS -m64 -fPIC -fno-strict-overflow) # 添加必要的选项

    # 添加 GCC/Clang 特定的警告
    list(APPEND COMMON_CFLAGS -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wformat -Wformat-security -Werror=format-security -Wstrict-prototypes -Wmissing-prototypes)
endif()

set(SRCS hazmat.c randombytes.c sss.c tweetnacl.c)
set(INCS hazmat.h randombytes.h sss.h tweetnacl.h)

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

set_target_properties(sss
    PROPERTIES
    PUBLIC_HEADER "${INCS}"
)

install(TARGETS sss
    PUBLIC_HEADER DESTINATION include
    RUNTIME DESTINATION bin # .dll
    LIBRARY DESTINATION bin # .dylib .so
    ARCHIVE DESTINATION lib # .lib .a
)
