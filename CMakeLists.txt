cmake_minimum_required(VERSION 2.6)
PROJECT(irrlicht-game)

#Set default build type to Debug
SET(DEFAULT_BUILD_TYPE "Release")
SET(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE
      STRING "Choose the type of build." FORCE)

#set irrlicht location variables
set(IRRLICHT_INCLUDE_DIR "/usr/include/irrlicht" CACHE FILEPATH "Path to Irrlicht include")
set(IRRLICHT_LIB_DIR "/usr/lib" CACHE FILEPATH "Path to Irrlicht lib")
set(RAPIDJSON_INCLUDE_DIR "rapidjson" CACHE FILEPATH "Path to rapidjson includes")

#set source and header files
FILE(GLOB SOURCES src/*.cpp src/ui/*.cpp src/item/*.cpp src/ia/*.cpp src/entity/*.cpp)
FILE(GLOB HEADERS src/*.hpp)

#set build and link
INCLUDE_DIRECTORIES(${IRRLICHT_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIR})
ADD_EXECUTABLE(irrlicht-game ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES(irrlicht-game	libIrrlicht.so)

#build flags
SET(CMAKE_CXX_FLAGS "-O3 -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG  "-g")