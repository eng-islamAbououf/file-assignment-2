# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\work\fileLab

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\work\fileLab\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/fileLab.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fileLab.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fileLab.dir/flags.make

CMakeFiles/fileLab.dir/main.cpp.obj: CMakeFiles/fileLab.dir/flags.make
CMakeFiles/fileLab.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\work\fileLab\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fileLab.dir/main.cpp.obj"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\fileLab.dir\main.cpp.obj -c D:\work\fileLab\main.cpp

CMakeFiles/fileLab.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fileLab.dir/main.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\work\fileLab\main.cpp > CMakeFiles\fileLab.dir\main.cpp.i

CMakeFiles/fileLab.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fileLab.dir/main.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\work\fileLab\main.cpp -o CMakeFiles\fileLab.dir\main.cpp.s

# Object files for target fileLab
fileLab_OBJECTS = \
"CMakeFiles/fileLab.dir/main.cpp.obj"

# External object files for target fileLab
fileLab_EXTERNAL_OBJECTS =

fileLab.exe: CMakeFiles/fileLab.dir/main.cpp.obj
fileLab.exe: CMakeFiles/fileLab.dir/build.make
fileLab.exe: CMakeFiles/fileLab.dir/linklibs.rsp
fileLab.exe: CMakeFiles/fileLab.dir/objects1.rsp
fileLab.exe: CMakeFiles/fileLab.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\work\fileLab\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable fileLab.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\fileLab.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fileLab.dir/build: fileLab.exe

.PHONY : CMakeFiles/fileLab.dir/build

CMakeFiles/fileLab.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\fileLab.dir\cmake_clean.cmake
.PHONY : CMakeFiles/fileLab.dir/clean

CMakeFiles/fileLab.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\work\fileLab D:\work\fileLab D:\work\fileLab\cmake-build-debug D:\work\fileLab\cmake-build-debug D:\work\fileLab\cmake-build-debug\CMakeFiles\fileLab.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fileLab.dir/depend
