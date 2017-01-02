//
// Created by defacto on 02.01.17.
//

#ifndef TINTORRENT_KERNELTEST_H
#define TINTORRENT_KERNELTEST_H

#include <gtest/gtest.h>
#include <Kernel/Kernel.h>

#define TEST_WORKING_DIRECTORY L"/home/defacto/ClionProjects/TinTorrent/Test/TestWorkingDirectory"

class KernelTest : public ::testing::Test{

};

TEST_F( KernelTest, KernelInit){
	Kernel kernel;
	kernel.startApp( StringHelp::toUtf8(std::wstring(TEST_WORKING_DIRECTORY)), "");
}


#endif //TINTORRENT_KERNELTEST_H
