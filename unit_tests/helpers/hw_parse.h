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

#pragma once
#include "runtime/command_queue/command_queue.h"
#include "runtime/command_stream/command_stream_receiver.h"
#include "runtime/command_stream/linear_stream.h"
#include "runtime/helpers/ptr_math.h"
#include "runtime/helpers/pipeline_select_helper.h"
#include "runtime/kernel/kernel.h"
#include "unit_tests/gen_common/gen_cmd_parse.h"
#include "gtest/gtest.h"

namespace OCLRT {

struct HardwareParse {
    HardwareParse() : previousCS(nullptr),
                      startCS(0),
                      startCSRCS(0),
                      sizeUsed(0),
                      cmdInterfaceDescriptorData(nullptr),
                      cmdMediaInterfaceDescriptorLoad(nullptr),
                      cmdMediaVfeState(nullptr),
                      cmdPipelineSelect(nullptr),
                      cmdStateBaseAddress(nullptr),
                      cmdWalker(nullptr),
                      cmdGpgpuCsrBaseAddress(nullptr) {
        itorMediaInterfaceDescriptorLoad = cmdList.end();
        itorMediaVfeState = cmdList.end();
        itorPipelineSelect = cmdList.end();
        itorStateBaseAddress = cmdList.end();
        itorWalker = cmdList.end();
        itorGpgpuCsrBaseAddress = cmdList.end();
    }

    void SetUp() {
    }

    void TearDown() {
        cmdList.clear();
        lriList.clear();
    }

    template <typename CmdType>
    GenCmdList getCommandsList() {
        GenCmdList list;
        for (auto it = cmdList.begin(); it != cmdList.end(); it++) {
            auto cmd = genCmdCast<CmdType *>(*it);
            if (cmd) {
                list.push_back(*it);
            }
        }
        return list;
    }

    template <typename FamilyType>
    void findCsrBaseAddress();

    template <typename FamilyType>
    void findHardwareCommands() {
        typedef typename FamilyType::GPGPU_WALKER GPGPU_WALKER;
        typedef typename FamilyType::PIPELINE_SELECT PIPELINE_SELECT;
        typedef typename FamilyType::STATE_BASE_ADDRESS STATE_BASE_ADDRESS;
        typedef typename FamilyType::MEDIA_INTERFACE_DESCRIPTOR_LOAD MEDIA_INTERFACE_DESCRIPTOR_LOAD;
        typedef typename FamilyType::MEDIA_VFE_STATE MEDIA_VFE_STATE;
        typedef typename FamilyType::INTERFACE_DESCRIPTOR_DATA INTERFACE_DESCRIPTOR_DATA;
        typedef typename FamilyType::MI_BATCH_BUFFER_START MI_BATCH_BUFFER_START;
        typedef typename FamilyType::MI_LOAD_REGISTER_IMM MI_LOAD_REGISTER_IMM;

        itorWalker = find<GPGPU_WALKER *>(cmdList.begin(), cmdList.end());
        if (itorWalker != cmdList.end()) {
            cmdWalker = *itorWalker;
        }

        itorBBStartAfterWalker = find<MI_BATCH_BUFFER_START *>(itorWalker, cmdList.end());
        if (itorBBStartAfterWalker != cmdList.end()) {
            cmdBBStartAfterWalker = *itorBBStartAfterWalker;
        }
        for (auto it = cmdList.begin(); it != cmdList.end(); it++) {
            auto lri = genCmdCast<MI_LOAD_REGISTER_IMM *>(*it);
            if (lri) {
                lriList.push_back(*it);
            }
        }

        MEDIA_INTERFACE_DESCRIPTOR_LOAD *cmdMIDL = nullptr;
        itorMediaInterfaceDescriptorLoad = find<MEDIA_INTERFACE_DESCRIPTOR_LOAD *>(cmdList.begin(), itorWalker);
        if (itorMediaInterfaceDescriptorLoad != itorWalker) {
            cmdMIDL = (MEDIA_INTERFACE_DESCRIPTOR_LOAD *)*itorMediaInterfaceDescriptorLoad;
            cmdMediaInterfaceDescriptorLoad = *itorMediaInterfaceDescriptorLoad;
        }

        itorPipelineSelect = find<PIPELINE_SELECT *>(cmdList.begin(), itorWalker);
        if (itorPipelineSelect != itorWalker) {
            cmdPipelineSelect = *itorPipelineSelect;
        }

        itorMediaVfeState = find<MEDIA_VFE_STATE *>(itorPipelineSelect, itorWalker);
        if (itorMediaVfeState != itorWalker) {
            cmdMediaVfeState = *itorMediaVfeState;
        }

        STATE_BASE_ADDRESS *cmdSBA = nullptr;
        uint64_t dynamicStateHeap = 0;
        itorStateBaseAddress = find<STATE_BASE_ADDRESS *>(cmdList.begin(), itorWalker);
        if (itorStateBaseAddress != itorWalker) {
            cmdSBA = (STATE_BASE_ADDRESS *)*itorStateBaseAddress;
            cmdStateBaseAddress = *itorStateBaseAddress;

            // Extract the dynamicStateHeap
            dynamicStateHeap = cmdSBA->getDynamicStateBaseAddress();
            ASSERT_NE(0u, dynamicStateHeap);
        }

        // interfaceDescriptorData should be located within dynamicStateHeap
        if (cmdMIDL && cmdSBA) {
            auto iddStart = cmdMIDL->getInterfaceDescriptorDataStartAddress();
            auto iddEnd = iddStart + cmdMIDL->getInterfaceDescriptorTotalLength();
            ASSERT_LE(iddEnd, cmdSBA->getDynamicStateBufferSize() * MemoryConstants::pageSize);

            // Extract the interfaceDescriptorData
            cmdInterfaceDescriptorData = (INTERFACE_DESCRIPTOR_DATA *)(dynamicStateHeap + iddStart);
        }
    }

