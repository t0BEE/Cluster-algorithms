# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /cygdrive/c/Users/schiffmt/.CLion2017.1/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/schiffmt/.CLion2017.1/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/k_Means.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/k_Means.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/k_Means.dir/flags.make

CMakeFiles/k_Means.dir/kMeans.cpp.o: CMakeFiles/k_Means.dir/flags.make
CMakeFiles/k_Means.dir/kMeans.cpp.o: ../kMeans.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/k_Means.dir/kMeans.cpp.o"
	/usr/bin/c++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/k_Means.dir/kMeans.cpp.o -c /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/kMeans.cpp

CMakeFiles/k_Means.dir/kMeans.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/k_Means.dir/kMeans.cpp.i"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/kMeans.cpp > CMakeFiles/k_Means.dir/kMeans.cpp.i

CMakeFiles/k_Means.dir/kMeans.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/k_Means.dir/kMeans.cpp.s"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/kMeans.cpp -o CMakeFiles/k_Means.dir/kMeans.cpp.s

CMakeFiles/k_Means.dir/kMeans.cpp.o.requires:

.PHONY : CMakeFiles/k_Means.dir/kMeans.cpp.o.requires

CMakeFiles/k_Means.dir/kMeans.cpp.o.provides: CMakeFiles/k_Means.dir/kMeans.cpp.o.requires
	$(MAKE) -f CMakeFiles/k_Means.dir/build.make CMakeFiles/k_Means.dir/kMeans.cpp.o.provides.build
.PHONY : CMakeFiles/k_Means.dir/kMeans.cpp.o.provides

CMakeFiles/k_Means.dir/kMeans.cpp.o.provides.build: CMakeFiles/k_Means.dir/kMeans.cpp.o


CMakeFiles/k_Means.dir/kPoint.cpp.o: CMakeFiles/k_Means.dir/flags.make
CMakeFiles/k_Means.dir/kPoint.cpp.o: ../kPoint.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/k_Means.dir/kPoint.cpp.o"
	/usr/bin/c++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/k_Means.dir/kPoint.cpp.o -c /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/kPoint.cpp

CMakeFiles/k_Means.dir/kPoint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/k_Means.dir/kPoint.cpp.i"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/kPoint.cpp > CMakeFiles/k_Means.dir/kPoint.cpp.i

CMakeFiles/k_Means.dir/kPoint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/k_Means.dir/kPoint.cpp.s"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/kPoint.cpp -o CMakeFiles/k_Means.dir/kPoint.cpp.s

CMakeFiles/k_Means.dir/kPoint.cpp.o.requires:

.PHONY : CMakeFiles/k_Means.dir/kPoint.cpp.o.requires

CMakeFiles/k_Means.dir/kPoint.cpp.o.provides: CMakeFiles/k_Means.dir/kPoint.cpp.o.requires
	$(MAKE) -f CMakeFiles/k_Means.dir/build.make CMakeFiles/k_Means.dir/kPoint.cpp.o.provides.build
.PHONY : CMakeFiles/k_Means.dir/kPoint.cpp.o.provides

CMakeFiles/k_Means.dir/kPoint.cpp.o.provides.build: CMakeFiles/k_Means.dir/kPoint.cpp.o


CMakeFiles/k_Means.dir/kCluster.cpp.o: CMakeFiles/k_Means.dir/flags.make
CMakeFiles/k_Means.dir/kCluster.cpp.o: ../kCluster.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/k_Means.dir/kCluster.cpp.o"
	/usr/bin/c++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/k_Means.dir/kCluster.cpp.o -c /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/kCluster.cpp

CMakeFiles/k_Means.dir/kCluster.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/k_Means.dir/kCluster.cpp.i"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/kCluster.cpp > CMakeFiles/k_Means.dir/kCluster.cpp.i

CMakeFiles/k_Means.dir/kCluster.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/k_Means.dir/kCluster.cpp.s"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/kCluster.cpp -o CMakeFiles/k_Means.dir/kCluster.cpp.s

CMakeFiles/k_Means.dir/kCluster.cpp.o.requires:

.PHONY : CMakeFiles/k_Means.dir/kCluster.cpp.o.requires

CMakeFiles/k_Means.dir/kCluster.cpp.o.provides: CMakeFiles/k_Means.dir/kCluster.cpp.o.requires
	$(MAKE) -f CMakeFiles/k_Means.dir/build.make CMakeFiles/k_Means.dir/kCluster.cpp.o.provides.build
.PHONY : CMakeFiles/k_Means.dir/kCluster.cpp.o.provides

CMakeFiles/k_Means.dir/kCluster.cpp.o.provides.build: CMakeFiles/k_Means.dir/kCluster.cpp.o


# Object files for target k_Means
k_Means_OBJECTS = \
"CMakeFiles/k_Means.dir/kMeans.cpp.o" \
"CMakeFiles/k_Means.dir/kPoint.cpp.o" \
"CMakeFiles/k_Means.dir/kCluster.cpp.o"

# External object files for target k_Means
k_Means_EXTERNAL_OBJECTS =

k_Means.exe: CMakeFiles/k_Means.dir/kMeans.cpp.o
k_Means.exe: CMakeFiles/k_Means.dir/kPoint.cpp.o
k_Means.exe: CMakeFiles/k_Means.dir/kCluster.cpp.o
k_Means.exe: CMakeFiles/k_Means.dir/build.make
k_Means.exe: CMakeFiles/k_Means.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable k_Means.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/k_Means.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/k_Means.dir/build: k_Means.exe

.PHONY : CMakeFiles/k_Means.dir/build

CMakeFiles/k_Means.dir/requires: CMakeFiles/k_Means.dir/kMeans.cpp.o.requires
CMakeFiles/k_Means.dir/requires: CMakeFiles/k_Means.dir/kPoint.cpp.o.requires
CMakeFiles/k_Means.dir/requires: CMakeFiles/k_Means.dir/kCluster.cpp.o.requires

.PHONY : CMakeFiles/k_Means.dir/requires

CMakeFiles/k_Means.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/k_Means.dir/cmake_clean.cmake
.PHONY : CMakeFiles/k_Means.dir/clean

CMakeFiles/k_Means.dir/depend:
	cd /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/cmake-build-debug /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/cmake-build-debug /cygdrive/c/Users/schiffmt/source/repos/Clustering/Parallel_Clustering/k-Means/cmake-build-debug/CMakeFiles/k_Means.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/k_Means.dir/depend

