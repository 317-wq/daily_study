cmake_minimum_required(VERSION 3.10)
project(MyRedisProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# hiredis 通过 apt 安装，没有 CMake 配置文件
# 这里提供自定义的 Findhiredis.cmake 模块
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# 查找 redis++，它会自动 find_dependency(hiredis)
find_package(redis++ REQUIRED)

add_executable(test test.cpp)

target_link_libraries(test redis++::redis++)
