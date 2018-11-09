include_directories(BEFORE ${ASIO_DIR}/include)
include_directories(BEFORE ${COMMON_DIR}/include)


set(LIBRARY_OUTPUT_PATH ${COMMON_DIR}/lib)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_DIR)

add_library(common STATIC ${SRC_DIR})