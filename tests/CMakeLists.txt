cmake_minimum_required(VERSION 3.10)

if (NOT TARGET XNet)
    message(FATAL_ERROR "Where is the library to test?")
endif ()

enable_testing()

add_executable(IPv4HeaderParseTester IPv4HeaderParseTester.cpp)
target_link_libraries(IPv4HeaderParseTester PRIVATE XNet)
add_test(ParseIPv4Header IPv4HeaderParseTester)
