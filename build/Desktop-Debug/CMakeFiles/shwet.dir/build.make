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
CMAKE_COMMAND = /nix/store/4k2h9qsps52n1v56zq03yi67dwmxa81i-cmake-3.30.5/bin/cmake

# The command to remove a file.
RM = /nix/store/4k2h9qsps52n1v56zq03yi67dwmxa81i-cmake-3.30.5/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/prayag/me/programming/in-progress/qt/shwet

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug

# Include any dependencies generated for this target.
include CMakeFiles/shwet.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/shwet.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/shwet.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/shwet.dir/flags.make

shwet_autogen/timestamp: /nix/store/4v8zscynk625j9nrcf40cxhgcc1biq09-qt-full-6.8.0/libexec/moc
shwet_autogen/timestamp: /nix/store/4v8zscynk625j9nrcf40cxhgcc1biq09-qt-full-6.8.0/libexec/uic
shwet_autogen/timestamp: CMakeFiles/shwet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target shwet"
	/nix/store/4k2h9qsps52n1v56zq03yi67dwmxa81i-cmake-3.30.5/bin/cmake -E cmake_autogen /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles/shwet_autogen.dir/AutogenInfo.json Debug
	/nix/store/4k2h9qsps52n1v56zq03yi67dwmxa81i-cmake-3.30.5/bin/cmake -E touch /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/shwet_autogen/timestamp

shwet_autogen/F4FAE6NH3Q/qrc_res.cpp: /home/prayag/me/programming/in-progress/qt/shwet/src/resources/res.qrc
shwet_autogen/F4FAE6NH3Q/qrc_res.cpp: CMakeFiles/shwet_autogen.dir/AutoRcc_res_F4FAE6NH3Q_Info.json
shwet_autogen/F4FAE6NH3Q/qrc_res.cpp: /home/prayag/me/programming/in-progress/qt/shwet/src/resources/style.qss
shwet_autogen/F4FAE6NH3Q/qrc_res.cpp: /nix/store/4v8zscynk625j9nrcf40cxhgcc1biq09-qt-full-6.8.0/libexec/rcc
shwet_autogen/F4FAE6NH3Q/qrc_res.cpp: /nix/store/4v8zscynk625j9nrcf40cxhgcc1biq09-qt-full-6.8.0/libexec/rcc
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Automatic RCC for src/resources/res.qrc"
	/nix/store/4k2h9qsps52n1v56zq03yi67dwmxa81i-cmake-3.30.5/bin/cmake -E cmake_autorcc /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles/shwet_autogen.dir/AutoRcc_res_F4FAE6NH3Q_Info.json Debug

CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.o: CMakeFiles/shwet.dir/flags.make
CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.o: shwet_autogen/mocs_compilation.cpp
CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.o: CMakeFiles/shwet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.o"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.o -MF CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.o -c /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/shwet_autogen/mocs_compilation.cpp

CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.i"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/shwet_autogen/mocs_compilation.cpp > CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.i

CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.s"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/shwet_autogen/mocs_compilation.cpp -o CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.s

CMakeFiles/shwet.dir/src/main.cpp.o: CMakeFiles/shwet.dir/flags.make
CMakeFiles/shwet.dir/src/main.cpp.o: /home/prayag/me/programming/in-progress/qt/shwet/src/main.cpp
CMakeFiles/shwet.dir/src/main.cpp.o: CMakeFiles/shwet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/shwet.dir/src/main.cpp.o"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/shwet.dir/src/main.cpp.o -MF CMakeFiles/shwet.dir/src/main.cpp.o.d -o CMakeFiles/shwet.dir/src/main.cpp.o -c /home/prayag/me/programming/in-progress/qt/shwet/src/main.cpp

CMakeFiles/shwet.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/shwet.dir/src/main.cpp.i"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/prayag/me/programming/in-progress/qt/shwet/src/main.cpp > CMakeFiles/shwet.dir/src/main.cpp.i

CMakeFiles/shwet.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/shwet.dir/src/main.cpp.s"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/prayag/me/programming/in-progress/qt/shwet/src/main.cpp -o CMakeFiles/shwet.dir/src/main.cpp.s

CMakeFiles/shwet.dir/src/window/window.cpp.o: CMakeFiles/shwet.dir/flags.make
CMakeFiles/shwet.dir/src/window/window.cpp.o: /home/prayag/me/programming/in-progress/qt/shwet/src/window/window.cpp
CMakeFiles/shwet.dir/src/window/window.cpp.o: CMakeFiles/shwet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/shwet.dir/src/window/window.cpp.o"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/shwet.dir/src/window/window.cpp.o -MF CMakeFiles/shwet.dir/src/window/window.cpp.o.d -o CMakeFiles/shwet.dir/src/window/window.cpp.o -c /home/prayag/me/programming/in-progress/qt/shwet/src/window/window.cpp

CMakeFiles/shwet.dir/src/window/window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/shwet.dir/src/window/window.cpp.i"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/prayag/me/programming/in-progress/qt/shwet/src/window/window.cpp > CMakeFiles/shwet.dir/src/window/window.cpp.i

