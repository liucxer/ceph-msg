cmake_minimum_required(VERSION 3.19)
project(ceph_msg)

include_directories ("${PROJECT_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD 17)
set(ENV{PKG_CONFIG_PATH} "/usr/local/Cellar/openssl@1.1/1.1.1k/lib/pkgconfig:/usr/local/Cellar/nss/3.64/lib/pkgconfig")
FIND_PACKAGE(PkgConfig REQUIRED)

set(msg_srcs
        msg/DispatchQueue.cc
        msg/Message.cc
        msg/Messenger.cc
        msg/QueueStrategy.cc
        msg/msg_types.cc
        msg/simple/Accepter.cc
        msg/simple/Pipe.cc
        msg/simple/PipeConnection.cc
        msg/simple/SimpleMessenger.cc include/acconfig.h)

list(APPEND msg_srcs
        msg/async/AsyncConnection.cc
        msg/async/AsyncMessenger.cc
        msg/async/Protocol.cc
        msg/async/ProtocolV1.cc
        msg/async/ProtocolV2.cc
        msg/async/Event.cc
        msg/async/EventSelect.cc
        msg/async/PosixStack.cc
        msg/async/Stack.cc
        msg/async/crypto_onwire.cc
        msg/async/net_handler.cc)

list(APPEND msg_srcs msg/async/EventKqueue.cc)
add_library(ceph_msg OBJECT ${msg_srcs})

# Boost
find_package(Boost REQUIRED COMPONENTS system thread)
message("Boost_INCLUDE_DIRS:")
message(STATUS ${Boost_INCLUDE_DIRS})
message("Boost_LIBRARIES:")
message(STATUS ${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS})

# nss
PKG_SEARCH_MODULE(PKG_NSS REQUIRED nss)
message("PKG_NSS_INCLUDE_DIRS:")
message(STATUS ${PKG_NSS_INCLUDE_DIRS})
message("PKG_NSS_LIBRARIES:")
message(STATUS ${PKG_NSS_LIBRARIES})
include_directories(${PKG_NSS_INCLUDE_DIRS})

# openssl
PKG_SEARCH_MODULE(PKG_OpenSSL REQUIRED openssl)
message("PKG_OpenSSL_INCLUDE_DIRS:")
message(STATUS ${PKG_OpenSSL_INCLUDE_DIRS})
message("PKG_OpenSSL_LIBRARIES:")
message(STATUS ${PKG_OpenSSL_LIBRARIES})
include_directories(${PKG_OpenSSL_INCLUDE_DIRS})

target_link_libraries(ceph_msg ${Boost_LIBRARIES})
target_link_libraries(ceph_msg ${PKG_OpenSSL_LIBRARIES})
target_link_libraries(ceph_msg ${PKG_NSS_LIBRARIES})