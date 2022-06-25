cmake_minimum_required(VERSION 3.23)
project(sfs_client)
include_directories(include)
set(SOURCES
  src/main.cpp
  src/app.cpp
  src/config.cpp
  src/file_watcher.cpp
)
set(CMAKE_CXX_STANDARD 20)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system)

include_directories(${Boost_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../shared/include)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES} ${CMAKE_CURRENT_SOURCE_DIR}/../shared/build/libsfs_shared.a)