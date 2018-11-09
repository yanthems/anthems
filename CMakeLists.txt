cmake_minimum_required (VERSION 3.8)

project ("anthems")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 14)

ADD_DEFINITIONS(-D ASIO_STANDALONE)

set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common CACHE STRING "common dir")
set(ASIO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/asio/asio CACHE STRING "asio dir")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lssl -lcrypto")
if (UNIX)
set(CMAKE_CXX_FLAGS "-pthread")
endif (UNIX)

add_subdirectory ("common")

add_subdirectory ("server")

add_subdirectory ("client")
