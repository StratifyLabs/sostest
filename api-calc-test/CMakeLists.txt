cmake_minimum_required (VERSION 3.6)


if( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin" )
  set(SOS_TOOLCHAIN_CMAKE_PATH /Applications/StratifyLabs-SDK/Tools/gcc/arm-none-eabi/cmake)
elseif( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows" )
	set(SOS_TOOLCHAIN_CMAKE_PATH C:/StratifyLabs-SDK/Tools/gcc/arm-none-eabi/cmake)
else()
  message(FATAL_ERROR "Cross compiling is not yet supportd on: " ${CMAKE_HOST_SYSTEM_NAME})
endif()

include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-variables.cmake)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-sdk.cmake)

if( ${SOS_BUILD_CONFIG} STREQUAL arm )
	set(CMAKE_TOOLCHAIN_FILE ${SOS_TOOLCHAIN_CMAKE_PATH}/sos-app-toolchain.cmake)
elseif( ${SOS_BUILD_CONFIG} STREQUAL link )
	set(CMAKE_TOOLCHAIN_FILE ${SOS_TOOLCHAIN_CMAKE_PATH}/link-toolchain.cmake)
endif()

# This will set the default RAM used by the application
set(SOS_RAM_SIZE 32768)

#Add sources to the project
sos_sdk_add_subdirectory(SOS_SOURCELIST src)
list(APPEND
	SOS_SOURCELIST
	${CMAKE_SOURCE_DIR}/sl_settings.json)

get_filename_component(SOS_NAME ${CMAKE_SOURCE_DIR} NAME)
project(${SOS_NAME} CXX C)

if( ${SOS_BUILD_CONFIG} STREQUAL arm )
	include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-app-std.cmake)
elseif( ${SOS_BUILD_CONFIG} STREQUAL link )
	set(SOS_ARCH link)
	set(SOS_OPTION "")
	set(SOS_CONFIG release)
	include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-app.cmake)
endif()
