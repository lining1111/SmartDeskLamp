cmake_minimum_required(VERSION 3.25)
project(SmartDeskLamp VERSION 0.0.1)

string(TIMESTAMP COMPILE_TIME ${CMAKE_PROJECT_VERSION}_%Y%m%d_%H.%M.%S)
set(build_time ${COMPILE_TIME})
configure_file("${PROJECT_SOURCE_DIR}/version.h.in" "${PROJECT_SOURCE_DIR}/version.h")

set(CMAKE_CXX_STANDARD 11)

add_definitions(-D${ARCH})

find_package(gflags REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(libuuid REQUIRED)
find_package(glog REQUIRED)
find_package(fmt REQUIRED)
find_package(Poco REQUIRED)
find_package(xpack REQUIRED)

set(3rdLibs gflags::gflags SQLite::SQLite3 CURL::libcurl openssl::openssl
        libuuid::libuuid glog::glog fmt::fmt Poco::Poco xpack::xpack)


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
include(utility)
include(CPM)

set(LIBS m pthread ${3rdLibs})

set(filtration_dirlist cmake-build-debug cmake-build-release-aarch64 .idea .git conan lib test cmake)
SUBDIRLISTINCLUDE(SRC_DIRS ${CMAKE_SOURCE_DIR} 2 ${filtration_dirlist})
message(STATUS "3.7 SRC_DIRS : ${SRC_DIRS}")
include_directories(.)


#opencv
set(OpenCV_DIR "lib/${ARCH}/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)
if (OpenCV_FOUND OR OpenCV_CXX_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    message(STATUS "OpenCV library status:")
    message(STATUS "    version: ${OpenCV_VERSION}")
    message(STATUS "    libraries: ${OpenCV_LIBS}")
    message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
else ()
    message(FATAL_ERROR "OpenCV not found!")
    return()
endif ()

foreach (src_dir ${SRC_DIRS})
    aux_source_directory(${src_dir} SRC_FILES)
endforeach (src_dir)
list(REMOVE_ITEM SRC_FILES ./main.cpp)

add_executable(SmartDeskLamp main.cpp ${SRC_FILES})

target_link_libraries(SmartDeskLamp ${LIBS} ${OpenCV_LIBS})
redefine_file_macro(SmartDeskLamp)