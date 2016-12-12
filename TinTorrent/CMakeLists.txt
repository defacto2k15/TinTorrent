cmake_minimum_required(VERSION 3.6)
project(TinTorrent)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib)

set(SOURCE_FILES main.cpp TinProtocolLib/TinServerSocket.cpp TinProtocolLib/TinServerSocket.h TinProtocolLib/Messages/MessageResourceRequest.cpp
        TinProtocolLib/Messages/MessageResourceRequest.h TinProtocolLib/Messages/MessageStartSendingRequest.cpp
        TinProtocolLib/Messages/MessageStartSendingRequest.h TinProtocolLib/Messages/MessageEnum.cpp TinProtocolLib/Messages/MessageEnum.h
        TinProtocolLib/SegmentInfo.cpp TinProtocolLib/SegmentInfo.h TinProtocolLib/Messages/MessageClose.cpp TinProtocolLib/Messages/MessageClose.h
        TinProtocolLib/Messages/MessageResourceResponse.cpp TinProtocolLib/Messages/MessageResourceResponse.h Assertions/Assertions.cpp
        Assertions/Assertions.h Common/Typedefs.h TinProtocolLib/TinConnectedServerSocket.cpp TinProtocolLib/TinConnectedServerSocket.h
        Common/Buffer.cpp Common/Buffer.h Common/InMemoryBuffer.cpp Common/InMemoryBuffer.h TinProtocolLib/Messages/SegmentResponse.cpp
        TinProtocolLib/Messages/SegmentResponse.h TinProtocolLib/SocketWrapper.cpp TinProtocolLib/SocketWrapper.h TinProtocolLib/Messages/Message.cpp
        TinProtocolLib/Messages/Message.h ModelEntities/Resource.cpp ModelEntities/Resource.h
        TinProtocolLib/TinClientSocket.cpp
        TinProtocolLib/TinClientSocket.h ModelEntities/TinAddress.cpp ModelEntities/TinAddress.h
        TinProtocolLib/TinConnectedClientSocket.cpp TinProtocolLib/TinConnectedClientSocket.h TinProtocolLib/TinConnectedSocket.cpp
        TinProtocolLib/TinConnectedSocket.h Common/ExHelp.cpp Common/ExHelp.h Multithreading/BlockingElement.cpp Multithreading/BlockingElement.h Multithreading/MultithreadingSequence.cpp Multithreading/MultithreadingSequence.h Test/MessagesTest/MessageResourceRequestTest.cpp TinProtocolLib/Messages/MessageType.cpp TinProtocolLib/Messages/MessageType.h Common/StringHelp.cpp Common/StringHelp.h)
add_executable(TinTorrent ${SOURCE_FILES})

find_package (Threads)
target_link_libraries (TinTorrent ${CMAKE_THREAD_LIBS_INIT})

option(BuildTests "BuildTests" OFF)
if(BuildTests)

    set(TEST_EXECUTABLE_NAME "tests")
    set(TEST_FILES   Test/TinProtocolLib/TinSocketTest.cpp Test/TinProtocolLib/TinSocketTest.h Test/testMain.cpp)
    # remove main.cpp file
    list(REMOVE_ITEM SOURCE_FILES "main.cpp")
    add_executable("${TEST_EXECUTABLE_NAME}" ${TEST_FILES} ${SOURCE_FILES} )
    target_link_libraries("${TEST_EXECUTABLE_NAME}" ${CMAKE_SOURCE_DIR}/lib/libgtest.a)
    target_link_libraries ("${TEST_EXECUTABLE_NAME}" ${CMAKE_THREAD_LIBS_INIT})
endif()