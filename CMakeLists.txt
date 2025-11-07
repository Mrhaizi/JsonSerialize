cmake_minimum_required(VERSION 3.17)
project(json_serialize)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(jsoncpp REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror=return-type)

target_link_libraries(${PROJECT_NAME} jsoncpp_lib)


