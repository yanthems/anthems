cmake_minimum_required (VERSION 3.8)

project ("anthems")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)

ADD_DEFINITIONS(-D ASIO_STANDALONE)
#ADD_DEFINITIONS(-D ASIO_SEPARATE_COMPILATION)


set(ANTHEM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/anthem CACHE STRING "anthem dir")
set(ASIO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/asio/asio CACHE STRING "asio dir")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lssl -lcrypto")

if (UNIX)
set(CMAKE_CXX_FLAGS "-pthread")
endif (UNIX)

if(APPLE)
    set(CMAKE_CXX_FLAGS "-lcrypto")
endif(APPLE)

add_subdirectory ("anthem")

add_subdirectory ("server")

add_subdirectory ("client")

