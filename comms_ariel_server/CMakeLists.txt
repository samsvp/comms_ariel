PROJECT(comms_ariel_server)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# Protobuf
SET(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11")
SET(COMPILE_FLAGS "-I/usr/local/include -pthread ")
SET(LINK_FLAGS "-L/usr/local/lib -lprotobuf -pthread -lpthread")

include(FindProtobuf)
#ADD_SUBDIRECTORY(proto)
add_library(proto_server proto/comms_ariel.pb.cc)
add_library(connect_server src/connect.cpp)

find_package(catkin REQUIRED COMPONENTS 
    roscpp
    rospy
    std_msgs
    geometry_msgs
)

catkin_package(
  CATKIN_DEPENDS roscpp rospy std_msgs
  DEPENDS system_lib
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})
ADD_EXECUTABLE(main_server src/main.cpp)
link_directories(${Boost_LIBRARY_DIRS})
TARGET_LINK_LIBRARIES(connect_server proto_server ${PROTOBUF_LIBRARY} ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(main_server proto_server connect_server 
                        ${PROTOBUF_LIBRARY} ${Boost_LIBRARIES} ${catkin_LIBRARIES})