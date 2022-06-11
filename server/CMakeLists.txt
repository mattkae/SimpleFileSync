project(sfs_server)
include_directories(include)
set(SOURCES
	src/main.cpp
	src/server.cpp
	src/config.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
