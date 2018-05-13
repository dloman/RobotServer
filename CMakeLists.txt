cmake_minimum_required(VERSION 3.0)

#fixes wierd library shortname isssue
cmake_policy(SET CMP0060 NEW )

project(RobotServer)

set(CMAKE_C_COMPILER /usr/local/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/local/bin/g++)

set(MODULE_NAME RobotServer)
add_definitions(-DMODULE_NAME=\"RobotServer\")

# Enable all warnings
add_definitions(-Wall)

# Allow relocation
add_definitions(-fPIC)

find_package(Threads)

# c++ 20 option
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++2a" COMPILER_SUPPORTS_CXX17)
if(COMPILER_SUPPORTS_CXX20)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
  add_definitions(-std=c++2a)
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++20 support.
    Please use a different C++ compiler.")
endif()

find_package(DanLib REQUIRED)

include_directories(${DanLib_INCLUDE_DIR})

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_DEBUG_POSTFIX d)

add_executable(
  RobotServer
  src/RobotServer.cpp
  )

target_link_libraries(
  RobotServer
  DanLibWebsockets
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  )
