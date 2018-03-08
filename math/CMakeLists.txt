cmake_minimum_required (VERSION 3.6)

# This will set the default RAM used by the application
set(SOS_APP_RAM_SIZE 8192)

#Add sources to the project
set(SOURCES_PREFIX ${CMAKE_SOURCE_DIR}/src)
add_subdirectory(src)
list(APPEND SOS_APP_SOURCELIST ${SOURCES})

if( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin" )
  set(SOS_TOOLCHAIN_CMAKE_PATH /Applications/StratifyLabs-SDK/Tools/gcc/arm-none-eabi/cmake)
elseif( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows" )
	set(SOS_TOOLCHAIN_CMAKE_PATH C:/StratifyLabs-SDK/Tools/gcc/arm-none-eabi/cmake)
else()
  message(FATAL_ERROR "Cross compiling is not yet supportd on: " ${CMAKE_HOST_SYSTEM_NAME})
endif()
set(CMAKE_TOOLCHAIN_FILE ${SOS_TOOLCHAIN_CMAKE_PATH}/sos-app-toolchain.cmake)

get_filename_component(SOS_APP_NAME ${CMAKE_SOURCE_DIR} NAME)
project(${SOS_APP_NAME} CXX C)

message(STATUS "Building App sources: " ${SOS_APP_SOURCELIST})

include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-app-std.cmake)
