# Install script for directory: /home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/build/release-install-cpp11")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/build/release-cpp11/lib/libmuduo_base.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/muduo/base" TYPE FILE FILES
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/AsyncLogging.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/Atomic.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/BlockingQueue.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/BoundedBlockingQueue.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/Condition.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/CountDownLatch.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/CurrentThread.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/Date.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/Exception.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/FileUtil.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/GzipFile.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/LogFile.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/LogStream.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/Logging.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/Mutex.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/ProcessInfo.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/Singleton.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/StringPiece.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/Thread.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/ThreadLocal.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/ThreadLocalSingleton.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/ThreadPool.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/TimeZone.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/Timestamp.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/Types.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/WeakCallback.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/copyable.h"
    "/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/muduo/muduo/base/noncopyable.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/tan/桌面/IMServer/IMServer/out/build/ubuntu/build/release-cpp11/muduo/base/tests/cmake_install.cmake")

endif()

