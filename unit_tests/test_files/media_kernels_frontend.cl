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

// VME KERNELS
__kernel __attribute__((reqd_work_group_size(16, 1, 1))) void
block_motion_estimate_intel(sampler_t accelerator, __read_only image2d_t srcImg,
                            __read_only image2d_t refImg,
                            __global short2 *prediction_motion_vector_buffer,
                            __global short2 *motion_vector_buffer,
                            __global ushort *residuals) {
}

__kernel __attribute__((reqd_work_group_size(16, 1, 1))) void
block_advanced_motion_estimate_check_intel(
    sampler_t accelerator, __read_only image2d_t srcImg,
    __read_only image2d_t refImg, uint flags, uint skip_block_type,
    uint search_cost_penalty, uint search_cost_precision,
    __global short2 *count_motion_vector_buffer,
    __global short2 *predictors_buffer,
    __global short2 *skip_motion_vector_buffer,
    __global short2 *motion_vector_buffer,
    __global char *intra_search_predictor_modes, __global ushort *residuals,
    __global ushort *skip_residuals, __global ushort *intra_residuals) {
}

__kernel __attribute__((reqd_work_group_size(16, 1, 1))) void
block_advanced_motion_estimate_bidirectional_check_intel(
    sampler_t accelerator, __read_only image2d_t srcImg,
    __read_only image2d_t refImg, __read_only image2d_t src_check_image,
    __read_only image2d_t ref0_check_image,
    __read_only image2d_t ref1_check_image, uint flags,
    uint search_cost_penalty, uint search_cost_precision, short2 count_global,
    uchar bidir_weight, __global short2 *count_motion_vector_buffer,
    __global short2 *prediction_motion_vector_buffer,
    __global char *skip_input_mode_buffer,
    __global short2 *skip_motion_vector_buffer,
    __global short2 *search_motion_vector_buffer,
    __global char *intra_search_predictor_modes,
    __global ushort *search_residuals, __global ushort *skip_residuals,
    __global ushort *intra_residuals) {
}

// VEBOX KERNELS:
__kernel void ve_enhance_intel(sampler_t accelerator,
                               int flags,
                               __read_only image2d_t current_input,
                               __write_only image2d_t current_output) {
}

__kernel void ve_dn_enhance_intel(sampler_t accelerator,
                                  int flags,
                                  __read_only image2d_t ref_input,
                                  __read_only image2d_t current_input,
                                  __write_only image2d_t current_output) {
}

__kernel void ve_dn_di_enhance_intel(sampler_t accelerator,
                                     int flags,
                                     __read_only image2d_t current_input,
                                     __read_only image2d_t ref_input,
                                     __write_only image2d_t current_output,
                                     __write_only image2d_t ref_output,
                                     __write_only image2d_t dndi_output) {
}
