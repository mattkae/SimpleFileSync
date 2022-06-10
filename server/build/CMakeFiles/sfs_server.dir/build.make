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
CMAKE_SOURCE_DIR = /home/matthewk/Projects/SimpleFileSync/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/matthewk/Projects/SimpleFileSync/server/build

# Include any dependencies generated for this target.
include CMakeFiles/sfs_server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/sfs_server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/sfs_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sfs_server.dir/flags.make

CMakeFiles/sfs_server.dir/src/main.o: CMakeFiles/sfs_server.dir/flags.make
CMakeFiles/sfs_server.dir/src/main.o: ../src/main.cpp
CMakeFiles/sfs_server.dir/src/main.o: CMakeFiles/sfs_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matthewk/Projects/SimpleFileSync/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sfs_server.dir/src/main.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sfs_server.dir/src/main.o -MF CMakeFiles/sfs_server.dir/src/main.o.d -o CMakeFiles/sfs_server.dir/src/main.o -c /home/matthewk/Projects/SimpleFileSync/server/src/main.cpp

CMakeFiles/sfs_server.dir/src/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sfs_server.dir/src/main.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matthewk/Projects/SimpleFileSync/server/src/main.cpp > CMakeFiles/sfs_server.dir/src/main.i

CMakeFiles/sfs_server.dir/src/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sfs_server.dir/src/main.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matthewk/Projects/SimpleFileSync/server/src/main.cpp -o CMakeFiles/sfs_server.dir/src/main.s

CMakeFiles/sfs_server.dir/src/server.o: CMakeFiles/sfs_server.dir/flags.make
CMakeFiles/sfs_server.dir/src/server.o: ../src/server.cpp
CMakeFiles/sfs_server.dir/src/server.o: CMakeFiles/sfs_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matthewk/Projects/SimpleFileSync/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/sfs_server.dir/src/server.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sfs_server.dir/src/server.o -MF CMakeFiles/sfs_server.dir/src/server.o.d -o CMakeFiles/sfs_server.dir/src/server.o -c /home/matthewk/Projects/SimpleFileSync/server/src/server.cpp

CMakeFiles/sfs_server.dir/src/server.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sfs_server.dir/src/server.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matthewk/Projects/SimpleFileSync/server/src/server.cpp > CMakeFiles/sfs_server.dir/src/server.i

CMakeFiles/sfs_server.dir/src/server.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sfs_server.dir/src/server.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matthewk/Projects/SimpleFileSync/server/src/server.cpp -o CMakeFiles/sfs_server.dir/src/server.s

# Object files for target sfs_server
sfs_server_OBJECTS = \
"CMakeFiles/sfs_server.dir/src/main.o" \
"CMakeFiles/sfs_server.dir/src/server.o"

# External object files for target sfs_server
sfs_server_EXTERNAL_OBJECTS =

sfs_server: CMakeFiles/sfs_server.dir/src/main.o
sfs_server: CMakeFiles/sfs_server.dir/src/server.o
sfs_server: CMakeFiles/sfs_server.dir/build.make
sfs_server: CMakeFiles/sfs_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/matthewk/Projects/SimpleFileSync/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable sfs_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sfs_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sfs_server.dir/build: sfs_server
.PHONY : CMakeFiles/sfs_server.dir/build

CMakeFiles/sfs_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sfs_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sfs_server.dir/clean

CMakeFiles/sfs_server.dir/depend:
	cd /home/matthewk/Projects/SimpleFileSync/server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/matthewk/Projects/SimpleFileSync/server /home/matthewk/Projects/SimpleFileSync/server /home/matthewk/Projects/SimpleFileSync/server/build /home/matthewk/Projects/SimpleFileSync/server/build /home/matthewk/Projects/SimpleFileSync/server/build/CMakeFiles/sfs_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sfs_server.dir/depend

