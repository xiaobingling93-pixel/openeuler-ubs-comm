include_directories(${CMAKE_CURRENT_LIST_DIR}/../util)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../include/umq)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../umq)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../umq/qbuf)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} COMMON_RPC_ADPT_SRC)
add_subdirectory(brpc)

