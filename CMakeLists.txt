cmake_minimum_required(VERSION 3.10)
project(MySQLTest)

set(CMAKE_CXX_STANDARD 11)

# MySQL Connector/C++ 1.1.x (旧版 API) — 手动指定头文件和库路径
find_path(MYSQL_CONCPP_INCLUDE_DIR mysql_driver.h)
find_library(MYSQL_CONCPP_LIBRARY mysqlcppconn)

if(NOT MYSQL_CONCPP_INCLUDE_DIR OR NOT MYSQL_CONCPP_LIBRARY)
    message(FATAL_ERROR "未找到 MySQL Connector/C++，请安装 libmysqlcppconn-dev")
endif()

include_directories(${MYSQL_CONCPP_INCLUDE_DIR})

# 添加可执行文件
add_executable(test test.cpp)

# 链接 MySQL 库
target_link_libraries(test ${MYSQL_CONCPP_LIBRARY})