cmake_minimum_required(VERSION 3.23)
project(sfs_shared)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC 
  src/client_message.cpp
  src/server_message.cpp
  src/hash_calculator.cpp
  src/base_config.cpp
  src/save_area.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

