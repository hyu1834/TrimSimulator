CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(execName TS)

SET(EXECUTABLE_OUTPUT_PATH "./bin")
# Mandatory
#Third parties libraries
#find_package()

#header files directory
include_directories(include)

#third party libraries lib directory
#link_directories(${ZED_LIBRARY_DIR})

#source code directory
SET(SRC_FOLDER src)
#find all cpp files
FILE(GLOB_RECURSE SRC_FILES "${SRC_FOLDER}/*.cpp")
 
ADD_EXECUTABLE(${execName} ${SRC_FILES})

set_property(TARGET ${execName} PROPERTY OUTPUT_NAME ${execName})
add_definitions(-std=c++0x)# -m64)

# Add the required libraries for linking:
TARGET_LINK_LIBRARIES(${execName}
					 )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3" ) # Release Perf mode
