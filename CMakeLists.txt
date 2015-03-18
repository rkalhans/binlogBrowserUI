PROJECT(MYSQL_BINLOG_BROWSER)
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
set(MySQL_BINLOG_BROWSER_VERSION_MAJOR "0")
set(MySQL_BINLOG_BROWSER_VERSION_MINOR "1")
set(MRL_VERSION "${MySQL_BINLOG_BROWSER_VERSION_MAJOR}.${MySQL_BINLOG_BROWSER_VERSION_MINOR}")
set(CMAKE_VERSION_STRING "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_SOURCE_DIR}/cmake_Modules/")
SET (CMAKE_CXX_FLAGS
     -g)
if (UNIX)
ADD_CUSTOM_TARGET (distclean @echo cleaning for source distribution)
SET( CMAKE_FILES
   cmake.depends
   cmake.check_depends
   CMakeCache.txt
   cmake.check_cache
   ./*.cmake
   ./src/*.cmake
   src/cmake.depends
   src/cmake.check_depends
   src/CMakeCache.txt
   src/cmake.check_cache
   src/*.cmake
   src/CMakeFiles
   install_manifest.txt
   src/install_manifest.txt
   CMakeFiles)
SET( MAKE_FILES
   Makefile
   src/Makefile
   )
SET(OTHER_FILES
   core core.*
   gmon.out
   *~
   src/core src/core.*
   src/gmon.out
   src/__/
   )
SET( GENERATED_FILES
     src/*.cxx
     src/ui_MySQLbinlogbrowser.h
     src/*.cxx
     MYSQL_BINLOG_BROWSER-0.1.1-Linux.rpm
     MYSQL_BINLOG_BROWSER-0.1.1-Linux.tar.gz
     MYSQL_BINLOG_BROWSER-0.1.1-Linux.deb
     MYSQL_BINLOG_BROWSER-0.1.1-Linux.sh
     MYSQL_BINLOG_BROWSER-0.1.1-Linux.tar.Z
   )
SET( BINARY_FILES
   ${CMAKE_INSTALL_PREFIX}/mysql_binlog_browser
   src/mysql_binlog_browser)
  
ADD_CUSTOM_COMMAND(
    DEPENDS clean
    COMMENT "Removing generated files"
    COMMAND rm
    ARGS    -Rf ${GENERATED_FILES}
    TARGET  distclean)
ADD_CUSTOM_COMMAND(
    DEPENDS clean
    COMMENT "Removing cmake files"
    COMMAND rm
    ARGS    -Rf CMakeTmp ${CMAKE_FILES}
    TARGET  distclean)
ADD_CUSTOM_COMMAND(
    DEPENDS clean
    COMMENT "Removing  make files"
    COMMAND rm
    ARGS    -Rf ${MAKE_FILES}
    TARGET  distclean)
ADD_CUSTOM_COMMAND(
    DEPENDS clean
    COMMENT "Removing binary files"
    COMMAND rm
    ARGS    -Rf ${BINARY_FILES}
    TARGET  distclean)
ADD_CUSTOM_COMMAND(
    DEPENDS clean
    COMMENT "Removing other files"
    COMMAND rm
    ARGS    -Rf ${OTHER_FILES}
    TARGET  distclean)
endif(UNIX)
ADD_SUBDIRECTORY(src)

INSTALL (DIRECTORY  docs
         DESTINATION ${CMAKE_INSTALL_PREFIX}
         PATTERN .bzr EXCLUDE)
#CPACK

INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_GENERATOR "STGZ;TGZ;TZ;DEB;RPM")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Rohit Kalhans <rohit.kalhans@oracle.com>")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MySQL Binlog Browser")
SET(CPACK_PACKAGE_VENDOR "Oracle Corporation.")
SET(CPACK_PACKAGE_VERSION_MAJOR
    ${MySQL_BINLOG_BROWSER_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR
    ${MySQL_BINLOG_BROWSER_VERSION_MINOR})
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_STRIP_FILES "src/mysql_binlog_browser")
SET(CPACK_SOURCE_STRIP_FILES "")
SET(CPACK_PACKAGE_EXECUTABLES "mysql_binlog_browser" "mysql binlog browser")
INCLUDE(CPack)

