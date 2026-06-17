cmake_minimum_required(VERSION 3.10)
project(CodeService)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 找到 redis++（会同时引入 hiredis）
find_package(PkgConfig REQUIRED)
pkg_check_modules(REDIS_PP REQUIRED IMPORTED_TARGET redis++)
pkg_check_modules(SPDLOG REQUIRED IMPORTED_TARGET spdlog)

add_executable(code_service main.cpp)

target_link_libraries(code_service
    PkgConfig::REDIS_PP
    PkgConfig::SPDLOG
)
