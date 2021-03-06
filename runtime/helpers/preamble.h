/*
 * Copyright (c) 2018, Intel Corporation
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
#include "igfxfmid.h"
#include "stdint.h"
#include "runtime/command_stream/thread_arbitration_policy.h"
#include "runtime/helpers/pipeline_select_helper.h"
#include <cstddef>

namespace OCLRT {

struct HardwareInfo;
class Device;
class GraphicsAllocation;
class LinearStream;

template <typename GfxFamily>
struct PreambleHelper {
    static constexpr size_t getScratchSpaceOffsetFor64bit() { return 4096; }

    static void programL3(LinearStream *pCommandStream, uint32_t l3Config);
    static void programPipelineSelect(LinearStream *pCommandStream, bool mediaSamplerRequired);
    static void programThreadArbitration(LinearStream *pCommandStream, uint32_t requiredThreadArbitrationPolicy);
    static void programPreemption(LinearStream *pCommandStream, const Device &device, GraphicsAllocation *preemptionCsr);
    static void setupPipeControlInFrontOfCommand(void *pCmd, const HardwareInfo *hwInfo, bool isVfeCommand);
    static void programVFEState(LinearStream *pCommandStream, const HardwareInfo &hwInfo, int scratchSize, uint64_t scratchAddress);
    static void programPreamble(LinearStream *pCommandStream, const Device &device, uint32_t l3Config,
                                uint32_t requiredThreadArbitrationPolicy, GraphicsAllocation *preemptionCsr);
    static uint32_t getL3Config(const HardwareInfo &hwInfo, bool useSLM);
    static uint32_t getAdditionalCommandsSize(const Device &device);
    static void programGenSpecificPreambleWorkArounds(LinearStream *pCommandStream, const HardwareInfo &hwInfo);
    static uint32_t getUrbEntryAllocationSize();
};

template <PRODUCT_FAMILY ProductFamily>
static uint32_t getL3ConfigHelper(bool useSLM);

template <PRODUCT_FAMILY ProductFamily>
struct L3CNTLREGConfig {
    static const uint32_t valueForSLM;
    static const uint32_t valueForNoSLM;
};

template <PRODUCT_FAMILY ProductFamily>
uint32_t getL3ConfigHelper(bool useSLM) {
    if (!useSLM) {
        return L3CNTLREGConfig<ProductFamily>::valueForNoSLM;
    }
    return L3CNTLREGConfig<ProductFamily>::valueForSLM;
}

template <typename GfxFamily>
struct L3CNTLRegisterOffset {
    static const uint32_t registerOffset;
};
} // namespace OCLRT
