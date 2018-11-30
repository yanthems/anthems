include_directories(${ANTHEM_DIR}/include)
include_directories(${ASIO_DIR}/include)
link_directories(${ANTHEM_DIR}/lib)

ADD_DEFINITIONS(-D USE_LIBGO)
set(CMAKE_CXX_FLAGS "-llibgo -ldl ${CMAKE_CXX_FLAGS}")

add_executable (server "main.cpp" ${ANTHEM_DIR}/include ${ASIO_DIR}/include)

target_include_directories(server PUBLIC ${ANTHEM_DIR}/include)

target_link_libraries(server anthem crypto)