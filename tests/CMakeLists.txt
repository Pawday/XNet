cmake_minimum_required(VERSION 3.10)

if (NOT TARGET XNet)
    message(FATAL_ERROR "Where is the library to test?")
endif ()

enable_testing()

add_executable(IPHeaderSearchTester IPHeaderSearchTest.cpp)
target_link_libraries(IPHeaderSearchTester PRIVATE XNet)
add_test(IPHeaderSearchTest IPHeaderSearchTester)
