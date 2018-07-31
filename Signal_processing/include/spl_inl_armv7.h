/*
 *  Copyright (c) 2011 The  project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


// This header file includes the inline functions for ARM processors in
// the fix point signal processing library.

#ifndef SPL_SPL_INL_ARMV7_H_
#define SPL_SPL_INL_ARMV7_H_

static __inline Word32 SPL_MUL_16_32_RSFT16(Word16 a,
                                                          Word32 b) {
  Word32 tmp;
  __asm__("smulwb %0, %1, %2":"=r"(tmp):"r"(b), "r"(a));
  return tmp;
}

static __inline Word32 SPL_MUL_32_32_RSFT32(Word16 a,
                                                          Word16 b,
                                                          Word32 c) {
  Word32 tmp;
  __asm__("pkhbt %0, %1, %2, lsl #16" : "=r"(tmp) : "r"(b), "r"(a));
  __asm__("smmul %0, %1, %2":"=r"(tmp):"r"(tmp), "r"(c));
  return tmp;
}

static __inline Word32 SPL_MUL_32_32_RSFT32BI(Word32 a,
                                                            Word32 b) {
  Word32 tmp;
  __asm__("smmul %0, %1, %2":"=r"(tmp):"r"(a), "r"(b));
  return tmp;
}

static __inline Word32 SPL_MUL_16_16(Word16 a,
                                                   Word16 b) {
  Word32 tmp;
  __asm__("smulbb %0, %1, %2":"=r"(tmp):"r"(a), "r"(b));
  return tmp;
}

static __inline Word32 MulAccumW16(Word16 a,
                                          Word16 b,
                                          Word32 c) {
  Word32 tmp = 0;
  __asm__("smlabb %0, %1, %2, %3":"=r"(tmp):"r"(a), "r"(b), "r"(c));
  return tmp;
}

static __inline Word16 Spl_AddSatW16(Word16 a,
                                                  Word16 b) {
  Word32 s_sum;

  __asm__("qadd16 %0, %1, %2":"=r"(s_sum):"r"(a), "r"(b));

  return (Word16) s_sum;
}

static __inline Word32 Spl_AddSatW32(Word32 l_var1,
                                                  Word32 l_var2) {
  Word32 l_sum;

  __asm__("qadd %0, %1, %2":"=r"(l_sum):"r"(l_var1), "r"(l_var2));

  return l_sum;
}

static __inline Word16 Spl_SubSatW16(Word16 var1,
                                                  Word16 var2) {
  Word32 s_sub;

  __asm__("qsub16 %0, %1, %2":"=r"(s_sub):"r"(var1), "r"(var2));

  return (Word16)s_sub;
}

static __inline Word32 Spl_SubSatW32(Word32 l_var1,
                                                  Word32 l_var2) {
  Word32 l_sub;

  __asm__("qsub %0, %1, %2":"=r"(l_sub):"r"(l_var1), "r"(l_var2));

  return l_sub;
}

static __inline Word16 Spl_GetSizeInBits(UWord32 n) {
  Word32 tmp;

  __asm__("clz %0, %1":"=r"(tmp):"r"(n));

  return (Word16)(32 - tmp);
}

static __inline int Spl_NormW32(Word32 a) {
  Word32 tmp;

  if (a <= 0) a ^= 0xFFFFFFFF;

  __asm__("clz %0, %1":"=r"(tmp):"r"(a));

  return tmp - 1;
}

static __inline int Spl_NormU32(UWord32 a) {
  int tmp;

  if (a == 0) return 0;

  __asm__("clz %0, %1":"=r"(tmp):"r"(a));

  return tmp;
}

static __inline int Spl_NormW16(Word16 a) {
  Word32 tmp;

  if (a <= 0) a ^= 0xFFFFFFFF;

  __asm__("clz %0, %1":"=r"(tmp):"r"(a));

  return tmp - 17;
}

static __inline Word16 Spl_SatW32ToW16(Word32 value32) {
  Word16 out16;

  __asm__("ssat %r0, #16, %r1" : "=r"(out16) : "r"(value32));

  return out16;
}
#endif  // SPL_SPL_INL_ARMV7_H_
