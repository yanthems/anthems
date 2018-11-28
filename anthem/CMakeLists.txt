include_directories(${ASIO_DIR}/include)
include_directories(${ANTHEM_DIR}/include)

set(LIBRARY_OUTPUT_PATH ${ANTHEM_DIR}/lib)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_DIR)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/include INCLUDE_DIR)

add_library(anthem STATIC ${SRC_DIR} ${INCLUDE_DIR} ${ASIO_DIR}/include)