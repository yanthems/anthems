include_directories(${COMMON_DIR}/include)
include_directories(${ASIO_DIR}/include)
link_directories(${COMMON_DIR}/lib)


add_executable (server "main.cpp")

target_include_directories(server PUBLIC ${COMMON_DIR}/include)

target_link_libraries(server common crypto)