CMakeFiles/shwet.dir/src/window/window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/shwet.dir/src/window/window.cpp.s"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/prayag/me/programming/in-progress/qt/shwet/src/window/window.cpp -o CMakeFiles/shwet.dir/src/window/window.cpp.s

CMakeFiles/shwet.dir/src/components/cbutton.cpp.o: CMakeFiles/shwet.dir/flags.make
CMakeFiles/shwet.dir/src/components/cbutton.cpp.o: /home/prayag/me/programming/in-progress/qt/shwet/src/components/cbutton.cpp
CMakeFiles/shwet.dir/src/components/cbutton.cpp.o: CMakeFiles/shwet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/shwet.dir/src/components/cbutton.cpp.o"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/shwet.dir/src/components/cbutton.cpp.o -MF CMakeFiles/shwet.dir/src/components/cbutton.cpp.o.d -o CMakeFiles/shwet.dir/src/components/cbutton.cpp.o -c /home/prayag/me/programming/in-progress/qt/shwet/src/components/cbutton.cpp

CMakeFiles/shwet.dir/src/components/cbutton.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/shwet.dir/src/components/cbutton.cpp.i"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/prayag/me/programming/in-progress/qt/shwet/src/components/cbutton.cpp > CMakeFiles/shwet.dir/src/components/cbutton.cpp.i

CMakeFiles/shwet.dir/src/components/cbutton.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/shwet.dir/src/components/cbutton.cpp.s"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/prayag/me/programming/in-progress/qt/shwet/src/components/cbutton.cpp -o CMakeFiles/shwet.dir/src/components/cbutton.cpp.s

CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.o: CMakeFiles/shwet.dir/flags.make
CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.o: /home/prayag/me/programming/in-progress/qt/shwet/src/components/ctoolbar.cpp
CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.o: CMakeFiles/shwet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.o"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.o -MF CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.o.d -o CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.o -c /home/prayag/me/programming/in-progress/qt/shwet/src/components/ctoolbar.cpp

CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.i"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/prayag/me/programming/in-progress/qt/shwet/src/components/ctoolbar.cpp > CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.i

CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.s"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/prayag/me/programming/in-progress/qt/shwet/src/components/ctoolbar.cpp -o CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.s

CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.o: CMakeFiles/shwet.dir/flags.make
CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.o: shwet_autogen/F4FAE6NH3Q/qrc_res.cpp
CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.o: CMakeFiles/shwet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.o"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.o -MF CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.o.d -o CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.o -c /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp

CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.i"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp > CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.i

CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.s"
	/etc/profiles/per-user/prayag/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp -o CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.s

# Object files for target shwet
shwet_OBJECTS = \
"CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/shwet.dir/src/main.cpp.o" \
"CMakeFiles/shwet.dir/src/window/window.cpp.o" \
"CMakeFiles/shwet.dir/src/components/cbutton.cpp.o" \
"CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.o" \
"CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.o"

# External object files for target shwet
shwet_EXTERNAL_OBJECTS =

shwet: CMakeFiles/shwet.dir/shwet_autogen/mocs_compilation.cpp.o
shwet: CMakeFiles/shwet.dir/src/main.cpp.o
shwet: CMakeFiles/shwet.dir/src/window/window.cpp.o
shwet: CMakeFiles/shwet.dir/src/components/cbutton.cpp.o
shwet: CMakeFiles/shwet.dir/src/components/ctoolbar.cpp.o
shwet: CMakeFiles/shwet.dir/shwet_autogen/F4FAE6NH3Q/qrc_res.cpp.o
shwet: CMakeFiles/shwet.dir/build.make
shwet: /nix/store/4v8zscynk625j9nrcf40cxhgcc1biq09-qt-full-6.8.0/lib/libQt6Widgets.so.6.8.0
shwet: /nix/store/4v8zscynk625j9nrcf40cxhgcc1biq09-qt-full-6.8.0/lib/libQt6Gui.so.6.8.0
shwet: /nix/store/4v8zscynk625j9nrcf40cxhgcc1biq09-qt-full-6.8.0/lib/libGLX.so
shwet: /nix/store/4v8zscynk625j9nrcf40cxhgcc1biq09-qt-full-6.8.0/lib/libOpenGL.so
shwet: /nix/store/4v8zscynk625j9nrcf40cxhgcc1biq09-qt-full-6.8.0/lib/libQt6Core.so.6.8.0
shwet: CMakeFiles/shwet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable shwet"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/shwet.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/shwet.dir/build: shwet
.PHONY : CMakeFiles/shwet.dir/build

CMakeFiles/shwet.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/shwet.dir/cmake_clean.cmake
.PHONY : CMakeFiles/shwet.dir/clean

CMakeFiles/shwet.dir/depend: shwet_autogen/F4FAE6NH3Q/qrc_res.cpp
CMakeFiles/shwet.dir/depend: shwet_autogen/timestamp
	cd /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/prayag/me/programming/in-progress/qt/shwet /home/prayag/me/programming/in-progress/qt/shwet /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug /home/prayag/me/programming/in-progress/qt/shwet/build/Desktop-Debug/CMakeFiles/shwet.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/shwet.dir/depend

