cmake_minimum_required(VERSION 3.23)
project(sfs_shared)

add_library(${PROJECT_NAME} STATIC 
  src/client_message.cpp
  src/server_message.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

set(CMAKE_CXX_STANDARD 20)
