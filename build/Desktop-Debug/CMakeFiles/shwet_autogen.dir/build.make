# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /nix/store/yxf0cmyfrar671zqh0ml8pcw15mxk0mh-cmake-3.30.5/bin/cmake

# The command to remove a file.
RM = /nix/store/yxf0cmyfrar671zqh0ml8pcw15mxk0mh-cmake-3.30.5/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/prayag/me/programming/in-progress/qt/shwet

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug

# Utility rule file for shwet_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/shwet_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/shwet_autogen.dir/progress.make

CMakeFiles/shwet_autogen: shwet_autogen/timestamp

shwet_autogen/timestamp: /nix/store/qcvz1s10bjr2na60w3zh91pfny8y5qxr-qt-full-6.8.1/libexec/moc
shwet_autogen/timestamp: /nix/store/qcvz1s10bjr2na60w3zh91pfny8y5qxr-qt-full-6.8.1/libexec/uic
shwet_autogen/timestamp: CMakeFiles/shwet_autogen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target shwet"
	/nix/store/yxf0cmyfrar671zqh0ml8pcw15mxk0mh-cmake-3.30.5/bin/cmake -E cmake_autogen /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles/shwet_autogen.dir/AutogenInfo.json Debug
	/nix/store/yxf0cmyfrar671zqh0ml8pcw15mxk0mh-cmake-3.30.5/bin/cmake -E touch /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/shwet_autogen/timestamp

shwet_autogen: CMakeFiles/shwet_autogen
shwet_autogen: shwet_autogen/timestamp
shwet_autogen: CMakeFiles/shwet_autogen.dir/build.make
.PHONY : shwet_autogen

# Rule to build all files generated by this target.
CMakeFiles/shwet_autogen.dir/build: shwet_autogen
.PHONY : CMakeFiles/shwet_autogen.dir/build

CMakeFiles/shwet_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/shwet_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/shwet_autogen.dir/clean

CMakeFiles/shwet_autogen.dir/depend:
	cd /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/prayag/me/programming/in-progress/qt/shwet /home/prayag/me/programming/in-progress/qt/shwet /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles/shwet_autogen.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/shwet_autogen.dir/depend

