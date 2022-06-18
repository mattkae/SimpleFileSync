# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/matthewk/Projects/SimpleFileSync

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/matthewk/Projects/SimpleFileSync

# Include any dependencies generated for this target.
include server/CMakeFiles/sfs_server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include server/CMakeFiles/sfs_server.dir/compiler_depend.make

# Include the progress variables for this target.
include server/CMakeFiles/sfs_server.dir/progress.make

# Include the compile flags for this target's objects.
include server/CMakeFiles/sfs_server.dir/flags.make

server/CMakeFiles/sfs_server.dir/src/main.cpp.o: server/CMakeFiles/sfs_server.dir/flags.make
server/CMakeFiles/sfs_server.dir/src/main.cpp.o: server/src/main.cpp
server/CMakeFiles/sfs_server.dir/src/main.cpp.o: server/CMakeFiles/sfs_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matthewk/Projects/SimpleFileSync/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object server/CMakeFiles/sfs_server.dir/src/main.cpp.o"
	cd /home/matthewk/Projects/SimpleFileSync/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT server/CMakeFiles/sfs_server.dir/src/main.cpp.o -MF CMakeFiles/sfs_server.dir/src/main.cpp.o.d -o CMakeFiles/sfs_server.dir/src/main.cpp.o -c /home/matthewk/Projects/SimpleFileSync/server/src/main.cpp

server/CMakeFiles/sfs_server.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sfs_server.dir/src/main.cpp.i"
	cd /home/matthewk/Projects/SimpleFileSync/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matthewk/Projects/SimpleFileSync/server/src/main.cpp > CMakeFiles/sfs_server.dir/src/main.cpp.i

server/CMakeFiles/sfs_server.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sfs_server.dir/src/main.cpp.s"
	cd /home/matthewk/Projects/SimpleFileSync/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matthewk/Projects/SimpleFileSync/server/src/main.cpp -o CMakeFiles/sfs_server.dir/src/main.cpp.s

server/CMakeFiles/sfs_server.dir/src/server.cpp.o: server/CMakeFiles/sfs_server.dir/flags.make
server/CMakeFiles/sfs_server.dir/src/server.cpp.o: server/src/server.cpp
server/CMakeFiles/sfs_server.dir/src/server.cpp.o: server/CMakeFiles/sfs_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matthewk/Projects/SimpleFileSync/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object server/CMakeFiles/sfs_server.dir/src/server.cpp.o"
	cd /home/matthewk/Projects/SimpleFileSync/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT server/CMakeFiles/sfs_server.dir/src/server.cpp.o -MF CMakeFiles/sfs_server.dir/src/server.cpp.o.d -o CMakeFiles/sfs_server.dir/src/server.cpp.o -c /home/matthewk/Projects/SimpleFileSync/server/src/server.cpp

server/CMakeFiles/sfs_server.dir/src/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sfs_server.dir/src/server.cpp.i"
	cd /home/matthewk/Projects/SimpleFileSync/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matthewk/Projects/SimpleFileSync/server/src/server.cpp > CMakeFiles/sfs_server.dir/src/server.cpp.i

server/CMakeFiles/sfs_server.dir/src/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sfs_server.dir/src/server.cpp.s"
	cd /home/matthewk/Projects/SimpleFileSync/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matthewk/Projects/SimpleFileSync/server/src/server.cpp -o CMakeFiles/sfs_server.dir/src/server.cpp.s

server/CMakeFiles/sfs_server.dir/src/config.cpp.o: server/CMakeFiles/sfs_server.dir/flags.make
server/CMakeFiles/sfs_server.dir/src/config.cpp.o: server/src/config.cpp
server/CMakeFiles/sfs_server.dir/src/config.cpp.o: server/CMakeFiles/sfs_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matthewk/Projects/SimpleFileSync/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object server/CMakeFiles/sfs_server.dir/src/config.cpp.o"
	cd /home/matthewk/Projects/SimpleFileSync/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT server/CMakeFiles/sfs_server.dir/src/config.cpp.o -MF CMakeFiles/sfs_server.dir/src/config.cpp.o.d -o CMakeFiles/sfs_server.dir/src/config.cpp.o -c /home/matthewk/Projects/SimpleFileSync/server/src/config.cpp

server/CMakeFiles/sfs_server.dir/src/config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sfs_server.dir/src/config.cpp.i"
	cd /home/matthewk/Projects/SimpleFileSync/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matthewk/Projects/SimpleFileSync/server/src/config.cpp > CMakeFiles/sfs_server.dir/src/config.cpp.i

server/CMakeFiles/sfs_server.dir/src/config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sfs_server.dir/src/config.cpp.s"
	cd /home/matthewk/Projects/SimpleFileSync/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matthewk/Projects/SimpleFileSync/server/src/config.cpp -o CMakeFiles/sfs_server.dir/src/config.cpp.s

# Object files for target sfs_server
sfs_server_OBJECTS = \
"CMakeFiles/sfs_server.dir/src/main.cpp.o" \
"CMakeFiles/sfs_server.dir/src/server.cpp.o" \
"CMakeFiles/sfs_server.dir/src/config.cpp.o"

# External object files for target sfs_server
sfs_server_EXTERNAL_OBJECTS =

server/sfs_server: server/CMakeFiles/sfs_server.dir/src/main.cpp.o
server/sfs_server: server/CMakeFiles/sfs_server.dir/src/server.cpp.o
server/sfs_server: server/CMakeFiles/sfs_server.dir/src/config.cpp.o
server/sfs_server: server/CMakeFiles/sfs_server.dir/build.make
server/sfs_server: /usr/lib/libboost_system.a
server/sfs_server: server/CMakeFiles/sfs_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/matthewk/Projects/SimpleFileSync/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable sfs_server"
	cd /home/matthewk/Projects/SimpleFileSync/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sfs_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
server/CMakeFiles/sfs_server.dir/build: server/sfs_server
.PHONY : server/CMakeFiles/sfs_server.dir/build

server/CMakeFiles/sfs_server.dir/clean:
	cd /home/matthewk/Projects/SimpleFileSync/server && $(CMAKE_COMMAND) -P CMakeFiles/sfs_server.dir/cmake_clean.cmake
.PHONY : server/CMakeFiles/sfs_server.dir/clean

server/CMakeFiles/sfs_server.dir/depend:
	cd /home/matthewk/Projects/SimpleFileSync && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/matthewk/Projects/SimpleFileSync /home/matthewk/Projects/SimpleFileSync/server /home/matthewk/Projects/SimpleFileSync /home/matthewk/Projects/SimpleFileSync/server /home/matthewk/Projects/SimpleFileSync/server/CMakeFiles/sfs_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : server/CMakeFiles/sfs_server.dir/depend

