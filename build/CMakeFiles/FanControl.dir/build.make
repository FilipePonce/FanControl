# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_SOURCE_DIR = /home/filipe/projects/FanControl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/filipe/projects/FanControl/build

# Include any dependencies generated for this target.
include CMakeFiles/FanControl.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FanControl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FanControl.dir/flags.make

CMakeFiles/FanControl.dir/main.cpp.o: CMakeFiles/FanControl.dir/flags.make
CMakeFiles/FanControl.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/filipe/projects/FanControl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FanControl.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FanControl.dir/main.cpp.o -c /home/filipe/projects/FanControl/main.cpp

CMakeFiles/FanControl.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FanControl.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/filipe/projects/FanControl/main.cpp > CMakeFiles/FanControl.dir/main.cpp.i

CMakeFiles/FanControl.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FanControl.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/filipe/projects/FanControl/main.cpp -o CMakeFiles/FanControl.dir/main.cpp.s

CMakeFiles/FanControl.dir/modbus_relay.cpp.o: CMakeFiles/FanControl.dir/flags.make
CMakeFiles/FanControl.dir/modbus_relay.cpp.o: ../modbus_relay.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/filipe/projects/FanControl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/FanControl.dir/modbus_relay.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FanControl.dir/modbus_relay.cpp.o -c /home/filipe/projects/FanControl/modbus_relay.cpp

CMakeFiles/FanControl.dir/modbus_relay.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FanControl.dir/modbus_relay.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/filipe/projects/FanControl/modbus_relay.cpp > CMakeFiles/FanControl.dir/modbus_relay.cpp.i

CMakeFiles/FanControl.dir/modbus_relay.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FanControl.dir/modbus_relay.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/filipe/projects/FanControl/modbus_relay.cpp -o CMakeFiles/FanControl.dir/modbus_relay.cpp.s

CMakeFiles/FanControl.dir/backend.cpp.o: CMakeFiles/FanControl.dir/flags.make
CMakeFiles/FanControl.dir/backend.cpp.o: ../backend.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/filipe/projects/FanControl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/FanControl.dir/backend.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FanControl.dir/backend.cpp.o -c /home/filipe/projects/FanControl/backend.cpp

CMakeFiles/FanControl.dir/backend.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FanControl.dir/backend.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/filipe/projects/FanControl/backend.cpp > CMakeFiles/FanControl.dir/backend.cpp.i

CMakeFiles/FanControl.dir/backend.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FanControl.dir/backend.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/filipe/projects/FanControl/backend.cpp -o CMakeFiles/FanControl.dir/backend.cpp.s

# Object files for target FanControl
FanControl_OBJECTS = \
"CMakeFiles/FanControl.dir/main.cpp.o" \
"CMakeFiles/FanControl.dir/modbus_relay.cpp.o" \
"CMakeFiles/FanControl.dir/backend.cpp.o"

# External object files for target FanControl
FanControl_EXTERNAL_OBJECTS =

bin/FanControl: CMakeFiles/FanControl.dir/main.cpp.o
bin/FanControl: CMakeFiles/FanControl.dir/modbus_relay.cpp.o
bin/FanControl: CMakeFiles/FanControl.dir/backend.cpp.o
bin/FanControl: CMakeFiles/FanControl.dir/build.make
bin/FanControl: /usr/lib/aarch64-linux-gnu/libcurl.so
bin/FanControl: /usr/lib/aarch64-linux-gnu/libmosquitto.so
bin/FanControl: CMakeFiles/FanControl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/filipe/projects/FanControl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable bin/FanControl"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FanControl.dir/link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Creating symlink to frontend directory"
	/usr/bin/cmake -E make_directory /home/filipe/projects/FanControl/build/bin
	/usr/bin/cmake -E create_symlink /home/filipe/projects/FanControl/frontend /home/filipe/projects/FanControl/build/bin/frontend

# Rule to build all files generated by this target.
CMakeFiles/FanControl.dir/build: bin/FanControl

.PHONY : CMakeFiles/FanControl.dir/build

CMakeFiles/FanControl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FanControl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FanControl.dir/clean

CMakeFiles/FanControl.dir/depend:
	cd /home/filipe/projects/FanControl/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/filipe/projects/FanControl /home/filipe/projects/FanControl /home/filipe/projects/FanControl/build /home/filipe/projects/FanControl/build /home/filipe/projects/FanControl/build/CMakeFiles/FanControl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FanControl.dir/depend

