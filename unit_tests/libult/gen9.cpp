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

#include "hw_info.h"
#include "hw_cmds.h"
#include "runtime/command_stream/command_stream_receiver_hw.inl"
#include "runtime/memory_manager/memory_manager.h"
#include "runtime/command_stream/device_command_stream.h"
#include "unit_tests/libult/ult_command_stream_receiver.h"

namespace OCLRT {

typedef SKLFamily Family;

static const auto gfxCore = IGFX_GEN9_CORE;

extern CommandStreamReceiverCreateFunc commandStreamReceiverFactory[2 * IGFX_MAX_CORE];

template <>
void populateFactoryTable<UltCommandStreamReceiver<Family>>() {
    commandStreamReceiverFactory[IGFX_MAX_CORE + gfxCore] = UltCommandStreamReceiver<Family>::create;
}

struct enableGen9 {
    enableGen9() {
        populateFactoryTable<UltCommandStreamReceiver<Family>>();
    }
};

static enableGen9 enable;

template class UltCommandStreamReceiver<SKLFamily>;
} // namespace OCLRT
