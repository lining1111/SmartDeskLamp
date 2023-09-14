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
find_package(yaml-cpp REQUIRED)

set(3rdLibs gflags::gflags SQLite::SQLite3 CURL::libcurl openssl::openssl
        libuuid::libuuid glog::glog fmt::fmt Poco::Poco xpack::xpack yaml-cpp)


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
include(utility)
include(CPM)

set(LIBS m pthread ${3rdLibs})

set(filtration_dirlist cmake-build-debug cmake-build-release-aarch64 .idea .git conan lib test cmake PaddleModels)
SUBDIRLISTINCLUDE(SRC_DIRS ${CMAKE_SOURCE_DIR} 2 ${filtration_dirlist})
message(STATUS "3.7 SRC_DIRS : ${SRC_DIRS}")
include_directories(.)


set(TARGET_OS "linux")
set(TARGET_ABI ${ARCH})
set(PADDLE_LITE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/PaddleLite")
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/OpenCV")

add_definitions(-D${TARGET_ABI})

message(STATUS "TARGET OS: ${TARGET_OS}")
message(STATUS "TARGET ABI: ${TARGET_ABI}")
message(STATUS "PADDLE LITE DIR: ${PADDLE_LITE_DIR}")
message(STATUS "OpenCV DIR: ${OpenCV_DIR}")

include_directories(${PADDLE_LITE_DIR}/${TARGET_OS}/${TARGET_ABI}/include)
link_directories(${PADDLE_LITE_DIR}/${TARGET_OS}/${TARGET_ABI}/lib)

#opencv
set(OpenCV_DIR "${OpenCV_DIR}/${TARGET_OS}/${TARGET_ABI}/lib/cmake/opencv4")
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

add_library(
        # Sets the name of the library.
        paddle_api_shared
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        IMPORTED)
set_target_properties(
        # Specifies the target library.
        paddle_api_shared
        # Specifies the parameter you want to define.
        PROPERTIES
        IMPORTED_LOCATION
        ${PADDLE_LITE_DIR}/${TARGET_OS}/${TARGET_ABI}/lib/libpaddle_full_api_shared.so
        # Provides the path to the library you want to import.
)

foreach (src_dir ${SRC_DIRS})
    aux_source_directory(${src_dir} SRC_FILES)
endforeach (src_dir)
list(REMOVE_ITEM SRC_FILES ./main.cpp)

add_executable(SmartDeskLamp main.cpp ${SRC_FILES})

set(LIBS ${LIBS} paddle_api_shared ${OpenCV_LIBS})
target_link_libraries(SmartDeskLamp ${LIBS})
redefine_file_macro(SmartDeskLamp)

add_subdirectory(test)