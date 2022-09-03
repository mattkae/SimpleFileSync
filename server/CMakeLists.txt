cmake_minimum_required(VERSION 3.1)
project(simplefilesync_s)
include_directories(include)
set(CMAKE_CXX_STANDARD 20)

set(SOURCES
	src/main.cpp
	src/server_app.cpp
	src/config.cpp
	src/server_socket.cpp
	src/socket_connection.cpp
)
find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)
set(OPENSSL_USE_STATIC_LIBS TRUE)
if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
endif()

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../shared/include)
add_executable(${PROJECT_NAME} ${SOURCES})
if(MSVC)
  target_compile_options(${PROJECT_NAME} /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../shared/build/libsimplefilesync_l.a
	${OPENSSL_LIBRARIES}
)
