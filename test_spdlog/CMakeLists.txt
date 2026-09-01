cmake_minimum_required(VERSION 3.10)
project(test_spdlog)

find_package(spdlog REQUIRED) # 查找系统安装的spdlog

add_executable(test1 test1.cpp)

target_link_libraries(test1 PRIVATE spdlog::spdlog) # 链接 spdlog 库 