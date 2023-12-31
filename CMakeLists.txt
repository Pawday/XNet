cmake_minimum_required(VERSION 3.25)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(XNet LANGUAGES CXX VERSION 0.1.0)

option(XNET_ENABLE_TESTS "Enable tests from XNet library" OFF)

include(Compiler)


add_library(XNet OBJECT)
cm_compiler_try_enable_strict_compilation(XNet)
target_compile_features(XNet PUBLIC "cxx_std_23")

target_include_directories(XNet PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(XNet PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IP.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IPv4.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IPProbe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IPv4TOS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IPv4Options.cpp
)

if (${XNET_ENABLE_TESTS})
    message(STATUS "XNet Configuring tests")
    enable_testing()
    add_subdirectory(tests)
endif()
