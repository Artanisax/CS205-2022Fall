# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /root/GitHub/CS205-2022Fall/Project5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/GitHub/CS205-2022Fall/Project5/build

# Include any dependencies generated for this target.
include CMakeFiles/Matrix.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Matrix.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Matrix.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Matrix.dir/flags.make

CMakeFiles/Matrix.dir/src/Matrix.cpp.o: CMakeFiles/Matrix.dir/flags.make
CMakeFiles/Matrix.dir/src/Matrix.cpp.o: ../src/Matrix.cpp
CMakeFiles/Matrix.dir/src/Matrix.cpp.o: CMakeFiles/Matrix.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/GitHub/CS205-2022Fall/Project5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Matrix.dir/src/Matrix.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Matrix.dir/src/Matrix.cpp.o -MF CMakeFiles/Matrix.dir/src/Matrix.cpp.o.d -o CMakeFiles/Matrix.dir/src/Matrix.cpp.o -c /root/GitHub/CS205-2022Fall/Project5/src/Matrix.cpp

CMakeFiles/Matrix.dir/src/Matrix.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Matrix.dir/src/Matrix.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/GitHub/CS205-2022Fall/Project5/src/Matrix.cpp > CMakeFiles/Matrix.dir/src/Matrix.cpp.i

CMakeFiles/Matrix.dir/src/Matrix.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Matrix.dir/src/Matrix.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/GitHub/CS205-2022Fall/Project5/src/Matrix.cpp -o CMakeFiles/Matrix.dir/src/Matrix.cpp.s

CMakeFiles/Matrix.dir/src/demo.cpp.o: CMakeFiles/Matrix.dir/flags.make
CMakeFiles/Matrix.dir/src/demo.cpp.o: ../src/demo.cpp
CMakeFiles/Matrix.dir/src/demo.cpp.o: CMakeFiles/Matrix.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/GitHub/CS205-2022Fall/Project5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Matrix.dir/src/demo.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Matrix.dir/src/demo.cpp.o -MF CMakeFiles/Matrix.dir/src/demo.cpp.o.d -o CMakeFiles/Matrix.dir/src/demo.cpp.o -c /root/GitHub/CS205-2022Fall/Project5/src/demo.cpp

CMakeFiles/Matrix.dir/src/demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Matrix.dir/src/demo.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/GitHub/CS205-2022Fall/Project5/src/demo.cpp > CMakeFiles/Matrix.dir/src/demo.cpp.i

CMakeFiles/Matrix.dir/src/demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Matrix.dir/src/demo.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/GitHub/CS205-2022Fall/Project5/src/demo.cpp -o CMakeFiles/Matrix.dir/src/demo.cpp.s

# Object files for target Matrix
Matrix_OBJECTS = \
"CMakeFiles/Matrix.dir/src/Matrix.cpp.o" \
"CMakeFiles/Matrix.dir/src/demo.cpp.o"

# External object files for target Matrix
Matrix_EXTERNAL_OBJECTS =

Matrix: CMakeFiles/Matrix.dir/src/Matrix.cpp.o
Matrix: CMakeFiles/Matrix.dir/src/demo.cpp.o
Matrix: CMakeFiles/Matrix.dir/build.make
Matrix: /usr/lib/gcc/x86_64-linux-gnu/11/libgomp.so
Matrix: /usr/lib/x86_64-linux-gnu/libpthread.a
Matrix: CMakeFiles/Matrix.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/GitHub/CS205-2022Fall/Project5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Matrix"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Matrix.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Matrix.dir/build: Matrix
.PHONY : CMakeFiles/Matrix.dir/build

CMakeFiles/Matrix.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Matrix.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Matrix.dir/clean

CMakeFiles/Matrix.dir/depend:
	cd /root/GitHub/CS205-2022Fall/Project5/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/GitHub/CS205-2022Fall/Project5 /root/GitHub/CS205-2022Fall/Project5 /root/GitHub/CS205-2022Fall/Project5/build /root/GitHub/CS205-2022Fall/Project5/build /root/GitHub/CS205-2022Fall/Project5/build/CMakeFiles/Matrix.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Matrix.dir/depend
