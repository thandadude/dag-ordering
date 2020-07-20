cmake_minimum_required(VERSION 2.8)
project(dag-ordering)

add_definitions(-std=c++11)

add_subdirectory(sources)
#include_directories(googlemock/include googletest/include)

#set (SRCS sources/*)
#set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -Wall -g")

#add_executable (dag-ordering ${SRCS})
#target_link_libraries(server)
#add_subdirectory(Test)
#add_test(alltests Test/run_tests)

option(WITH_TEST "build test" ON)
if(WITH_TEST)
	enable_testing()
endif()

