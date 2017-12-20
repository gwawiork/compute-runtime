/*
 * Copyright (c) 2017, Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "unit_tests/fixtures/two_walker_fixture.h"
#include "runtime/helpers/kernel_commands.h"
#include "test.h"

using namespace OCLRT;

typedef TwoWalkerTest<HelloWorldFixtureFactory> IOQWithTwoWalkers;

HWTEST_F(IOQWithTwoWalkers, shouldHaveTwoWalkers) {
    enqueueTwoKernels<FamilyType>();

    EXPECT_NE(itorWalker1, itorWalker2);
}

HWTEST_F(IOQWithTwoWalkers, shouldHaveOnePS) {
    enqueueTwoKernels<FamilyType>();

    auto itorCmd1 = find<typename FamilyType::PIPELINE_SELECT *>(cmdList.begin(), cmdList.end());
    auto itorCmd2 = itorCmd1;
    ++itorCmd2;
    itorCmd2 = find<typename FamilyType::PIPELINE_SELECT *>(itorCmd2, cmdList.end());

    EXPECT_EQ(cmdList.end(), itorCmd2);
}

HWTEST_F(IOQWithTwoWalkers, shouldHaveOneVFEState) {
    enqueueTwoKernels<FamilyType>();

    auto numCommands = getCommandsList<typename FamilyType::MEDIA_VFE_STATE>().size();
    EXPECT_EQ(1u, numCommands);
}

HWTEST_F(IOQWithTwoWalkers, shouldHaveAPipecontrolBetweenWalkers2) {
    enqueueTwoKernels<FamilyType>();
    auto &commandStreamReceiver = pDevice->getCommandStreamReceiver();

    typedef typename FamilyType::PIPE_CONTROL PIPE_CONTROL;

    auto WaNeeded = KernelCommandsHelper<FamilyType>::isPipeControlWArequired();

    auto itorCmd = find<PIPE_CONTROL *>(itorWalker1, itorWalker2);
    ASSERT_NE(itorWalker2, itorCmd);

    auto pipeControl = genCmdCast<PIPE_CONTROL *>(*itorCmd);

    if (WaNeeded) {
        EXPECT_EQ(0u, pipeControl->getPostSyncOperation());
        itorCmd++;
        itorCmd = find<PIPE_CONTROL *>(itorCmd, itorWalker2);
    }

    pipeControl = genCmdCast<PIPE_CONTROL *>(*itorCmd);
    ASSERT_NE(nullptr, pipeControl);

    // We should be writing a tag value to an address
    EXPECT_EQ(PIPE_CONTROL::POST_SYNC_OPERATION_WRITE_IMMEDIATE_DATA, pipeControl->getPostSyncOperation());
    uint64_t addressPC = ((uint64_t)pipeControl->getAddressHigh() << 32) | pipeControl->getAddress();

    // The PC address should match the CS tag address
    EXPECT_EQ((uint64_t)commandStreamReceiver.getTagAddress(), addressPC);
    EXPECT_EQ(1u, pipeControl->getImmediateData());
}
