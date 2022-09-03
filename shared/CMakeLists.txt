cmake_minimum_required(VERSION 3.1)
project(simplefilesync_l)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC 
  src/client_message.cpp
  src/server_message.cpp
  src/hash_calculator.cpp
  src/base_config.cpp
  src/environment.cpp
  src/state.cpp
  src/event.cpp
  src/event_ledger.cpp
  src/program_options.cpp
  src/logger.cpp
)

if(MSVC)
  target_compile_options(${PROJECT_NAME} /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC )
