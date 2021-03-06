CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(COCAINE-CORE)

SET(COCAINE_VERSION 0.8)

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/config.hpp.in"
    "${PROJECT_SOURCE_DIR}/include/cocaine/config.hpp")

FIND_PACKAGE(Boost 1.40.0 REQUIRED
    COMPONENTS
        filesystem
        program_options
        thread)

FIND_PACKAGE(OpenSSL REQUIRED)

INCLUDE(cmake/locate_library.cmake)

LOCATE_LIBRARY(LIBEV "ev++.h" "ev" "libev")
LOCATE_LIBRARY(LIBMSGPACK "msgpack.hpp" "msgpack")
LOCATE_LIBRARY(LIBZMQ "zmq.hpp" "zmq")
LOCATE_LIBRARY(LIBARCHIVE "archive.h" "archive")

IF(NOT APPLE)
    LOCATE_LIBRARY(LIBCGROUP "libcgroup.h" "cgroup")
    LOCATE_LIBRARY(LIBUUID "uuid/uuid.h" "uuid")

    SET(LIBCGROUP_LIBRARY "cgroup")
    SET(LIBUUID_LIBRARY "uuid")

    ADD_DEFINITIONS("-DHAVE_CGROUPS")
ENDIF()

INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
    ${LIBEV_INCLUDE_DIRS}
    ${LIBMSGPACK_INCLUDE_DIRS}
    ${LIBZMQ_INCLUDE_DIRS}
    ${LIBARCHIVE_INCLUDE_DIRS})

LINK_DIRECTORIES(
    ${Boost_LIBRARY_DIRS}
    ${LIBEV_LIBRARY_DIRS}
    ${LIBMSGPACK_LIBRARY_DIRS}
    ${LIBZMQ_LIBRARY_DIRS}
    ${LIBARCHIVE_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/foreign/jsoncpp-0.6.0-rc2/include)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)

ADD_LIBRARY(json
    foreign/jsoncpp-0.6.0-rc2/src/lib_json/json_value
    foreign/jsoncpp-0.6.0-rc2/src/lib_json/json_reader
    foreign/jsoncpp-0.6.0-rc2/src/lib_json/json_writer)

SET_TARGET_PROPERTIES(json PROPERTIES
    COMPILE_FLAGS -fPIC)

ADD_LIBRARY(cocaine-common SHARED
    src/auth
    src/context
    src/loggers/syslog
    src/logging
    src/repository
    src/storages/files)

TARGET_LINK_LIBRARIES(cocaine-common
    boost_filesystem-mt
    boost_system-mt
    boost_thread-mt
    crypto
    json
    ltdl
    msgpack
    zmq)

SET_TARGET_PROPERTIES(cocaine-common PROPERTIES
    VERSION 1)

ADD_LIBRARY(cocaine-core SHARED
    src/app
    src/engine
    src/job
    src/manifest
    src/master
    src/package
    src/policy)

TARGET_LINK_LIBRARIES(cocaine-core
    archive
    ${LIBCGROUP_LIBRARY}
    cocaine-common
    ev
    ${LIBUUID_LIBRARY})

SET_TARGET_PROPERTIES(cocaine-core PROPERTIES
    VERSION 1)

ADD_EXECUTABLE(cocaine-slave
    src/slave/slave
    src/slave/main)

ADD_EXECUTABLE(cocained
    src/server/server
    src/server/main)

TARGET_LINK_LIBRARIES(cocaine-slave
    boost_program_options-mt
    cocaine-core)

TARGET_LINK_LIBRARIES(cocained
    boost_program_options-mt
    cocaine-core)

ADD_SUBDIRECTORY(tools)

INSTALL(
    TARGETS
        cocaine-common
        cocaine-core
        cocaine-slave
        cocained
        json
    RUNTIME DESTINATION bin COMPONENT runtime
    LIBRARY DESTINATION lib COMPONENT runtime
    ARCHIVE DESTINATION lib COMPONENT developement)

INSTALL(
    DIRECTORY
        include/
        foreign/jsoncpp-0.6.0-rc2/include/
    DESTINATION include
    COMPONENT development)
