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
CMAKE_SOURCE_DIR = /root/GitHub/CS205-2022Fall/Project2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/GitHub/CS205-2022Fall/Project2/build

# Include any dependencies generated for this target.
include CMakeFiles/Calculator.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Calculator.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Calculator.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Calculator.dir/flags.make

CMakeFiles/Calculator.dir/src/calculator.cpp.o: CMakeFiles/Calculator.dir/flags.make
CMakeFiles/Calculator.dir/src/calculator.cpp.o: ../src/calculator.cpp
CMakeFiles/Calculator.dir/src/calculator.cpp.o: CMakeFiles/Calculator.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/GitHub/CS205-2022Fall/Project2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Calculator.dir/src/calculator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Calculator.dir/src/calculator.cpp.o -MF CMakeFiles/Calculator.dir/src/calculator.cpp.o.d -o CMakeFiles/Calculator.dir/src/calculator.cpp.o -c /root/GitHub/CS205-2022Fall/Project2/src/calculator.cpp

CMakeFiles/Calculator.dir/src/calculator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Calculator.dir/src/calculator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/GitHub/CS205-2022Fall/Project2/src/calculator.cpp > CMakeFiles/Calculator.dir/src/calculator.cpp.i

CMakeFiles/Calculator.dir/src/calculator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Calculator.dir/src/calculator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/GitHub/CS205-2022Fall/Project2/src/calculator.cpp -o CMakeFiles/Calculator.dir/src/calculator.cpp.s

CMakeFiles/Calculator.dir/src/number.cpp.o: CMakeFiles/Calculator.dir/flags.make
CMakeFiles/Calculator.dir/src/number.cpp.o: ../src/number.cpp
CMakeFiles/Calculator.dir/src/number.cpp.o: CMakeFiles/Calculator.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/GitHub/CS205-2022Fall/Project2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Calculator.dir/src/number.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Calculator.dir/src/number.cpp.o -MF CMakeFiles/Calculator.dir/src/number.cpp.o.d -o CMakeFiles/Calculator.dir/src/number.cpp.o -c /root/GitHub/CS205-2022Fall/Project2/src/number.cpp

CMakeFiles/Calculator.dir/src/number.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Calculator.dir/src/number.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/GitHub/CS205-2022Fall/Project2/src/number.cpp > CMakeFiles/Calculator.dir/src/number.cpp.i

CMakeFiles/Calculator.dir/src/number.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Calculator.dir/src/number.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/GitHub/CS205-2022Fall/Project2/src/number.cpp -o CMakeFiles/Calculator.dir/src/number.cpp.s

# Object files for target Calculator
Calculator_OBJECTS = \
"CMakeFiles/Calculator.dir/src/calculator.cpp.o" \
"CMakeFiles/Calculator.dir/src/number.cpp.o"

# External object files for target Calculator
Calculator_EXTERNAL_OBJECTS =

Calculator: CMakeFiles/Calculator.dir/src/calculator.cpp.o
Calculator: CMakeFiles/Calculator.dir/src/number.cpp.o
Calculator: CMakeFiles/Calculator.dir/build.make
Calculator: CMakeFiles/Calculator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/GitHub/CS205-2022Fall/Project2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Calculator"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Calculator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Calculator.dir/build: Calculator
.PHONY : CMakeFiles/Calculator.dir/build

CMakeFiles/Calculator.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Calculator.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Calculator.dir/clean

CMakeFiles/Calculator.dir/depend:
	cd /root/GitHub/CS205-2022Fall/Project2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/GitHub/CS205-2022Fall/Project2 /root/GitHub/CS205-2022Fall/Project2 /root/GitHub/CS205-2022Fall/Project2/build /root/GitHub/CS205-2022Fall/Project2/build /root/GitHub/CS205-2022Fall/Project2/build/CMakeFiles/Calculator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Calculator.dir/depend

