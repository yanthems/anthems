include_directories(${ANTHEM_DIR}/include)
include_directories(${ASIO_DIR}/include)
link_directories(${ANTHEM_DIR}/lib)

option(USE_LIBGO "use the lib go" ON)
message("option USE_LIBGO is ${USE_LIBGO}!")

if(USE_LIBGO)
ADD_DEFINITIONS(-D USE_LIBGO)
endif(USE_LIBGO)

add_executable (server "main.cpp" ${ANTHEM_DIR}/include ${ASIO_DIR}/include)

target_include_directories(server PUBLIC ${ANTHEM_DIR}/include)

if (USE_LIBGO)
    target_link_libraries(server libgo dl anthem crypto)
else()
    target_link_libraries(server anthem crypto)
endif (USE_LIBGO)
