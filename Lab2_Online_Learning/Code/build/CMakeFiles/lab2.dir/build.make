# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code/build

# Include any dependencies generated for this target.
include CMakeFiles/lab2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lab2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lab2.dir/flags.make

CMakeFiles/lab2.dir/src/main.cpp.o: CMakeFiles/lab2.dir/flags.make
CMakeFiles/lab2.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lab2.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lab2.dir/src/main.cpp.o -c /home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code/src/main.cpp

CMakeFiles/lab2.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lab2.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code/src/main.cpp > CMakeFiles/lab2.dir/src/main.cpp.i

CMakeFiles/lab2.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lab2.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code/src/main.cpp -o CMakeFiles/lab2.dir/src/main.cpp.s

CMakeFiles/lab2.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/lab2.dir/src/main.cpp.o.requires

CMakeFiles/lab2.dir/src/main.cpp.o.provides: CMakeFiles/lab2.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/lab2.dir/build.make CMakeFiles/lab2.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/lab2.dir/src/main.cpp.o.provides

CMakeFiles/lab2.dir/src/main.cpp.o.provides.build: CMakeFiles/lab2.dir/src/main.cpp.o


CMakeFiles/lab2.dir/src/visualize.cpp.o: CMakeFiles/lab2.dir/flags.make
CMakeFiles/lab2.dir/src/visualize.cpp.o: ../src/visualize.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/lab2.dir/src/visualize.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lab2.dir/src/visualize.cpp.o -c /home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code/src/visualize.cpp

CMakeFiles/lab2.dir/src/visualize.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lab2.dir/src/visualize.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code/src/visualize.cpp > CMakeFiles/lab2.dir/src/visualize.cpp.i

CMakeFiles/lab2.dir/src/visualize.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lab2.dir/src/visualize.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code/src/visualize.cpp -o CMakeFiles/lab2.dir/src/visualize.cpp.s

CMakeFiles/lab2.dir/src/visualize.cpp.o.requires:

.PHONY : CMakeFiles/lab2.dir/src/visualize.cpp.o.requires

CMakeFiles/lab2.dir/src/visualize.cpp.o.provides: CMakeFiles/lab2.dir/src/visualize.cpp.o.requires
	$(MAKE) -f CMakeFiles/lab2.dir/build.make CMakeFiles/lab2.dir/src/visualize.cpp.o.provides.build
.PHONY : CMakeFiles/lab2.dir/src/visualize.cpp.o.provides

CMakeFiles/lab2.dir/src/visualize.cpp.o.provides.build: CMakeFiles/lab2.dir/src/visualize.cpp.o


# Object files for target lab2
lab2_OBJECTS = \
"CMakeFiles/lab2.dir/src/main.cpp.o" \
"CMakeFiles/lab2.dir/src/visualize.cpp.o"

# External object files for target lab2
lab2_EXTERNAL_OBJECTS =

../bin/lab2: CMakeFiles/lab2.dir/src/main.cpp.o
../bin/lab2: CMakeFiles/lab2.dir/src/visualize.cpp.o
../bin/lab2: CMakeFiles/lab2.dir/build.make
../bin/lab2: /usr/local/lib/libopencv_stitching.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_superres.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_videostab.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_cvv.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_xobjdetect.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_aruco.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_hfs.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_bgsegm.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_fuzzy.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_optflow.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_saliency.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_ximgproc.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_hdf.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_rgbd.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_bioinspired.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_surface_matching.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_xphoto.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_face.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_dpm.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_sfm.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_reg.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_line_descriptor.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_dnn_objdetect.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_freetype.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_structured_light.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_stereo.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_ccalib.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_img_hash.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_photo.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_tracking.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_datasets.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_text.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_plot.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_objdetect.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_xfeatures2d.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_ml.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_shape.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_video.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_dnn.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_phase_unwrapping.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_calib3d.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_features2d.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_flann.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_highgui.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_videoio.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_imgcodecs.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_imgproc.so.3.4.1
../bin/lab2: /usr/local/lib/libopencv_core.so.3.4.1
../bin/lab2: CMakeFiles/lab2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../bin/lab2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lab2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lab2.dir/build: ../bin/lab2

.PHONY : CMakeFiles/lab2.dir/build

CMakeFiles/lab2.dir/requires: CMakeFiles/lab2.dir/src/main.cpp.o.requires
CMakeFiles/lab2.dir/requires: CMakeFiles/lab2.dir/src/visualize.cpp.o.requires

.PHONY : CMakeFiles/lab2.dir/requires

CMakeFiles/lab2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lab2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lab2.dir/clean

CMakeFiles/lab2.dir/depend:
	cd /home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code /home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code /home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code/build /home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code/build /home/osboxes/Documents/Probabilistic-Robotics/Lab2_Online_Learning/Code/build/CMakeFiles/lab2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lab2.dir/depend

