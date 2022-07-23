cmake_minimum_required(VERSION 3.23)
project(sfs_shared)

find_package(ZLIB)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC 
  src/client_message.cpp
  src/server_message.cpp
  src/hash_calculator.cpp
  src/base_config.cpp
  src/save_area.cpp
  src/state.cpp
  src/event.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../spdlog/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/../spdlog/build/libspdlog.a
  ZLIB::ZLIB
)
