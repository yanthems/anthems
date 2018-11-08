include_directories(${COMMON_DIR}/include)
link_directories(${COMMON_DIR}/lib)

add_executable (server "main.cpp")

target_link_libraries(server common crypto)