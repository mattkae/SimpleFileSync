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
include shared/CMakeFiles/sfs_shared.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include shared/CMakeFiles/sfs_shared.dir/compiler_depend.make

# Include the progress variables for this target.
include shared/CMakeFiles/sfs_shared.dir/progress.make

# Include the compile flags for this target's objects.
include shared/CMakeFiles/sfs_shared.dir/flags.make

shared/CMakeFiles/sfs_shared.dir/src/message.cpp.o: shared/CMakeFiles/sfs_shared.dir/flags.make
shared/CMakeFiles/sfs_shared.dir/src/message.cpp.o: shared/src/message.cpp
shared/CMakeFiles/sfs_shared.dir/src/message.cpp.o: shared/CMakeFiles/sfs_shared.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matthewk/Projects/SimpleFileSync/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object shared/CMakeFiles/sfs_shared.dir/src/message.cpp.o"
	cd /home/matthewk/Projects/SimpleFileSync/shared && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT shared/CMakeFiles/sfs_shared.dir/src/message.cpp.o -MF CMakeFiles/sfs_shared.dir/src/message.cpp.o.d -o CMakeFiles/sfs_shared.dir/src/message.cpp.o -c /home/matthewk/Projects/SimpleFileSync/shared/src/message.cpp

shared/CMakeFiles/sfs_shared.dir/src/message.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sfs_shared.dir/src/message.cpp.i"
	cd /home/matthewk/Projects/SimpleFileSync/shared && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matthewk/Projects/SimpleFileSync/shared/src/message.cpp > CMakeFiles/sfs_shared.dir/src/message.cpp.i

shared/CMakeFiles/sfs_shared.dir/src/message.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sfs_shared.dir/src/message.cpp.s"
	cd /home/matthewk/Projects/SimpleFileSync/shared && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matthewk/Projects/SimpleFileSync/shared/src/message.cpp -o CMakeFiles/sfs_shared.dir/src/message.cpp.s

# Object files for target sfs_shared
sfs_shared_OBJECTS = \
"CMakeFiles/sfs_shared.dir/src/message.cpp.o"

# External object files for target sfs_shared
sfs_shared_EXTERNAL_OBJECTS =

shared/libsfs_shared.a: shared/CMakeFiles/sfs_shared.dir/src/message.cpp.o
shared/libsfs_shared.a: shared/CMakeFiles/sfs_shared.dir/build.make
shared/libsfs_shared.a: shared/CMakeFiles/sfs_shared.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/matthewk/Projects/SimpleFileSync/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libsfs_shared.a"
	cd /home/matthewk/Projects/SimpleFileSync/shared && $(CMAKE_COMMAND) -P CMakeFiles/sfs_shared.dir/cmake_clean_target.cmake
	cd /home/matthewk/Projects/SimpleFileSync/shared && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sfs_shared.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
shared/CMakeFiles/sfs_shared.dir/build: shared/libsfs_shared.a
.PHONY : shared/CMakeFiles/sfs_shared.dir/build

shared/CMakeFiles/sfs_shared.dir/clean:
	cd /home/matthewk/Projects/SimpleFileSync/shared && $(CMAKE_COMMAND) -P CMakeFiles/sfs_shared.dir/cmake_clean.cmake
.PHONY : shared/CMakeFiles/sfs_shared.dir/clean

shared/CMakeFiles/sfs_shared.dir/depend:
	cd /home/matthewk/Projects/SimpleFileSync && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/matthewk/Projects/SimpleFileSync /home/matthewk/Projects/SimpleFileSync/shared /home/matthewk/Projects/SimpleFileSync /home/matthewk/Projects/SimpleFileSync/shared /home/matthewk/Projects/SimpleFileSync/shared/CMakeFiles/sfs_shared.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : shared/CMakeFiles/sfs_shared.dir/depend

