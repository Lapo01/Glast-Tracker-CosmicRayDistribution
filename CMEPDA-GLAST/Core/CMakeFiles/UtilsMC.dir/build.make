# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/calisse/Desktop/CMEPDA/Glast-Tracker-CosmicRayDistribution/CMEPDA-GLAST

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/calisse/Desktop/CMEPDA/Glast-Tracker-CosmicRayDistribution/CMEPDA-GLAST/Core

# Include any dependencies generated for this target.
include CMakeFiles/UtilsMC.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/UtilsMC.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/UtilsMC.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/UtilsMC.dir/flags.make

CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.o: CMakeFiles/UtilsMC.dir/flags.make
CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.o: LibrariesSources/UtilsMC.cpp
CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.o: CMakeFiles/UtilsMC.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/calisse/Desktop/CMEPDA/Glast-Tracker-CosmicRayDistribution/CMEPDA-GLAST/Core/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.o -MF CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.o.d -o CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.o -c /home/calisse/Desktop/CMEPDA/Glast-Tracker-CosmicRayDistribution/CMEPDA-GLAST/Core/LibrariesSources/UtilsMC.cpp

CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/calisse/Desktop/CMEPDA/Glast-Tracker-CosmicRayDistribution/CMEPDA-GLAST/Core/LibrariesSources/UtilsMC.cpp > CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.i

CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/calisse/Desktop/CMEPDA/Glast-Tracker-CosmicRayDistribution/CMEPDA-GLAST/Core/LibrariesSources/UtilsMC.cpp -o CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.s

# Object files for target UtilsMC
UtilsMC_OBJECTS = \
"CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.o"

# External object files for target UtilsMC
UtilsMC_EXTERNAL_OBJECTS =

SharedLibs/libUtilsMC.so: CMakeFiles/UtilsMC.dir/LibrariesSources/UtilsMC.cpp.o
SharedLibs/libUtilsMC.so: CMakeFiles/UtilsMC.dir/build.make
SharedLibs/libUtilsMC.so: CMakeFiles/UtilsMC.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/calisse/Desktop/CMEPDA/Glast-Tracker-CosmicRayDistribution/CMEPDA-GLAST/Core/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library SharedLibs/libUtilsMC.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/UtilsMC.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/UtilsMC.dir/build: SharedLibs/libUtilsMC.so
.PHONY : CMakeFiles/UtilsMC.dir/build

CMakeFiles/UtilsMC.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/UtilsMC.dir/cmake_clean.cmake
.PHONY : CMakeFiles/UtilsMC.dir/clean

CMakeFiles/UtilsMC.dir/depend:
	cd /home/calisse/Desktop/CMEPDA/Glast-Tracker-CosmicRayDistribution/CMEPDA-GLAST/Core && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/calisse/Desktop/CMEPDA/Glast-Tracker-CosmicRayDistribution/CMEPDA-GLAST /home/calisse/Desktop/CMEPDA/Glast-Tracker-CosmicRayDistribution/CMEPDA-GLAST /home/calisse/Desktop/CMEPDA/Glast-Tracker-CosmicRayDistribution/CMEPDA-GLAST/Core /home/calisse/Desktop/CMEPDA/Glast-Tracker-CosmicRayDistribution/CMEPDA-GLAST/Core /home/calisse/Desktop/CMEPDA/Glast-Tracker-CosmicRayDistribution/CMEPDA-GLAST/Core/CMakeFiles/UtilsMC.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/UtilsMC.dir/depend

