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
#include "runtime/api/cl_types.h"
#include "runtime/helpers/base_object.h"
#include "runtime/helpers/hw_info.h"

namespace OCLRT {
class Context;

template <>
struct OpenCLObjectMapper<_cl_sampler> {
    typedef class Sampler DerivedType;
};

class Sampler : public BaseObject<_cl_sampler> {
  public:
    static const cl_ulong objectMagic = 0x4684913AC213EF00LL;
    static const uint32_t samplerStateArrayAlignment = 64;

    static Sampler *create(Context *context, cl_bool normalizedCoordinates,
                           cl_addressing_mode addressingMode, cl_filter_mode filterMode,
                           cl_int &errcodeRet);

    static Sampler *create(Context *context,
                           const cl_sampler_properties *samplerProperties,
                           cl_int &errcodeRet);

    cl_int getInfo(cl_sampler_info paramName, size_t paramValueSize,
                   void *paramValue, size_t *paramValueSizeRet);

    virtual void setArg(void *memory) = 0;

    static size_t getSamplerStateSize(const HardwareInfo &hwInfo);

    Sampler(Context *context,
            cl_bool normalizedCoordinates,
            cl_addressing_mode addressingMode,
            cl_filter_mode filterMode);

    unsigned int getSnapWaValue() const;

    // clang-format off
    cl_context         context;
    cl_bool            normalizedCoordinates;
    cl_addressing_mode addressingMode;
    cl_filter_mode     filterMode;
    // clang-format off
};

template <typename GfxFamily>
struct SamplerHw : public Sampler {
    void setArg(void *memory) override;

    SamplerHw(Context *context, 
              cl_bool normalizedCoordinates,
              cl_addressing_mode addressingMode, 
              cl_filter_mode filterMode) : Sampler(context, 
                                                   normalizedCoordinates, 
                                                   addressingMode, 
                                                   filterMode) {
    }

    static Sampler *create(Context *context, 
                           cl_bool normalizedCoordinates,
                           cl_addressing_mode addressingMode, 
                           cl_filter_mode filterMode) {
        return new SamplerHw<GfxFamily>(context,
                                         normalizedCoordinates, 
                                         addressingMode, 
                                         filterMode);
    }

    static size_t getSamplerStateSize();
};

typedef Sampler *(*SamplerCreateFunc)(Context *context, 
                           cl_bool normalizedCoordinates,
                           cl_addressing_mode addressingMode, 
                           cl_filter_mode filterMode);

typedef size_t(*getSamplerStateSizeHwFunc)();
}
