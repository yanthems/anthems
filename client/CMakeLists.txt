include_directories(${COMMON_DIR}/include)
link_directories(${COMMON_DIR}/lib)


add_executable (client "main.cpp")

target_include_directories(client PUBLIC ${ASIO_DIR}/include ${COMMON_DIR}/include)

target_link_libraries(client common crypto)