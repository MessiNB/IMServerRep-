# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tan/桌面/IMServer/IMServer/out/build/ubuntu/build/release-cpp11

# Utility rule file for protobuf_rpc_all.

# Include the progress variables for this target.
include examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all.dir/progress.make

examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all: lib/libmuduo_protorpc.a
examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all: bin/protobuf_rpc_balancer
examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all: bin/protobuf_rpc_balancer_raw
examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all: bin/protobuf_rpc_echo_client
examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all: bin/protobuf_rpc_echo_server
examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all: bin/protobuf_rpc_resolver_client
examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all: bin/protobuf_rpc_resolver_server
examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all: bin/protobuf_rpc_sudoku_client
examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all: bin/protobuf_rpc_sudoku_server


protobuf_rpc_all: examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all
protobuf_rpc_all: examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all.dir/build.make

.PHONY : protobuf_rpc_all

# Rule to build all files generated by this target.
examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all.dir/build: protobuf_rpc_all

.PHONY : examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all.dir/build

examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all.dir/clean:
	cd /home/tan/桌面/IMServer/IMServer/out/build/ubuntu/build/release-cpp11/examples/protobuf/rpc && $(CMAKE_COMMAND) -P CMakeFiles/protobuf_rpc_all.dir/cmake_clean.cmake
.PHONY : examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all.dir/clean

examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all.dir/depend:
	cd /home/tan/桌面/IMServer/IMServer/out/build/ubuntu/build/release-cpp11 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo /home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/examples/protobuf/rpc /home/tan/桌面/IMServer/IMServer/out/build/ubuntu/build/release-cpp11 /home/tan/桌面/IMServer/IMServer/out/build/ubuntu/build/release-cpp11/examples/protobuf/rpc /home/tan/桌面/IMServer/IMServer/out/build/ubuntu/build/release-cpp11/examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/protobuf/rpc/CMakeFiles/protobuf_rpc_all.dir/depend

