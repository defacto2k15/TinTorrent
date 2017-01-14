cmake_minimum_required(VERSION 3.1)
project(TinTorrent)

set(CMAKE_CXX_STANDARD 14)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
include_directories(lib)
include_directories(.)

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
        TinProtocolLib/TinConnectedSocket.h Common/ExHelp.cpp Common/ExHelp.h Multithreading/BlockingElement.cpp Multithreading/BlockingElement.h
        Multithreading/MultithreadingSequence.cpp Multithreading/MultithreadingSequence.h
        TinProtocolLib/Messages/MessageType.cpp TinProtocolLib/Messages/MessageType.h Common/StringHelp.cpp Common/StringHelp.h
        TinProtocolLib/Segment.cpp TinProtocolLib/Segment.h TinProtocolLib/MessageCloseException.cpp TinProtocolLib/MessageCloseException.h
        FileManager/FileInformationConsumer.h FileManager/WorkingDirectoryManager.cpp FileManager/WorkingDirectoryManager.h
        FileManager/ModelEntities/FileInfo.cpp FileManager/ModelEntities/FileInfo.h FileManager/ModelEntities/FileSegmentsInfo.cpp
        FileManager/ModelEntities/FileSegmentsInfo.h Test/Mocks/MockFileInformationConsumer.h Utils/MyString.cpp Utils/MyString.h
        Common/Constants.cpp Common/Constants.h Utils/ContainerUtils.cpp Utils/ContainerUtils.h Utils/StreamUtils.cpp Utils/StreamUtils.h
        Utils/BitUtils.cpp Utils/BitUtils.h Utils/SegmentUtils.cpp Utils/SegmentUtils.h Utils/FileUtils.cpp Utils/FileUtils.h
        FileManager/UpdateInfo.cpp FileManager/UpdateInfo.h FileManager/FileManager.cpp FileManager/FileManager.h Common/SegmentsSet.cpp
        Common/SegmentsSet.h Common/SegmentRange.cpp Common/SegmentRange.h Common/Exceptions/MissingRequiredFile.cpp
        Common/Exceptions/MissingRequiredFile.h TinProtocolLib/TinBroadcastSocket.cpp TinProtocolLib/TinBroadcastSocket.h
        TinProtocolLib/Messages/BroadcastMessage.cpp TinProtocolLib/Messages/BroadcastMessage.h TinProtocolLib/TinListeningUdpSocket.cpp
        TinProtocolLib/TinListeningUdpSocket.h Kernel/Kernel.cpp Kernel/Kernel.h Common/MultithreadingVector.cpp Common/MultithreadingVector.h
        TinProtocolLib/Threads/ThreadTinBroadcast.cpp TinProtocolLib/Threads/ThreadTinBroadcast.h Multithreading/ActionQueue.cpp
        Multithreading/ActionQueue.h TinProtocolLib/Threads/ThreadTinUdpListeningSocket.cpp TinProtocolLib/Threads/ThreadTinUdpListeningSocket.h
        TinProtocolLib/Threads/TinServerSocketThread.cpp TinProtocolLib/Threads/TinServerSocketThread.h
        TinProtocolLib/Threads/TinConnectedServerThread.cpp TinProtocolLib/Threads/TinConnectedServerThread.h
        TinProtocolLib/Threads/TinClientThread.cpp TinProtocolLib/Threads/TinClientThread.h FileManager/FileManagerThread.cpp
        View/View.h View/View.cpp View/Screen.h View/MainMenuScreen.h View/MainMenuScreen.cpp
		  View/LocalResourcesScreen.h View/LocalResourcesScreen.cpp
		  View/OtherClientsScreen.h View/OtherClientsScreen.cpp
		  View/StatusScreen.h View/StatusScreen.cpp
        FileManager/FileManagerThread.h Kernel/WorkingDirectoryState.cpp Kernel/WorkingDirectoryState.h Kernel/TinNetworkState.cpp
        Kernel/TinNetworkState.h Kernel/SegmentState.h Kernel/ClientThreadsCollection.cpp Kernel/ClientThreadsCollection.h
        Kernel/ServerThreadsCollection.cpp Kernel/ServerThreadsCollection.h ProgramInfoProvider/outStructures/OutLocalResource.cpp
        ProgramInfoProvider/outStructures/OutLocalResource.h ProgramInfoProvider/ProgramInfoProvider.cpp ProgramInfoProvider/ProgramInfoProvider.h
        TinProtocolLib/SocketCommunicationException.cpp TinProtocolLib/SocketCommunicationException.h Multithreading/ActionThread.cpp
        Multithreading/ActionThread.h Test/FileTestingBase.cpp Test/FileTestingBase.h
        ProgramInfoProvider/outStructures/OutTinResourcesInOtherClients.cpp ProgramInfoProvider/outStructures/OutTinResourcesInOtherClients.h
        ProgramInfoProvider/outStructures/OutClientConnectionInfo.cpp ProgramInfoProvider/outStructures/OutClientConnectionInfo.h
        ProgramInfoProvider/outStructures/OutServerConnectionInfo.cpp ProgramInfoProvider/outStructures/OutServerConnectionInfo.h
        Logging/LogBase.cpp Logging/LogBase.h Logging/LoggingMain.cpp Logging/LoggingMain.h
        ProgramInfoProvider/outStructures/OutResourcesBeingDownloaded.cpp ProgramInfoProvider/outStructures/OutResourcesBeingDownloaded.h
        Common/ConfigLoader.cpp Common/ConfigLoader.h Test/ScenarioLoader/ScenarioRunner.cpp Test/ScenarioLoader/ScenarioRunner.h Test/ScenarioLoader/ScenarioAction.cpp Test/ScenarioLoader/ScenarioAction.h)
add_executable(TinTorrent ${SOURCE_FILES})

target_compile_options( TinTorrent PRIVATE -Wall -Wextra )
find_package (Threads)
target_link_libraries (TinTorrent ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (TinTorrent ${CURSES_LIBRARIES})

option(BuildTests "BuildTests" OFF)
if(BuildTests)

    set(TEST_EXECUTABLE_NAME "tests")
    set(TEST_FILES  Test/MessagesTest/MessageResourceRequestTest.cpp  Test/TinProtocolLib/TinSocketTest.cpp
            Test/TinProtocolLib/TinSocketTest.h Test/testMain.cpp Test/FileManager/FileManagerTest.cpp
            Test/TinProtocolLib/TinBroadcastSocketTest.cpp Test/ThreadInTest.cpp Test/ThreadInTest.h
            Test/Kernel/KernelTest.cpp Test/Kernel/KernelTest.h Test/Kernel/WorkingDirectoryStateTest.cpp
            Test/Kernel/WorkingDirectoryStateTest.h)
    # remove main.cpp file
    list(REMOVE_ITEM SOURCE_FILES "main.cpp")

    add_executable("${TEST_EXECUTABLE_NAME}" ${TEST_FILES} ${SOURCE_FILES} )
    target_compile_options( "${TEST_EXECUTABLE_NAME}" PRIVATE -Wall -Wextra -Wfatal-errors)
    target_link_libraries("${TEST_EXECUTABLE_NAME}" ${CMAKE_SOURCE_DIR}/lib/libgtest.a)
    target_link_libraries("${TEST_EXECUTABLE_NAME}" ${CMAKE_SOURCE_DIR}/lib/libgmock.a)
    target_link_libraries ("${TEST_EXECUTABLE_NAME}" ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries ("${TEST_EXECUTABLE_NAME}" ${CURSES_LIBRARIES})
endif()