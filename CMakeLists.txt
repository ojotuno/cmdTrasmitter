cmake_minimum_required(VERSION 3.16)

project(cmdTrasnmitter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBS_DIRS "${CMAKE_SOURCE_DIR}/lib/debug")
message("Lib dir -> " ${LIBS_DIRS})

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/zmq")
message("Include dir -> " ${INCLUDE_DIR})

include_directories(src)

add_executable(server src/main_server.cpp)
add_executable(client src/main_client.cpp)

# Add shared lib directory
target_link_directories(server PRIVATE  ${LIBS_DIRS})
target_link_directories(client PRIVATE  ${LIBS_DIRS})
# Add include path if needed
target_include_directories(server PRIVATE ${INCLUDE_DIR})
target_include_directories(client PRIVATE ${INCLUDE_DIR})
# Link against the binareis
target_link_libraries(server zmq)
target_link_libraries(client zmq)
