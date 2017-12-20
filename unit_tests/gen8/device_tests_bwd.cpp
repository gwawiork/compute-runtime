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

#include "unit_tests/fixtures/device_fixture.h"
#include "unit_tests/fixtures/memory_management_fixture.h"
#include "test.h"

using namespace OCLRT;
struct DeviceTest : public DeviceFixture,
                    public MemoryManagementFixture,
                    public ::testing::Test {
    void SetUp() override {
        MemoryManagementFixture::SetUp();
        DeviceFixture::SetUp();
    }

    void TearDown() override {
        DeviceFixture::TearDown();
        MemoryManagementFixture::TearDown();
    }
};

BDWTEST_F(DeviceTest, getSupportedClVersion20Device) {
    auto version = pDevice->getSupportedClVersion();
    EXPECT_EQ(20u, version);
}
