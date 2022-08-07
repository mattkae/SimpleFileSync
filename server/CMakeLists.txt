cmake_minimum_required(VERSION 3.23)
project(sfs_server)
include_directories(include)
set(CMAKE_CXX_STANDARD 20)

set(SOURCES
	src/main.cpp
	src/app.cpp
	src/config.cpp
)
find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)
set(OPENSSL_USE_STATIC_LIBS TRUE)
if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
endif()
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system)

include_directories(${Boost_INCLUDE_DIR} 
	${CMAKE_CURRENT_SOURCE_DIR}/../shared/include 
	${CMAKE_CURRENT_SOURCE_DIR}/../spdlog/include)
add_executable(${PROJECT_NAME} ${SOURCES})
if(MSVC)
  target_compile_options(${PROJECT_NAME} /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC
	${Boost_LIBRARIES}
	${CMAKE_CURRENT_SOURCE_DIR}/../shared/build/libsfs_shared.a
	${CMAKE_CURRENT_SOURCE_DIR}/../spdlog/build/libspdlog.a
	${OPENSSL_LIBRARIES}
)
