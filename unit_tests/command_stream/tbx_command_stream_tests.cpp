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

#include "runtime/mem_obj/mem_obj.h"
#include "tbx_command_stream_fixture.h"
#include "runtime/command_stream/tbx_command_stream_receiver_hw.h"
#include "runtime/command_stream/command_stream_receiver_hw.h"
#include "runtime/helpers/ptr_math.h"
#include "gen_cmd_parse.h"
#include "unit_tests/command_queue/command_queue_fixture.h"
#include "unit_tests/fixtures/device_fixture.h"
#include "test.h"
#include <cstdint>

using namespace OCLRT;
namespace Os {
extern const char *tbxLibName;
}

struct TbxFixture : public TbxCommandStreamFixture,
                    public DeviceFixture {

    using TbxCommandStreamFixture::SetUp;

    void SetUp() {
        DeviceFixture::SetUp();
        TbxCommandStreamFixture::SetUp(pDevice);
    }

    void TearDown() override {
        TbxCommandStreamFixture::TearDown();
        DeviceFixture::TearDown();
    }
};

typedef Test<TbxFixture> Tbx_command_stream;

TEST_F(Tbx_command_stream, DISABLED_testFactory) {
}

HWTEST_F(Tbx_command_stream, DISABLED_testTbxMemoryManager) {
    TbxCommandStreamReceiverHw<FamilyType> *tbxCsr = (TbxCommandStreamReceiverHw<FamilyType> *)pCommandStreamReceiver;
    TbxMemoryManager *getMM = tbxCsr->getMemoryManager();
    EXPECT_NE(nullptr, getMM);
    EXPECT_EQ(1 * GB, getMM->getSystemSharedMemory());
}

TEST_F(Tbx_command_stream, DISABLED_makeResident) {
    uint8_t buffer[0x10000];
    size_t size = sizeof(buffer);

    GraphicsAllocation *graphicsAllocation = mmTbx->allocateGraphicsMemory(size, buffer);
    pCommandStreamReceiver->makeResident(*graphicsAllocation);
    pCommandStreamReceiver->makeNonResident(*graphicsAllocation);
    mmTbx->freeGraphicsMemory(graphicsAllocation);
}

TEST_F(Tbx_command_stream, DISABLED_makeResidentOnZeroSizedBufferShouldDoNothing) {
    GraphicsAllocation graphicsAllocation(nullptr, 0);

    pCommandStreamReceiver->makeResident(graphicsAllocation);
    pCommandStreamReceiver->makeNonResident(graphicsAllocation);
}

TEST_F(Tbx_command_stream, DISABLED_flush) {
    char buffer[4096];
    memset(buffer, 0, 4096);
    LinearStream cs(buffer, 4096);
    size_t startOffset = 0;
    BatchBuffer batchBuffer{cs.getGraphicsAllocation(), startOffset, false, false, cs.getUsed(), &cs};
    pCommandStreamReceiver->flush(batchBuffer, EngineType::ENGINE_RCS, nullptr);
}

HWTEST_F(Tbx_command_stream, DISABLED_flushUntilTailRCSLargerThanSizeRCS) {
    char buffer[4096];
    memset(buffer, 0, 4096);
    LinearStream cs(buffer, 4096);
    size_t startOffset = 0;
    TbxCommandStreamReceiverHw<FamilyType> *tbxCsr = (TbxCommandStreamReceiverHw<FamilyType> *)pCommandStreamReceiver;
    auto &engineInfo = tbxCsr->engineInfoTable[EngineType::ENGINE_RCS];

    BatchBuffer batchBuffer{cs.getGraphicsAllocation(), startOffset, false, false, cs.getUsed(), &cs};
    pCommandStreamReceiver->flush(batchBuffer, EngineType::ENGINE_RCS, nullptr);
    auto size = engineInfo.sizeRCS;
    engineInfo.sizeRCS = 64;
    pCommandStreamReceiver->flush(batchBuffer, EngineType::ENGINE_RCS, nullptr);
    pCommandStreamReceiver->flush(batchBuffer, EngineType::ENGINE_RCS, nullptr);
    pCommandStreamReceiver->flush(batchBuffer, EngineType::ENGINE_RCS, nullptr);
    engineInfo.sizeRCS = size;
}

HWTEST_F(Tbx_command_stream, DISABLED_getCsTraits) {
    TbxCommandStreamReceiverHw<FamilyType> *tbxCsr = (TbxCommandStreamReceiverHw<FamilyType> *)pCommandStreamReceiver;
    tbxCsr->getCsTraits(EngineType::ENGINE_RCS);
    tbxCsr->getCsTraits(EngineType::ENGINE_BCS);
    tbxCsr->getCsTraits(EngineType::ENGINE_VCS);
    tbxCsr->getCsTraits(EngineType::ENGINE_VECS);
}

#if defined(__linux__)
TEST(TbxCommandStreamReceiverTest, DISABLED_createShouldReturnFunctionPointer) {
    TbxCommandStreamReceiver tbx;
    const HardwareInfo *hwInfo = platformDevices[0];
    CommandStreamReceiver *csr = tbx.create(*hwInfo);
    EXPECT_NE(nullptr, csr);
    delete csr;
}

namespace OCLRT {
TEST(TbxCommandStreamReceiverTest, createShouldReturnNullptrForEmptyEntryInFactory) {
    extern TbxCommandStreamReceiverCreateFunc tbxCommandStreamReceiverFactory[IGFX_MAX_PRODUCT];

    TbxCommandStreamReceiver tbx;
    const HardwareInfo *hwInfo = platformDevices[0];
    GFXCORE_FAMILY family = hwInfo->pPlatform->eRenderCoreFamily;
    auto pCreate = tbxCommandStreamReceiverFactory[family];

    tbxCommandStreamReceiverFactory[family] = nullptr;
    CommandStreamReceiver *csr = tbx.create(*hwInfo);
    EXPECT_EQ(nullptr, csr);

    tbxCommandStreamReceiverFactory[family] = pCreate;
}
} // namespace OCLRT
#endif

TEST(TbxCommandStreamReceiverTest, givenTbxCommandStreamReceiverWhenItIsCreatedWithWrongGfxCoreFamilyThenNullPointerShouldBeReturned) {
    HardwareInfo hwInfo = *platformDevices[0];
    GFXCORE_FAMILY family = hwInfo.pPlatform->eRenderCoreFamily;

    const_cast<PLATFORM *>(hwInfo.pPlatform)->eRenderCoreFamily = GFXCORE_FAMILY_FORCE_ULONG; // wrong gfx core family

    CommandStreamReceiver *csr = TbxCommandStreamReceiver::create(hwInfo);
    EXPECT_EQ(nullptr, csr);

    const_cast<PLATFORM *>(hwInfo.pPlatform)->eRenderCoreFamily = family;
}
