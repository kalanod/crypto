cmake_minimum_required(VERSION 3.20)
project(crypto)

set(CMAKE_CXX_STANDARD 23)
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
add_executable(crypto1 main.cpp User.h)

