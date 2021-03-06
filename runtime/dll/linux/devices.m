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

// clang-format off
#ifdef SUPPORT_GEN9
#ifdef SUPPORT_SKL
DEVICE( ISKL_GT1_DESK_DEVICE_F0_ID,         SKL_1x2x6,  GTTYPE_GT1 )
DEVICE( ISKL_GT1_DT_DEVICE_F0_ID,           SKL_1x2x6,  GTTYPE_GT1 )
DEVICE( ISKL_GT1_SERV_DEVICE_F0_ID,         SKL_1x2x6,  GTTYPE_GT1 )
DEVICE( ISKL_GT1_5_DT_DEVICE_F0_ID,         SKL_1x3x6,  GTTYPE_GT1_5 )
DEVICE( ISKL_GT2_DESK_DEVICE_F0_ID,         SKL_1x3x8,  GTTYPE_GT2 )
DEVICE( ISKL_GT2_DT_DEVICE_F0_ID,           SKL_1x3x8,  GTTYPE_GT2 )
DEVICE( ISKL_GT2_WRK_DEVICE_F0_ID,          SKL_1x3x8,  GTTYPE_GT2 )
DEVICE( ISKL_GT2_SERV_DEVICE_F0_ID,         SKL_1x3x8,  GTTYPE_GT2 )
DEVICE( ISKL_GT3_DESK_DEVICE_F0_ID,         SKL_2x3x8,  GTTYPE_GT3 )
DEVICE( ISKL_GT3_SERV_DEVICE_F0_ID,         SKL_2x3x8,  GTTYPE_GT3 )
DEVICE( ISKL_GT3_MEDIA_SERV_DEVICE_F0_ID,   SKL_2x3x8,  GTTYPE_GT3 )
DEVICE( ISKL_GT4_DESK_DEVICE_F0_ID,         SKL_3x3x8,  GTTYPE_GT4 )
DEVICE( ISKL_GT4_DT_DEVICE_F0_ID,           SKL_3x3x8,  GTTYPE_GT4 )
DEVICE( ISKL_GT4_WRK_DEVICE_F0_ID,          SKL_3x3x8,  GTTYPE_GT4 )
DEVICE( ISKL_GT4_SERV_DEVICE_F0_ID,         SKL_3x3x8,  GTTYPE_GT4 )
DEVICE( ISKL_GT2_ULT_DEVICE_F0_ID,          SKL_1x3x8,  GTTYPE_GT2 )
DEVICE( ISKL_GT2F_ULT_DEVICE_F0_ID,         SKL_1x3x8,  GTTYPE_GT2 )
DEVICE( ISKL_GT2_ULX_DEVICE_F0_ID,          SKL_1x3x8,  GTTYPE_GT2 )
DEVICE( ISKL_GT1_ULT_DEVICE_F0_ID,          SKL_1x2x6,  GTTYPE_GT1 )
DEVICE( ISKL_GT1_ULX_DEVICE_F0_ID,          SKL_1x2x6,  GTTYPE_GT1 )
DEVICE( ISKL_GT1_5_ULT_DEVICE_F0_ID,        SKL_1x3x6,  GTTYPE_GT1_5 )
DEVICE( ISKL_GT1_5_ULX_DEVICE_F0_ID,        SKL_1x3x6,  GTTYPE_GT1_5 )
DEVICE( ISKL_GT3_ULT_DEVICE_F0_ID,          SKL_2x3x8,  GTTYPE_GT3 )
DEVICE( ISKL_GT3e_ULT_DEVICE_F0_ID_540,     SKL_2x3x8,  GTTYPE_GT3 )
DEVICE( ISKL_GT3e_ULT_DEVICE_F0_ID_550,     SKL_2x3x8,  GTTYPE_GT3 )
DEVICE( ISKL_GT1_HALO_MOBL_DEVICE_F0_ID,    SKL_1x2x6,  GTTYPE_GT1 )
DEVICE( ISKL_GT2_HALO_MOBL_DEVICE_F0_ID,    SKL_1x3x8,  GTTYPE_GT2 )
DEVICE( ISKL_GT3_HALO_MOBL_DEVICE_F0_ID,    SKL_2x3x8,  GTTYPE_GT3 )
DEVICE( ISKL_GT4_HALO_MOBL_DEVICE_F0_ID,    SKL_3x3x8,  GTTYPE_GT4 )
DEVICE( ISKL_LP_DEVICE_F0_ID,               SKL_1x3x8,  GTTYPE_GT2 )
#endif
#ifdef SUPPORT_KBL
DEVICE( IKBL_GT1_ULT_DEVICE_F0_ID,           KBL_1x2x6,  GTTYPE_GT1 )
DEVICE( IKBL_GT1_5_ULT_DEVICE_F0_ID,         KBL_1x3x6,  GTTYPE_GT1_5 )
DEVICE( IKBL_GT2_ULT_DEVICE_F0_ID,           KBL_1x3x8,  GTTYPE_GT2 )
DEVICE( IKBL_GT2F_ULT_DEVICE_F0_ID,          KBL_1x3x8,  GTTYPE_GT2 )
DEVICE( IKBL_GT3_15W_ULT_DEVICE_F0_ID,       KBL_2x3x8,  GTTYPE_GT3 )
DEVICE( IKBL_GT1_ULX_DEVICE_F0_ID,           KBL_1x3x6,  GTTYPE_GT1 )
DEVICE( IKBL_GT1_5_ULX_DEVICE_F0_ID,         KBL_1x2x6,  GTTYPE_GT1_5 )
DEVICE( IKBL_GT2_ULX_DEVICE_F0_ID,           KBL_1x3x8,  GTTYPE_GT2 )
DEVICE( IKBL_GT1_DT_DEVICE_F0_ID,            KBL_1x2x6,  GTTYPE_GT1 )
DEVICE( IKBL_GT2_R_ULT_DEVICE_F0_ID,         KBL_1x3x8,  GTTYPE_GT2 )
DEVICE( IKBL_GT2_DT_DEVICE_F0_ID,            KBL_1x3x8,  GTTYPE_GT2 )
DEVICE( IKBL_GT1_HALO_DEVICE_F0_ID,          KBL_1x2x6,  GTTYPE_GT1 )
DEVICE( IKBL_GT1F_HALO_DEVICE_F0_ID,         KBL_1x2x6,  GTTYPE_GT1 )
DEVICE( IKBL_GT2_HALO_DEVICE_F0_ID,          KBL_1x3x8,  GTTYPE_GT2 )
DEVICE( IKBL_GT4_HALO_DEVICE_F0_ID,          KBL_3x3x8,  GTTYPE_GT4 )
DEVICE( IKBL_GT1_SERV_DEVICE_F0_ID,          KBL_1x2x6,  GTTYPE_GT1 )
DEVICE( IKBL_GT2_SERV_DEVICE_F0_ID,          KBL_1x3x8,  GTTYPE_GT2 )
DEVICE( IKBL_GT2_WRK_DEVICE_F0_ID,           KBL_1x3x8,  GTTYPE_GT2 )
DEVICE( IKBL_GT3_ULT_DEVICE_F0_ID,           KBL_2x3x8,  GTTYPE_GT3 )
DEVICE( IKBL_GT3_28W_ULT_DEVICE_F0_ID,       KBL_2x3x8,  GTTYPE_GT3 )
DEVICE( IKBL_GT4_DT_DEVICE_F0_ID,            KBL_3x3x8,  GTTYPE_GT4 )
DEVICE( IKBL_GT3_HALO_DEVICE_F0_ID,          KBL_2x3x8,  GTTYPE_GT3 )
DEVICE( IKBL_GT3_SERV_DEVICE_F0_ID,          KBL_2x3x8,  GTTYPE_GT3 )
DEVICE( IKBL_GT4_SERV_DEVICE_F0_ID,          KBL_3x3x8,  GTTYPE_GT4 )
DEVICE( IKBL_GT4_WRK_DEVICE_F0_ID,           KBL_3x3x8,  GTTYPE_GT4 )
#endif
#ifdef SUPPORT_BXT
DEVICE(IBXT_P_3x6_DEVICE_ID,                BXT_1x3x6,  GTTYPE_GTA)   //18EU APL
DEVICE(IBXT_P_12EU_3x6_DEVICE_ID,           BXT_1x2x6,  GTTYPE_GTA)   //12EU APL
#endif
#endif
#ifdef SUPPORT_GEN8
DEVICE( IBDW_GT1_HALO_MOBL_DEVICE_F0_ID,    BDW_1x2x6,  GTTYPE_GT1 )
DEVICE( IBDW_GT1_ULT_MOBL_DEVICE_F0_ID,     BDW_1x2x6,  GTTYPE_GT1 )
DEVICE( IBDW_GT2_HALO_MOBL_DEVICE_F0_ID,    BDW_1x3x8,  GTTYPE_GT2 )
DEVICE( IBDW_GT2_ULT_MOBL_DEVICE_F0_ID,     BDW_1x3x8,  GTTYPE_GT2 )
DEVICE( IBDW_GT2_ULX_DEVICE_F0_ID,          BDW_1x3x8,  GTTYPE_GT2 )
DEVICE( IBDW_GT3_HALO_MOBL_DEVICE_F0_ID,    BDW_2x3x8,  GTTYPE_GT3 )
DEVICE( IBDW_GT3_ULT_MOBL_DEVICE_F0_ID,     BDW_2x3x8,  GTTYPE_GT3 )
DEVICE( IBDW_GT3_SERV_DEVICE_F0_ID,         BDW_2x3x8,  GTTYPE_GT3 )
DEVICE( IBDW_GT3_ULT25W_MOBL_DEVICE_F0_ID,  BDW_2x3x8,  GTTYPE_GT3 )
#endif
// clang-format on
