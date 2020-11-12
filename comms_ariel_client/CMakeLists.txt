PROJECT(rpc_client)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# Protobuf
SET(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11")
SET(COMPILE_FLAGS "-I/usr/local/include -pthread ")
SET(LINK_FLAGS "-L/usr/local/lib -lprotobuf -pthread -lpthread")

include(FindProtobuf)
#ADD_SUBDIRECTORY(proto)
add_library(proto_client proto/comms_ariel.pb.cc)
add_library(messages_client src/messages.cpp)
add_library(connect_client src/connect.cpp)
# Boost
find_package(Boost 1.55.0 REQUIRED COMPONENTS system filesystem)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIRS})
ADD_EXECUTABLE(main_client src/main.cpp)
link_directories(${Boost_LIBRARY_DIRS})
TARGET_LINK_LIBRARIES(main_client proto_client messages_client connect_client 
                      ${PROTOBUF_LIBRARY} ${Boost_LIBRARIES})
