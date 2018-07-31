/*
 *  Copyright (c) 2011 The  project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


// This header file includes the inline functions in
// the fix point signal processing library.

#ifndef SPL_SPL_INL_H_
#define SPL_SPL_INL_H_

#ifdef ARCH_ARM_V7A
#include "spl_inl_armv7.h"
#else

static __inline Word16 Spl_SatW32ToW16(Word32 value32) {
  Word16 out16 = (Word16) value32;

  if (value32 > 32767)
    out16 = 32767;
  else if (value32 < -32768)
    out16 = -32768;

  return out16;
}

static __inline Word16 Spl_AddSatW16(Word16 a,
                                                  Word16 b) {
  return Spl_SatW32ToW16((Word32) a + (Word32) b);
}

static __inline Word32 Spl_AddSatW32(Word32 l_var1,
                                                  Word32 l_var2) {
  Word32 l_sum;

  // perform long addition
  l_sum = l_var1 + l_var2;

  // check for under or overflow
  if (SPL_IS_NEG(l_var1)) {
    if (SPL_IS_NEG(l_var2) && !SPL_IS_NEG(l_sum)) {
        l_sum = (Word32)0x80000000;
    }
  } else {
    if (!SPL_IS_NEG(l_var2) && SPL_IS_NEG(l_sum)) {
        l_sum = (Word32)0x7FFFFFFF;
    }
  }

  return l_sum;
}

static __inline Word16 Spl_SubSatW16(Word16 var1,
                                                  Word16 var2) {
  return Spl_SatW32ToW16((Word32) var1 - (Word32) var2);
}

static __inline Word32 Spl_SubSatW32(Word32 l_var1,
                                                  Word32 l_var2) {
  Word32 l_diff;

  // perform subtraction
  l_diff = l_var1 - l_var2;

  // check for underflow
  if ((l_var1 < 0) && (l_var2 > 0) && (l_diff > 0))
    l_diff = (Word32)0x80000000;
  // check for overflow
  if ((l_var1 > 0) && (l_var2 < 0) && (l_diff < 0))
    l_diff = (Word32)0x7FFFFFFF;

  return l_diff;
}

static __inline Word16 Spl_GetSizeInBits(UWord32 n) {
  int bits;

  if (0xFFFF0000 & n) {
    bits = 16;
  } else {
    bits = 0;
  }
  if (0x0000FF00 & (n >> bits)) bits += 8;
  if (0x000000F0 & (n >> bits)) bits += 4;
  if (0x0000000C & (n >> bits)) bits += 2;
  if (0x00000002 & (n >> bits)) bits += 1;
  if (0x00000001 & (n >> bits)) bits += 1;

  return bits;
}

static __inline int Spl_NormW32(Word32 a) {
  int zeros;

  if (a <= 0) a ^= 0xFFFFFFFF;

  if (!(0xFFFF8000 & a)) {
    zeros = 16;
  } else {
    zeros = 0;
  }
  if (!(0xFF800000 & (a << zeros))) zeros += 8;
  if (!(0xF8000000 & (a << zeros))) zeros += 4;
  if (!(0xE0000000 & (a << zeros))) zeros += 2;
  if (!(0xC0000000 & (a << zeros))) zeros += 1;

  return zeros;
}

static __inline int Spl_NormU32(UWord32 a) {
  int zeros;

  if (a == 0) return 0;

  if (!(0xFFFF0000 & a)) {
    zeros = 16;
  } else {
    zeros = 0;
  }
  if (!(0xFF000000 & (a << zeros))) zeros += 8;
  if (!(0xF0000000 & (a << zeros))) zeros += 4;
  if (!(0xC0000000 & (a << zeros))) zeros += 2;
  if (!(0x80000000 & (a << zeros))) zeros += 1;

  return zeros;
}

static __inline int Spl_NormW16(Word16 a) {
  int zeros;

  if (a <= 0) a ^= 0xFFFF;

  if (!(0xFF80 & a)) {
    zeros = 8;
  } else {
    zeros = 0;
  }
  if (!(0xF800 & (a << zeros))) zeros += 4;
  if (!(0xE000 & (a << zeros))) zeros += 2;
  if (!(0xC000 & (a << zeros))) zeros += 1;

  return zeros;
}

static __inline Word32 MulAccumW16(Word16 a,
                                          Word16 b,
                                          Word32 c) {
  return (a * b + c);
}

#endif  // ARCH_ARM_V7A

#endif  // SPL_SPL_INL_H_
