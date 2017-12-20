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
#include "runtime/gen_common/aub_mapper.h"
#include "runtime/command_stream/command_stream_receiver_hw.h"
#include "runtime/command_stream/aub_command_stream_receiver.h"
#include "runtime/memory_manager/address_mapper.h"
#include "runtime/memory_manager/page_table.h"
#include "runtime/memory_manager/os_agnostic_memory_manager.h"

namespace OCLRT {
template <typename GfxFamily>
class AUBCommandStreamReceiverHw : public CommandStreamReceiverHw<GfxFamily> {
    typedef CommandStreamReceiverHw<GfxFamily> BaseClass;
    typedef typename AUBFamilyMapper<GfxFamily>::AUB AUB;
    typedef typename AUB::MiContextDescriptorReg MiContextDescriptorReg;

  public:
    FlushStamp flush(BatchBuffer &batchBuffer, EngineType engineOrdinal, ResidencyContainer *allocationsForResidency) override;
    void makeResident(GraphicsAllocation &gfxAllocation) override;
    void makeNonResident(GraphicsAllocation &gfxAllocation) override;

    // Family specific version
    void submitLRCA(EngineType engineOrdinal, const MiContextDescriptorReg &contextDescriptor);
    void pollForCompletion(EngineType engineOrdinal);
    void initGlobalMMIO();
    void initEngineMMIO(EngineType engineOrdinal);

    void addContextToken();

    static CommandStreamReceiver *create(const HardwareInfo &hwInfoIn, const std::string &fileName);

    AUBCommandStreamReceiverHw(const HardwareInfo &hwInfoIn);
    ~AUBCommandStreamReceiverHw() override;

    void initializeEngine(EngineType engineOrdinal);

    MemoryManager *createMemoryManager(bool enable64kbPages) override {
        this->memoryManager = new OsAgnosticMemoryManager(enable64kbPages);
        return this->memoryManager;
    }

    static const AubMemDump::LrcaHelper &getCsTraits(EngineType engineOrdinal);

    struct EngineInfo {
        void *pLRCA;
        uint32_t ggttLRCA;
        void *pGlobalHWStatusPage;
        uint32_t ggttHWSP;
        void *pRingBuffer;
        uint32_t ggttRingBuffer;
        size_t sizeRingBuffer;
        uint32_t tailRingBuffer;
    } engineInfoTable[EngineType::NUM_ENGINES];

    AUBCommandStreamReceiver::AubFileStream stream;

    TypeSelector<PML4, PDPE, sizeof(void *) == 8>::type ppgtt;
    PDPE ggtt;
    // remap CPU VA -> GGTT VA
    AddressMapper gttRemap;
};
} // namespace OCLRT