    template <typename FamilyType>
    void parseCommands(OCLRT::LinearStream &commandStream, size_t startOffset = 0) {
        ASSERT_LE(startOffset, commandStream.getUsed());
        auto sizeToParse = commandStream.getUsed() - startOffset;
        ASSERT_TRUE(FamilyType::PARSE::parseCommandBuffer(
            cmdList,
            ptrOffset(commandStream.getBase(), startOffset),
            sizeToParse));
    }

    template <typename FamilyType>
    void parseCommands(OCLRT::CommandQueue &commandQueue) {
        auto &device = commandQueue.getDevice();
        auto &commandStreamReceiver = device.getCommandStreamReceiver();
        auto &commandStreamCSR = commandStreamReceiver.getCS();

        parseCommands<FamilyType>(commandStreamCSR, startCSRCS);
        startCSRCS = commandStreamCSR.getUsed();

        auto &commandStream = commandQueue.getCS();
        if (previousCS != &commandStream) {
            startCS = 0;
        }
        parseCommands<FamilyType>(commandStream, startCS);
        startCS = commandStream.getUsed();
        previousCS = &commandStream;

        sizeUsed = commandStream.getUsed();
        findHardwareCommands<FamilyType>();
    }

    template <typename FamilyType>
    const typename FamilyType::RENDER_SURFACE_STATE &getSurfaceState(uint32_t index) {
        typedef typename FamilyType::BINDING_TABLE_STATE BINDING_TABLE_STATE;
        typedef typename FamilyType::INTERFACE_DESCRIPTOR_DATA INTERFACE_DESCRIPTOR_DATA;
        typedef typename FamilyType::RENDER_SURFACE_STATE RENDER_SURFACE_STATE;
        typedef typename FamilyType::STATE_BASE_ADDRESS STATE_BASE_ADDRESS;

        const auto &interfaceDescriptorData = *(INTERFACE_DESCRIPTOR_DATA *)cmdInterfaceDescriptorData;

        auto cmdSBA = (STATE_BASE_ADDRESS *)cmdStateBaseAddress;
        auto surfaceStateHeap = cmdSBA->getSurfaceStateBaseAddress();
        EXPECT_NE(0u, surfaceStateHeap);

        auto bindingTablePointer = interfaceDescriptorData.getBindingTablePointer();

        const auto &bindingTableState = reinterpret_cast<BINDING_TABLE_STATE *>(surfaceStateHeap + bindingTablePointer)[index];
        auto surfaceStatePointer = bindingTableState.getSurfaceStatePointer();

        return *(RENDER_SURFACE_STATE *)(surfaceStateHeap + surfaceStatePointer);
    }

    template <typename FamilyType>
    const typename FamilyType::SAMPLER_STATE &getSamplerState(uint32_t index) {
        typedef typename FamilyType::INTERFACE_DESCRIPTOR_DATA INTERFACE_DESCRIPTOR_DATA;
        typedef typename FamilyType::SAMPLER_STATE SAMPLER_STATE;
        typedef typename FamilyType::STATE_BASE_ADDRESS STATE_BASE_ADDRESS;

        const auto &interfaceDescriptorData = *(INTERFACE_DESCRIPTOR_DATA *)cmdInterfaceDescriptorData;

        auto cmdSBA = (STATE_BASE_ADDRESS *)cmdStateBaseAddress;
        auto dynamicStateHeap = cmdSBA->getDynamicStateBaseAddress();
        EXPECT_NE(0, dynamicStateHeap);

        const auto samplerState = reinterpret_cast<SAMPLER_STATE *>(dynamicStateHeap + interfaceDescriptorData.getSamplerStatePointer());
        return samplerState[index];
    }

