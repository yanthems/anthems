include_directories(SYSTEM ${ASIO_DIR}/include)
include_directories(BEFORE ${COMMON_DIR}/include)


set(LIBRARY_OUTPUT_PATH ${COMMON_DIR}/lib)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_DIR)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/include INCLUDE_DIR)

add_library(common STATIC ${SRC_DIR} ${INCLUDE_DIR})