    template <typename FamilyType>
    const void *getStatelessArgumentPointer(const Kernel &kernel, uint32_t indexArg) {
        typedef typename FamilyType::INTERFACE_DESCRIPTOR_DATA INTERFACE_DESCRIPTOR_DATA;
        typedef typename FamilyType::STATE_BASE_ADDRESS STATE_BASE_ADDRESS;
        typedef typename FamilyType::GPGPU_WALKER GPGPU_WALKER;
        auto cmdIDD = (INTERFACE_DESCRIPTOR_DATA *)cmdInterfaceDescriptorData;
        EXPECT_NE(nullptr, cmdIDD);
        auto cmdSBA = (STATE_BASE_ADDRESS *)cmdStateBaseAddress;
        EXPECT_NE(nullptr, cmdSBA);

        auto cmdWalker = (GPGPU_WALKER *)this->cmdWalker;
        EXPECT_NE(nullptr, cmdWalker);

        auto offsetCrossThreadData = cmdWalker->getIndirectDataStartAddress();
        EXPECT_LT(offsetCrossThreadData, cmdSBA->getIndirectObjectBufferSize() * MemoryConstants::pageSize);

        // Get the base of cross thread
        auto pCrossThreadData = ptrOffset(
            reinterpret_cast<const void *>(cmdSBA->getIndirectObjectBaseAddress()),
            offsetCrossThreadData);

        // Determine where the argument is
        auto &patchInfo = kernel.getKernelInfo().patchInfo;
        for (auto &arg : patchInfo.statelessGlobalMemObjKernelArgs) {
            if (arg->ArgumentNumber == indexArg) {
                return ptrOffset(pCrossThreadData, arg->DataParamOffset);
            }
        }
        return nullptr;
    }

    template <typename CmdType>
    CmdType *getCommand(GenCmdList::iterator itorStart, GenCmdList::iterator itorEnd) {
        auto itorCmd = find<CmdType *>(itorStart, itorEnd);
        return itorCmd != cmdList.end()
                   ? genCmdCast<CmdType *>(*itorCmd)
                   : nullptr;
    }

    template <typename CmdType>
    CmdType *getCommand() {
        return getCommand<CmdType>(cmdList.begin(), cmdList.end());
    }

    template <typename FamilyType>
    int getNumberOfPipelineSelectsThatEnablePipelineSelect() {
        typedef typename FamilyType::PIPELINE_SELECT PIPELINE_SELECT;
        int numCommands = 0;
        auto itorCmd = find<PIPELINE_SELECT *>(cmdList.begin(), cmdList.end());
        while (itorCmd != cmdList.end()) {
            auto cmd = getCommand<PIPELINE_SELECT>(itorCmd, cmdList.end());
            if (cmd->getPipelineSelection() == PIPELINE_SELECT::PIPELINE_SELECTION_GPGPU &&
                pipelineSelectEnablePipelineSelectMaskBits == (pipelineSelectEnablePipelineSelectMaskBits & cmd->getMaskBits())) {
                numCommands++;
            }
            itorCmd = find<PIPELINE_SELECT *>(++itorCmd, cmdList.end());
        }
        return numCommands;
    }

    // The starting point of parsing commandBuffers.  This is important
    // because as buffers get reused, we only want to parse the deltas.
    LinearStream *previousCS;
    size_t startCS;
    size_t startCSRCS;

    size_t sizeUsed;
    GenCmdList cmdList;
    GenCmdList lriList;
    GenCmdList::iterator itorMediaInterfaceDescriptorLoad;
    GenCmdList::iterator itorMediaVfeState;
    GenCmdList::iterator itorPipelineSelect;
    GenCmdList::iterator itorStateBaseAddress;
    GenCmdList::iterator itorWalker;
    GenCmdList::iterator itorBBStartAfterWalker;
    GenCmdList::iterator itorGpgpuCsrBaseAddress;

    void *cmdInterfaceDescriptorData;
    void *cmdMediaInterfaceDescriptorLoad;
    void *cmdMediaVfeState;
    void *cmdPipelineSelect;
    void *cmdStateBaseAddress;
    void *cmdWalker;
    void *cmdBBStartAfterWalker;
    void *cmdGpgpuCsrBaseAddress;
};
} // namespace OCLRT
