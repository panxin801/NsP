/*
 *  Copyright (c) 2011 The  project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


/*
 * This header file includes all of the fix point signal processing library (SPL) function
 * descriptions and declarations.
 * For specific function calls, see bottom of file.
 */

#ifndef SPL_SIGNAL_PROCESSING_LIBRARY_H_
#define SPL_SIGNAL_PROCESSING_LIBRARY_H_

#include <string.h>
#include "typedefs.h"

#ifdef ARM_WINM
#include <Armintr.h> // intrinsic file for windows mobile
#endif

// Macros specific for the fixed point implementation
#define SPL_WORD16_MAX       32767
#define SPL_WORD16_MIN       -32768
#define SPL_WORD32_MAX       (Word32)0x7fffffff
#define SPL_WORD32_MIN       (Word32)0x80000000
#define SPL_MAX_LPC_ORDER    14
#define SPL_MAX_SEED_USED    0x80000000L
#define SPL_MIN(A, B)        (A < B ? A : B) // Get min value
#define SPL_MAX(A, B)        (A > B ? A : B) // Get max value
#define SPL_ABS_W16(a) \
    (((Word16)a >= 0) ? ((Word16)a) : -((Word16)a))
#define SPL_ABS_W32(a) \
    (((Word32)a >= 0) ? ((Word32)a) : -((Word32)a))

#if (defined TARGET_PC)||(defined __TARGET_XSCALE)
#define SPL_GET_BYTE(a, nr)  (((Word8 *)a)[nr])
#define SPL_SET_BYTE(d_ptr, val, index) \
    (((Word8 *)d_ptr)[index] = (val))
#elif defined BIG_ENDIAN
#define SPL_GET_BYTE(a, nr) \
    ((((Word16 *)a)[nr >> 1]) >> (((nr + 1) & 0x1) * 8) & 0x00ff)
#define SPL_SET_BYTE(d_ptr, val, index) \
    ((Word16 *)d_ptr)[index >> 1] = \
    ((((Word16 *)d_ptr)[index >> 1]) \
    & (0x00ff << (8 * ((index) & 0x1)))) | (val << (8 * ((index + 1) & 0x1)))
#else
#define SPL_GET_BYTE(a,nr) \
    ((((Word16 *)(a))[(nr) >> 1]) >> (((nr) & 0x1) * 8) & 0x00ff)
#define SPL_SET_BYTE(d_ptr, val, index) \
    ((Word16 *)(d_ptr))[(index) >> 1] = \
    ((((Word16 *)(d_ptr))[(index) >> 1]) \
    & (0x00ff << (8 * (((index) + 1) & 0x1)))) | \
    ((val) << (8 * ((index) & 0x1)))
#endif

#define SPL_MUL(a, b) \
    ((Word32) ((Word32)(a) * (Word32)(b)))
#define SPL_UMUL(a, b) \
    ((UWord32) ((UWord32)(a) * (UWord32)(b)))
#define SPL_UMUL_RSFT16(a, b) \
    ((UWord32) ((UWord32)(a) * (UWord32)(b)) >> 16)
#define SPL_UMUL_16_16(a, b) \
    ((UWord32) (UWord16)(a) * (UWord16)(b))
#define SPL_UMUL_16_16_RSFT16(a, b) \
    (((UWord32) (UWord16)(a) * (UWord16)(b)) >> 16)
#define SPL_UMUL_32_16(a, b) \
    ((UWord32) ((UWord32)(a) * (UWord16)(b)))
#define SPL_UMUL_32_16_RSFT16(a, b) \
    ((UWord32) ((UWord32)(a) * (UWord16)(b)) >> 16)
#define SPL_MUL_16_U16(a, b) \
    ((Word32)(Word16)(a) * (UWord16)(b))
#define SPL_DIV(a, b) \
    ((Word32) ((Word32)(a) / (Word32)(b)))
#define SPL_UDIV(a, b) \
    ((UWord32) ((UWord32)(a) / (UWord32)(b)))

#ifndef ARCH_ARM_V7A
// For ARMv7 platforms, these are inline functions in spl_inl_armv7.h
#define SPL_MUL_16_16(a, b) \
    ((Word32) (((Word16)(a)) * ((Word16)(b))))
#define SPL_MUL_16_32_RSFT16(a, b) \
    (SPL_MUL_16_16(a, b >> 16) \
     + ((SPL_MUL_16_16(a, (b & 0xffff) >> 1) + 0x4000) >> 15))
#define SPL_MUL_32_32_RSFT32(a32a, a32b, b32) \
    ((Word32)(SPL_MUL_16_32_RSFT16(a32a, b32) \
    + (SPL_MUL_16_32_RSFT16(a32b, b32) >> 16)))
#define SPL_MUL_32_32_RSFT32BI(a32, b32) \
    ((Word32)(SPL_MUL_16_32_RSFT16(( \
    (Word16)(a32 >> 16)), b32) + \
    (SPL_MUL_16_32_RSFT16(( \
    (Word16)((a32 & 0x0000FFFF) >> 1)), b32) >> 15)))
#endif

#define SPL_MUL_16_32_RSFT11(a, b) \
    ((SPL_MUL_16_16(a, (b) >> 16) << 5) \
    + (((SPL_MUL_16_U16(a, (UWord16)(b)) >> 1) + 0x0200) >> 10))
#define SPL_MUL_16_32_RSFT14(a, b) \
    ((SPL_MUL_16_16(a, (b) >> 16) << 2) \
    + (((SPL_MUL_16_U16(a, (UWord16)(b)) >> 1) + 0x1000) >> 13))
#define SPL_MUL_16_32_RSFT15(a, b) \
    ((SPL_MUL_16_16(a, (b) >> 16) << 1) \
    + (((SPL_MUL_16_U16(a, (UWord16)(b)) >> 1) + 0x2000) >> 14))

#ifdef ARM_WINM
#define SPL_MUL_16_16(a, b) \
    _SmulLo_SW_SL((Word16)(a), (Word16)(b))
#endif

#define SPL_MUL_16_16_RSFT(a, b, c) \
    (SPL_MUL_16_16(a, b) >> (c))

#define SPL_MUL_16_16_RSFT_WITH_ROUND(a, b, c) \
    ((SPL_MUL_16_16(a, b) + ((Word32) \
                                  (((Word32)1) << ((c) - 1)))) >> (c))
#define SPL_MUL_16_16_RSFT_WITH_FIXROUND(a, b) \
    ((SPL_MUL_16_16(a, b) + ((Word32) (1 << 14))) >> 15)

// C + the 32 most significant bits of A * B
#define SPL_SCALEDIFF32(A, B, C) \
    (C + (B >> 16) * A + (((UWord32)(0x0000FFFF & B) * A) >> 16))

#define SPL_ADD_SAT_W32(a, b)    Spl_AddSatW32(a, b)
#define SPL_SAT(a, b, c)         (b > a ? a : b < c ? c : b)
#define SPL_MUL_32_16(a, b)      ((a) * (b))

#define SPL_SUB_SAT_W32(a, b)    Spl_SubSatW32(a, b)
#define SPL_ADD_SAT_W16(a, b)    Spl_AddSatW16(a, b)
#define SPL_SUB_SAT_W16(a, b)    Spl_SubSatW16(a, b)

// We cannot do casting here due to signed/unsigned problem
#define SPL_IS_NEG(a)            ((a) & 0x80000000)
// Shifting with negative numbers allowed
// Positive means left shift
#define SPL_SHIFT_W16(x, c) \
    (((c) >= 0) ? ((x) << (c)) : ((x) >> (-(c))))
#define SPL_SHIFT_W32(x, c) \
    (((c) >= 0) ? ((x) << (c)) : ((x) >> (-(c))))

// Shifting with negative numbers not allowed
// We cannot do casting here due to signed/unsigned problem
#define SPL_RSHIFT_W16(x, c)     ((x) >> (c))
#define SPL_LSHIFT_W16(x, c)     ((x) << (c))
#define SPL_RSHIFT_W32(x, c)     ((x) >> (c))
#define SPL_LSHIFT_W32(x, c)     ((x) << (c))

#define SPL_RSHIFT_U16(x, c)     ((UWord16)(x) >> (c))
#define SPL_LSHIFT_U16(x, c)     ((UWord16)(x) << (c))
#define SPL_RSHIFT_U32(x, c)     ((UWord32)(x) >> (c))
#define SPL_LSHIFT_U32(x, c)     ((UWord32)(x) << (c))

#define SPL_VNEW(t, n)           (t *) malloc (sizeof (t) * (n))
#define SPL_FREE                 free

#define SPL_RAND(a) \
    ((Word16)(SPL_MUL_16_16_RSFT((a), 18816, 7) & 0x00007fff))

#ifdef __cplusplus
extern "C"
{
#endif

#define SPL_MEMCPY_W8(v1, v2, length) \
   memcpy(v1, v2, (length) * sizeof(char))
#define SPL_MEMCPY_W16(v1, v2, length) \
   memcpy(v1, v2, (length) * sizeof(Word16))

#define SPL_MEMMOVE_W16(v1, v2, length) \
   memmove(v1, v2, (length) * sizeof(Word16))

// inline functions:
#include "spl_inl.h"

// Get SPL Version
Word16 Spl_get_version(char* version,
                                    Word16 length_in_bytes);

int ANS_YL_Spl_GetScalingSquare(Word16* in_vector,
                               int in_vector_length,
                               int times);

// Copy and set operations. Implementation in copy_set_operations.c.
// Descriptions at bottom of file.
void ANS_YL_Spl_MemSetW16(Word16* vector,
                         Word16 set_value,
                         int vector_length);
void ANS_YL_Spl_MemSetW32(Word32* vector,
                         Word32 set_value,
                         int vector_length);
void ANS_YL_Spl_MemCpyReversedOrder(Word16* out_vector,
                                   Word16* in_vector,
                                   int vector_length);
Word16 ANS_YL_Spl_CopyFromEndW16(G_CONST Word16* in_vector,
                                       Word16 in_vector_length,
                                       Word16 samples,
                                       Word16* out_vector);
Word16 ANS_YL_Spl_ZerosArrayW16(Word16* vector,
                                      Word16 vector_length);
Word16 ANS_YL_Spl_ZerosArrayW32(Word32* vector,
                                      Word16 vector_length);
Word16 ANS_YL_Spl_OnesArrayW16(Word16* vector,
                                     Word16 vector_length);
Word16 ANS_YL_Spl_OnesArrayW32(Word32* vector,
                                     Word16 vector_length);
// End: Copy and set operations.

// Minimum and maximum operations. Implementation in min_max_operations.c.
// Descriptions at bottom of file.
Word16 ANS_YL_Spl_MaxAbsValueW16(const Word16* vector,
                                       Word16 length);
Word32 ANS_YL_Spl_MaxAbsValueW32(G_CONST Word32* vector,
                                       Word16 length);
Word16 ANS_YL_Spl_MinValueW16(G_CONST Word16* vector,
                                    Word16 length);
Word32 ANS_YL_Spl_MinValueW32(G_CONST Word32* vector,
                                    Word16 length);
Word16 ANS_YL_Spl_MaxValueW16(G_CONST Word16* vector,
                                    Word16 length);

Word16 ANS_YL_Spl_MaxAbsIndexW16(G_CONST Word16* vector,
                                       Word16 length);
Word32 ANS_YL_Spl_MaxValueW32(G_CONST Word32* vector,
                                    Word16 length);
Word16 ANS_YL_Spl_MinIndexW16(G_CONST Word16* vector,
                                    Word16 length);
Word16 ANS_YL_Spl_MinIndexW32(G_CONST Word32* vector,
                                    Word16 length);
Word16 ANS_YL_Spl_MaxIndexW16(G_CONST Word16* vector,
                                    Word16 length);
Word16 ANS_YL_Spl_MaxIndexW32(G_CONST Word32* vector,
                                    Word16 length);
// End: Minimum and maximum operations.

// Vector scaling operations. Implementation in vector_scaling_operations.c.
// Description at bottom of file.
void Spl_VectorBitShiftW16(Word16* out_vector,
                                 Word16 vector_length,
                                 G_CONST Word16* in_vector,
                                 Word16 right_shifts);
void Spl_VectorBitShiftW32(Word32* out_vector,
                                 Word16 vector_length,
                                 G_CONST Word32* in_vector,
                                 Word16 right_shifts);
void Spl_VectorBitShiftW32ToW16(Word16* out_vector,
                                      Word16 vector_length,
                                      G_CONST Word32* in_vector,
                                      Word16 right_shifts);

void Spl_ScaleVector(G_CONST Word16* in_vector,
                           Word16* out_vector,
                           Word16 gain,
                           Word16 vector_length,
                           Word16 right_shifts);
void Spl_ScaleVectorWithSat(G_CONST Word16* in_vector,
                                  Word16* out_vector,
                                  Word16 gain,
                                  Word16 vector_length,
                                  Word16 right_shifts);
void Spl_ScaleAndAddVectors(G_CONST Word16* in_vector1,
                                  Word16 gain1, int right_shifts1,
                                  G_CONST Word16* in_vector2,
                                  Word16 gain2, int right_shifts2,
                                  Word16* out_vector,
                                  int vector_length);
// End: Vector scaling operations.

// iLBC specific functions. Implementations in ilbc_specific_functions.c.
// Description at bottom of file.
void Spl_ScaleAndAddVectorsWithRound(Word16* in_vector1,
                                           Word16 scale1,
                                           Word16* in_vector2,
                                           Word16 scale2,
                                           Word16 right_shifts,
                                           Word16* out_vector,
                                           Word16 vector_length);
void Spl_ReverseOrderMultArrayElements(Word16* out_vector,
                                             G_CONST Word16* in_vector,
                                             G_CONST Word16* window,
                                             Word16 vector_length,
                                             Word16 right_shifts);
void Spl_ElementwiseVectorMult(Word16* out_vector,
                                     G_CONST Word16* in_vector,
                                     G_CONST Word16* window,
                                     Word16 vector_length,
                                     Word16 right_shifts);
void Spl_AddVectorsAndShift(Word16* out_vector,
                                  G_CONST Word16* in_vector1,
                                  G_CONST Word16* in_vector2,
                                  Word16 vector_length,
                                  Word16 right_shifts);
void Spl_AddAffineVectorToVector(Word16* out_vector,
                                       Word16* in_vector,
                                       Word16 gain,
                                       Word32 add_constant,
                                       Word16 right_shifts,
                                       int vector_length);
void Spl_AffineTransformVector(Word16* out_vector,
                                     Word16* in_vector,
                                     Word16 gain,
                                     Word32 add_constant,
                                     Word16 right_shifts,
                                     int vector_length);
// End: iLBC specific functions.

// Signal processing operations. Descriptions at bottom of this file.
int Spl_AutoCorrelation(G_CONST Word16* vector,
                              int vector_length, int order,
                              Word32* result_vector,
                              int* scale);
Word16 Spl_LevinsonDurbin(Word32* auto_corr,
                                       Word16* lpc_coef,
                                       Word16* refl_coef,
                                       Word16 order);
void Spl_ReflCoefToLpc(G_CONST Word16* refl_coef,
                             int use_order,
                             Word16* lpc_coef);
void Spl_LpcToReflCoef(Word16* lpc_coef,
                             int use_order,
                             Word16* refl_coef);
void Spl_AutoCorrToReflCoef(G_CONST Word32* auto_corr,
                                  int use_order,
                                  Word16* refl_coef);
void Spl_CrossCorrelation(Word32* cross_corr,
                                Word16* vector1,
                                Word16* vector2,
                                Word16 dim_vector,
                                Word16 dim_cross_corr,
                                Word16 right_shifts,
                                Word16 step_vector2);
void Spl_GetHanningWindow(Word16* window, Word16 size);
void Spl_SqrtOfOneMinusXSquared(Word16* in_vector,
                                      int vector_length,
                                      Word16* out_vector);
// End: Signal processing operations.

// Randomization functions. Implementations collected in randomization_functions.c and
// descriptions at bottom of this file.
UWord32 Spl_IncreaseSeed(UWord32* seed);
Word16 Spl_RandU(UWord32* seed);
Word16 Spl_RandN(UWord32* seed);
Word16 Spl_RandUArray(Word16* vector,
                                   Word16 vector_length,
                                   UWord32* seed);
// End: Randomization functions.

// Math functions
Word32 Spl_Sqrt(Word32 value);
Word32 ANS_YL_Spl_SqrtFloor(Word32 value);

// Divisions. Implementations collected in division_operations.c and
// descriptions at bottom of this file.
UWord32 ANS_YL_Spl_DivU32U16(UWord32 num, UWord16 den);
Word32 ANS_YL_Spl_DivW32W16(Word32 num, Word16 den);
Word16 ANS_YL_Spl_DivW32W16ResW16(Word32 num, Word16 den);
Word32 ANS_YL_Spl_DivResultInQ31(Word32 num, Word32 den);
Word32 ANS_YL_Spl_DivW32HiLow(Word32 num, Word16 den_hi,
                                    Word16 den_low);
// End: Divisions.

Word32 ANS_YL_Spl_Energy(Word16* vector,
                               int vector_length,
                               int* scale_factor);

Word32 Spl_DotProductWithScale(Word16* vector1,
                                            Word16* vector2,
                                            int vector_length,
                                            int scaling);

// Filter operations.
int Spl_FilterAR(G_CONST Word16* ar_coef, int ar_coef_length,
                       G_CONST Word16* in_vector, int in_vector_length,
                       Word16* filter_state, int filter_state_length,
                       Word16* filter_state_low,
                       int filter_state_low_length, Word16* out_vector,
                       Word16* out_vector_low, int out_vector_low_length);

void Spl_FilterMAFastQ12(Word16* in_vector,
                               Word16* out_vector,
                               Word16* ma_coef,
                               Word16 ma_coef_length,
                               Word16 vector_length);
void Spl_FilterARFastQ12(Word16* in_vector,
                               Word16* out_vector,
                               Word16* ar_coef,
                               Word16 ar_coef_length,
                               Word16 vector_length);
int Spl_DownsampleFast(Word16* in_vector,
                             Word16 in_vector_length,
                             Word16* out_vector,
                             Word16 out_vector_length,
                             Word16* ma_coef,
                             Word16 ma_coef_length,
                             Word16 factor,
                             Word16 delay);
// End: Filter operations.

// FFT operations
int ANS_YL_Spl_ComplexFFT(Word16 vector[], int stages, int mode);
int ANS_YL_Spl_ComplexIFFT(Word16 vector[], int stages, int mode);
void ANS_YL_Spl_ComplexBitReverse(Word16 vector[], int stages);
// End: FFT operations

/************************************************************
 *
 * RESAMPLING FUNCTIONS AND THEIR STRUCTS ARE DEFINED BELOW
 *
 ************************************************************/

/*******************************************************************
 * resample.c
 *
 * Includes the following resampling combinations
 * 22 kHz -> 16 kHz
 * 16 kHz -> 22 kHz
 * 22 kHz ->  8 kHz
 *  8 kHz -> 22 kHz
 *
 ******************************************************************/

// state structure for 22 -> 16 resampler
typedef struct
{
    Word32 S_22_44[8];
    Word32 S_44_32[8];
    Word32 S_32_16[8];
} Spl_State22khzTo16khz;

void Spl_Resample22khzTo16khz(const Word16* in,
                                    Word16* out,
                                    Spl_State22khzTo16khz* state,
                                    Word32* tmpmem);

void Spl_ResetResample22khzTo16khz(Spl_State22khzTo16khz* state);

// state structure for 16 -> 22 resampler
typedef struct
{
    Word32 S_16_32[8];
    Word32 S_32_22[8];
} Spl_State16khzTo22khz;

void Spl_Resample16khzTo22khz(const Word16* in,
                                    Word16* out,
                                    Spl_State16khzTo22khz* state,
                                    Word32* tmpmem);

void Spl_ResetResample16khzTo22khz(Spl_State16khzTo22khz* state);

// state structure for 22 -> 8 resampler
typedef struct
{
    Word32 S_22_22[16];
    Word32 S_22_16[8];
    Word32 S_16_8[8];
} Spl_State22khzTo8khz;

void Spl_Resample22khzTo8khz(const Word16* in, Word16* out,
                                   Spl_State22khzTo8khz* state,
                                   Word32* tmpmem);

void Spl_ResetResample22khzTo8khz(Spl_State22khzTo8khz* state);

// state structure for 8 -> 22 resampler
typedef struct
{
    Word32 S_8_16[8];
    Word32 S_16_11[8];
    Word32 S_11_22[8];
} Spl_State8khzTo22khz;

void Spl_Resample8khzTo22khz(const Word16* in, Word16* out,
                                   Spl_State8khzTo22khz* state,
                                   Word32* tmpmem);

void Spl_ResetResample8khzTo22khz(Spl_State8khzTo22khz* state);

/*******************************************************************
 * resample_fractional.c
 * Functions for internal use in the other resample functions
 *
 * Includes the following resampling combinations
 * 48 kHz -> 32 kHz
 * 32 kHz -> 24 kHz
 * 44 kHz -> 32 kHz
 *
 ******************************************************************/

void Spl_Resample48khzTo32khz(const Word32* In, Word32* Out,
                                    const Word32 K);

void Spl_Resample32khzTo24khz(const Word32* In, Word32* Out,
                                    const Word32 K);

void Spl_Resample44khzTo32khz(const Word32* In, Word32* Out,
                                    const Word32 K);

/*******************************************************************
 * resample_48khz.c
 *
 * Includes the following resampling combinations
 * 48 kHz -> 16 kHz
 * 16 kHz -> 48 kHz
 * 48 kHz ->  8 kHz
 *  8 kHz -> 48 kHz
 *
 ******************************************************************/

typedef struct
{
    Word32 S_48_48[16];
    Word32 S_48_32[8];
    Word32 S_32_16[8];
} Spl_State48khzTo16khz;

void Spl_Resample48khzTo16khz(const Word16* in, Word16* out,
                                    Spl_State48khzTo16khz* state,
                                    Word32* tmpmem);

void Spl_ResetResample48khzTo16khz(Spl_State48khzTo16khz* state);

typedef struct
{
    Word32 S_16_32[8];
    Word32 S_32_24[8];
    Word32 S_24_48[8];
} Spl_State16khzTo48khz;

void Spl_Resample16khzTo48khz(const Word16* in, Word16* out,
                                    Spl_State16khzTo48khz* state,
                                    Word32* tmpmem);

void Spl_ResetResample16khzTo48khz(Spl_State16khzTo48khz* state);

typedef struct
{
    Word32 S_48_24[8];
    Word32 S_24_24[16];
    Word32 S_24_16[8];
    Word32 S_16_8[8];
} Spl_State48khzTo8khz;

void Spl_Resample48khzTo8khz(const Word16* in, Word16* out,
                                   Spl_State48khzTo8khz* state,
                                   Word32* tmpmem);

void Spl_ResetResample48khzTo8khz(Spl_State48khzTo8khz* state);

typedef struct
{
    Word32 S_8_16[8];
    Word32 S_16_12[8];
    Word32 S_12_24[8];
    Word32 S_24_48[8];
} Spl_State8khzTo48khz;

void Spl_Resample8khzTo48khz(const Word16* in, Word16* out,
                                   Spl_State8khzTo48khz* state,
                                   Word32* tmpmem);

void Spl_ResetResample8khzTo48khz(Spl_State8khzTo48khz* state);

/*******************************************************************
 * resample_by_2.c
 *
 * Includes down and up sampling by a factor of two.
 *
 ******************************************************************/

void Spl_DownsampleBy2(const Word16* in, const Word16 len,
                             Word16* out, Word32* filtState);

void Spl_UpsampleBy2(const Word16* in, Word16 len, Word16* out,
                           Word32* filtState);

/************************************************************
 * END OF RESAMPLING FUNCTIONS
 ************************************************************/
void Spl_AnalysisQMF(const Word16* in_data,
                           Word16* low_band,
                           Word16* high_band,
                           Word32* filter_state1,
                           Word32* filter_state2);
void Spl_SynthesisQMF(const Word16* low_band,
                            const Word16* high_band,
                            Word16* out_data,
                            Word32* filter_state1,
                            Word32* filter_state2);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SPL_SIGNAL_PROCESSING_LIBRARY_H_

//
// Spl_AddSatW16(...)
// Spl_AddSatW32(...)
//
// Returns the result of a saturated 16-bit, respectively 32-bit, addition of
// the numbers specified by the |var1| and |var2| parameters.
//
// Input:
//      - var1      : Input variable 1
//      - var2      : Input variable 2
//
// Return value     : Added and saturated value
//

//
// Spl_SubSatW16(...)
// Spl_SubSatW32(...)
//
// Returns the result of a saturated 16-bit, respectively 32-bit, subtraction
// of the numbers specified by the |var1| and |var2| parameters.
//
// Input:
//      - var1      : Input variable 1
//      - var2      : Input variable 2
//
// Returned value   : Subtracted and saturated value
//

//
// Spl_GetSizeInBits(...)
//
// Returns the # of bits that are needed at the most to represent the number
// specified by the |value| parameter.
//
// Input:
//      - value     : Input value
//
// Return value     : Number of bits needed to represent |value|
//

//
// Spl_NormW32(...)
//
// Norm returns the # of left shifts required to 32-bit normalize the 32-bit
// signed number specified by the |value| parameter.
//
// Input:
//      - value     : Input value
//
// Return value     : Number of bit shifts needed to 32-bit normalize |value|
//

//
// Spl_NormW16(...)
//
// Norm returns the # of left shifts required to 16-bit normalize the 16-bit
// signed number specified by the |value| parameter.
//
// Input:
//      - value     : Input value
//
// Return value     : Number of bit shifts needed to 32-bit normalize |value|
//

//
// Spl_NormU32(...)
//
// Norm returns the # of left shifts required to 32-bit normalize the unsigned
// 32-bit number specified by the |value| parameter.
//
// Input:
//      - value     : Input value
//
// Return value     : Number of bit shifts needed to 32-bit normalize |value|
//

//
// ANS_YL_Spl_GetScalingSquare(...)
//
// Returns the # of bits required to scale the samples specified in the
// |in_vector| parameter so that, if the squares of the samples are added the
// # of times specified by the |times| parameter, the 32-bit addition will not
// overflow (result in Word32).
//
// Input:
//      - in_vector         : Input vector to check scaling on
//      - in_vector_length  : Samples in |in_vector|
//      - times             : Number of additions to be performed
//
// Return value             : Number of right bit shifts needed to avoid
//                            overflow in the addition calculation
//

//
// ANS_YL_Spl_MemSetW16(...)
//
// Sets all the values in the Word16 vector |vector| of length
// |vector_length| to the specified value |set_value|
//
// Input:
//      - vector        : Pointer to the Word16 vector
//      - set_value     : Value specified
//      - vector_length : Length of vector
//

//
// ANS_YL_Spl_MemSetW32(...)
//
// Sets all the values in the Word32 vector |vector| of length
// |vector_length| to the specified value |set_value|
//
// Input:
//      - vector        : Pointer to the Word16 vector
//      - set_value     : Value specified
//      - vector_length : Length of vector
//

//
// ANS_YL_Spl_MemCpyReversedOrder(...)
//
// Copies all the values from the source Word16 vector |in_vector| to a
// destination Word16 vector |out_vector|. It is done in reversed order,
// meaning that the first sample of |in_vector| is copied to the last sample of
// the |out_vector|. The procedure continues until the last sample of
// |in_vector| has been copied to the first sample of |out_vector|. This
// creates a reversed vector. Used in e.g. prediction in iLBC.
//
// Input:
//      - in_vector     : Pointer to the first sample in a Word16 vector
//                        of length |length|
//      - vector_length : Number of elements to copy
//
// Output:
//      - out_vector    : Pointer to the last sample in a Word16 vector
//                        of length |length|
//

//
// ANS_YL_Spl_CopyFromEndW16(...)
//
// Copies the rightmost |samples| of |in_vector| (of length |in_vector_length|)
// to the vector |out_vector|.
//
// Input:
//      - in_vector         : Input vector
//      - in_vector_length  : Number of samples in |in_vector|
//      - samples           : Number of samples to extract (from right side)
//                            from |in_vector|
//
// Output:
//      - out_vector        : Vector with the requested samples
//
// Return value             : Number of copied samples in |out_vector|
//

//
// ANS_YL_Spl_ZerosArrayW16(...)
// ANS_YL_Spl_ZerosArrayW32(...)
//
// Inserts the value "zero" in all positions of a w16 and a w32 vector
// respectively.
//
// Input:
//      - vector_length : Number of samples in vector
//
// Output:
//      - vector        : Vector containing all zeros
//
// Return value         : Number of samples in vector
//

//
// ANS_YL_Spl_OnesArrayW16(...)
// ANS_YL_Spl_OnesArrayW32(...)
//
// Inserts the value "one" in all positions of a w16 and a w32 vector
// respectively.
//
// Input:
//      - vector_length : Number of samples in vector
//
// Output:
//      - vector        : Vector containing all ones
//
// Return value         : Number of samples in vector
//

//
// ANS_YL_Spl_MinValueW16(...)
// ANS_YL_Spl_MinValueW32(...)
//
// Returns the minimum value of a vector
//
// Input:
//      - vector        : Input vector
//      - vector_length : Number of samples in vector
//
// Return value         : Minimum sample value in vector
//

//
// ANS_YL_Spl_MaxValueW16(...)
// ANS_YL_Spl_MaxValueW32(...)
//
// Returns the maximum value of a vector
//
// Input:
//      - vector        : Input vector
//      - vector_length : Number of samples in vector
//
// Return value         : Maximum sample value in vector
//

//
// ANS_YL_Spl_MaxAbsValueW16(...)
// ANS_YL_Spl_MaxAbsValueW32(...)
//
// Returns the largest absolute value of a vector
//
// Input:
//      - vector        : Input vector
//      - vector_length : Number of samples in vector
//
// Return value         : Maximum absolute value in vector
//

//
// ANS_YL_Spl_MaxAbsIndexW16(...)
//
// Returns the vector index to the largest absolute value of a vector
//
// Input:
//      - vector        : Input vector
//      - vector_length : Number of samples in vector
//
// Return value         : Index to maximum absolute value in vector
//

//
// ANS_YL_Spl_MinIndexW16(...)
// ANS_YL_Spl_MinIndexW32(...)
//
// Returns the vector index to the minimum sample value of a vector
//
// Input:
//      - vector        : Input vector
//      - vector_length : Number of samples in vector
//
// Return value         : Index to minimum sample value in vector
//

//
// ANS_YL_Spl_MaxIndexW16(...)
// ANS_YL_Spl_MaxIndexW32(...)
//
// Returns the vector index to the maximum sample value of a vector
//
// Input:
//      - vector        : Input vector
//      - vector_length : Number of samples in vector
//
// Return value         : Index to maximum sample value in vector
//

//
// Spl_VectorBitShiftW16(...)
// Spl_VectorBitShiftW32(...)
//
// Bit shifts all the values in a vector up or downwards. Different calls for
// Word16 and Word32 vectors respectively.
//
// Input:
//      - vector_length : Length of vector
//      - in_vector     : Pointer to the vector that should be bit shifted
//      - right_shifts  : Number of right bit shifts (negative value gives left
//                        shifts)
//
// Output:
//      - out_vector    : Pointer to the result vector (can be the same as
//                        |in_vector|)
//

//
// Spl_VectorBitShiftW32ToW16(...)
//
// Bit shifts all the values in a Word32 vector up or downwards and
// stores the result as a Word16 vector
//
// Input:
//      - vector_length : Length of vector
//      - in_vector     : Pointer to the vector that should be bit shifted
//      - right_shifts  : Number of right bit shifts (negative value gives left
//                        shifts)
//
// Output:
//      - out_vector    : Pointer to the result vector (can be the same as
//                        |in_vector|)
//

//
// Spl_ScaleVector(...)
//
// Performs the vector operation:
//  out_vector[k] = (gain*in_vector[k])>>right_shifts
//
// Input:
//      - in_vector     : Input vector
//      - gain          : Scaling gain
//      - vector_length : Elements in the |in_vector|
//      - right_shifts  : Number of right bit shifts applied
//
// Output:
//      - out_vector    : Output vector (can be the same as |in_vector|)
//

//
// Spl_ScaleVectorWithSat(...)
//
// Performs the vector operation:
//  out_vector[k] = SATURATE( (gain*in_vector[k])>>right_shifts )
//
// Input:
//      - in_vector     : Input vector
//      - gain          : Scaling gain
//      - vector_length : Elements in the |in_vector|
//      - right_shifts  : Number of right bit shifts applied
//
// Output:
//      - out_vector    : Output vector (can be the same as |in_vector|)
//

//
// Spl_ScaleAndAddVectors(...)
//
// Performs the vector operation:
//  out_vector[k] = (gain1*in_vector1[k])>>right_shifts1
//                  + (gain2*in_vector2[k])>>right_shifts2
//
// Input:
//      - in_vector1    : Input vector 1
//      - gain1         : Gain to be used for vector 1
//      - right_shifts1 : Right bit shift to be used for vector 1
//      - in_vector2    : Input vector 2
//      - gain2         : Gain to be used for vector 2
//      - right_shifts2 : Right bit shift to be used for vector 2
//      - vector_length : Elements in the input vectors
//
// Output:
//      - out_vector    : Output vector
//

//
// Spl_ScaleAndAddVectorsWithRound(...)
//
// Performs the vector operation:
//
//  out_vector[k] = ((scale1*in_vector1[k]) + (scale2*in_vector2[k])
//                      + round_value) >> right_shifts
//
//      where:
//
//  round_value = (1<<right_shifts)>>1
//
// Input:
//      - in_vector1    : Input vector 1
//      - scale1        : Gain to be used for vector 1
//      - in_vector2    : Input vector 2
//      - scale2        : Gain to be used for vector 2
//      - right_shifts  : Number of right bit shifts to be applied
//      - vector_length : Number of elements in the input vectors
//
// Output:
//      - out_vector    : Output vector
//

//
// Spl_ReverseOrderMultArrayElements(...)
//
// Performs the vector operation:
//  out_vector[n] = (in_vector[n]*window[-n])>>right_shifts
//
// Input:
//      - in_vector     : Input vector
//      - window        : Window vector (should be reversed). The pointer
//                        should be set to the last value in the vector
//      - right_shifts  : Number of right bit shift to be applied after the
//                        multiplication
//      - vector_length : Number of elements in |in_vector|
//
// Output:
//      - out_vector    : Output vector (can be same as |in_vector|)
//

//
// Spl_ElementwiseVectorMult(...)
//
// Performs the vector operation:
//  out_vector[n] = (in_vector[n]*window[n])>>right_shifts
//
// Input:
//      - in_vector     : Input vector
//      - window        : Window vector.
//      - right_shifts  : Number of right bit shift to be applied after the
//                        multiplication
//      - vector_length : Number of elements in |in_vector|
//
// Output:
//      - out_vector    : Output vector (can be same as |in_vector|)
//

//
// Spl_AddVectorsAndShift(...)
//
// Performs the vector operation:
//  out_vector[k] = (in_vector1[k] + in_vector2[k])>>right_shifts
//
// Input:
//      - in_vector1    : Input vector 1
//      - in_vector2    : Input vector 2
//      - right_shifts  : Number of right bit shift to be applied after the
//                        multiplication
//      - vector_length : Number of elements in |in_vector1| and |in_vector2|
//
// Output:
//      - out_vector    : Output vector (can be same as |in_vector1|)
//

//
// Spl_AddAffineVectorToVector(...)
//
// Adds an affine transformed vector to another vector |out_vector|, i.e,
// performs
//  out_vector[k] += (in_vector[k]*gain+add_constant)>>right_shifts
//
// Input:
//      - in_vector     : Input vector
//      - gain          : Gain value, used to multiply the in vector with
//      - add_constant  : Constant value to add (usually 1<<(right_shifts-1),
//                        but others can be used as well
//      - right_shifts  : Number of right bit shifts (0-16)
//      - vector_length : Number of samples in |in_vector| and |out_vector|
//
// Output:
//      - out_vector    : Vector with the output
//

//
// Spl_AffineTransformVector(...)
//
// Affine transforms a vector, i.e, performs
//  out_vector[k] = (in_vector[k]*gain+add_constant)>>right_shifts
//
// Input:
//      - in_vector     : Input vector
//      - gain          : Gain value, used to multiply the in vector with
//      - add_constant  : Constant value to add (usually 1<<(right_shifts-1),
//                        but others can be used as well
//      - right_shifts  : Number of right bit shifts (0-16)
//      - vector_length : Number of samples in |in_vector| and |out_vector|
//
// Output:
//      - out_vector    : Vector with the output
//

//
// Spl_AutoCorrelation(...)
//
// A 32-bit fix-point implementation of auto-correlation computation
//
// Input:
//      - vector        : Vector to calculate autocorrelation upon
//      - vector_length : Length (in samples) of |vector|
//      - order         : The order up to which the autocorrelation should be
//                        calculated
//
// Output:
//      - result_vector : auto-correlation values (values should be seen
//                        relative to each other since the absolute values
//                        might have been down shifted to avoid overflow)
//
//      - scale         : The number of left shifts required to obtain the
//                        auto-correlation in Q0
//
// Return value         : Number of samples in |result_vector|, i.e., (order+1)
//

//
// Spl_LevinsonDurbin(...)
//
// A 32-bit fix-point implementation of the Levinson-Durbin algorithm that
// does NOT use the 64 bit class
//
// Input:
//      - auto_corr : Vector with autocorrelation values of length >=
//                    |use_order|+1
//      - use_order : The LPC filter order (support up to order 20)
//
// Output:
//      - lpc_coef  : lpc_coef[0..use_order] LPC coefficients in Q12
//      - refl_coef : refl_coef[0...use_order-1]| Reflection coefficients in
//                    Q15
//
// Return value     : 1 for stable 0 for unstable
//

//
// Spl_ReflCoefToLpc(...)
//
// Converts reflection coefficients |refl_coef| to LPC coefficients |lpc_coef|.
// This version is a 16 bit operation.
//
// NOTE: The 16 bit refl_coef -> lpc_coef conversion might result in a
// "slightly unstable" filter (i.e., a pole just outside the unit circle) in
// "rare" cases even if the reflection coefficients are stable.
//
// Input:
//      - refl_coef : Reflection coefficients in Q15 that should be converted
//                    to LPC coefficients
//      - use_order : Number of coefficients in |refl_coef|
//
// Output:
//      - lpc_coef  : LPC coefficients in Q12
//

//
// Spl_LpcToReflCoef(...)
//
// Converts LPC coefficients |lpc_coef| to reflection coefficients |refl_coef|.
// This version is a 16 bit operation.
// The conversion is implemented by the step-down algorithm.
//
// Input:
//      - lpc_coef  : LPC coefficients in Q12, that should be converted to
//                    reflection coefficients
//      - use_order : Number of coefficients in |lpc_coef|
//
// Output:
//      - refl_coef : Reflection coefficients in Q15.
//

//
// Spl_AutoCorrToReflCoef(...)
//
// Calculates reflection coefficients (16 bit) from auto-correlation values
//
// Input:
//      - auto_corr : Auto-correlation values
//      - use_order : Number of coefficients wanted be calculated
//
// Output:
//      - refl_coef : Reflection coefficients in Q15.
//

//
// Spl_CrossCorrelation(...)
//
// Calculates the cross-correlation between two sequences |vector1| and
// |vector2|. |vector1| is fixed and |vector2| slides as the pointer is
// increased with the amount |step_vector2|
//
// Input:
//      - vector1           : First sequence (fixed throughout the correlation)
//      - vector2           : Second sequence (slides |step_vector2| for each
//                            new correlation)
//      - dim_vector        : Number of samples to use in the cross-correlation
//      - dim_cross_corr    : Number of cross-correlations to calculate (the
//                            start position for |vector2| is updated for each
//                            new one)
//      - right_shifts      : Number of right bit shifts to use. This will
//                            become the output Q-domain.
//      - step_vector2      : How many (positive or negative) steps the
//                            |vector2| pointer should be updated for each new
//                            cross-correlation value.
//
// Output:
//      - cross_corr        : The cross-correlation in Q(-right_shifts)
//

//
// Spl_GetHanningWindow(...)
//
// Creates (the first half of) a Hanning window. Size must be at least 1 and
// at most 512.
//
// Input:
//      - size      : Length of the requested Hanning window (1 to 512)
//
// Output:
//      - window    : Hanning vector in Q14.
//

//
// Spl_SqrtOfOneMinusXSquared(...)
//
// Calculates y[k] = sqrt(1 - x[k]^2) for each element of the input vector
// |in_vector|. Input and output values are in Q15.
//
// Inputs:
//      - in_vector     : Values to calculate sqrt(1 - x^2) of
//      - vector_length : Length of vector |in_vector|
//
// Output:
//      - out_vector    : Output values in Q15
//

//
// Spl_IncreaseSeed(...)
//
// Increases the seed (and returns the new value)
//
// Input:
//      - seed      : Seed for random calculation
//
// Output:
//      - seed      : Updated seed value
//
// Return value     : The new seed value
//

//
// Spl_RandU(...)
//
// Produces a uniformly distributed value in the Word16 range
//
// Input:
//      - seed      : Seed for random calculation
//
// Output:
//      - seed      : Updated seed value
//
// Return value     : Uniformly distributed value in the range
//                    [Word16_MIN...Word16_MAX]
//

//
// Spl_RandN(...)
//
// Produces a normal distributed value in the Word16 range
//
// Input:
//      - seed      : Seed for random calculation
//
// Output:
//      - seed      : Updated seed value
//
// Return value     : N(0,1) value in the Q13 domain
//

//
// Spl_RandUArray(...)
//
// Produces a uniformly distributed vector with elements in the Word16
// range
//
// Input:
//      - vector_length : Samples wanted in the vector
//      - seed          : Seed for random calculation
//
// Output:
//      - vector        : Vector with the uniform values
//      - seed          : Updated seed value
//
// Return value         : Number of samples in vector, i.e., |vector_length|
//

//
// Spl_Sqrt(...)
//
// Returns the square root of the input value |value|. The precision of this
// function is integer precision, i.e., sqrt(8) gives 2 as answer.
// If |value| is a negative number then 0 is returned.
//
// Algorithm:
//
// A sixth order Taylor Series expansion is used here to compute the square
// root of a number y^0.5 = (1+x)^0.5
// where
// x = y-1
//   = 1+(x/2)-0.5*((x/2)^2+0.5*((x/2)^3-0.625*((x/2)^4+0.875*((x/2)^5)
// 0.5 <= x < 1
//
// Input:
//      - value     : Value to calculate sqrt of
//
// Return value     : Result of the sqrt calculation
//

//
// ANS_YL_Spl_SqrtFloor(...)
//
// Returns the square root of the input value |value|. The precision of this
// function is rounding down integer precision, i.e., sqrt(8) gives 2 as answer.
// If |value| is a negative number then 0 is returned.
//
// Algorithm:
//
// An iterative 4 cylce/bit routine
//
// Input:
//      - value     : Value to calculate sqrt of
//
// Return value     : Result of the sqrt calculation
//

//
// ANS_YL_Spl_DivU32U16(...)
//
// Divides a UWord32 |num| by a UWord16 |den|.
//
// If |den|==0, (UWord32)0xFFFFFFFF is returned.
//
// Input:
//      - num       : Numerator
//      - den       : Denominator
//
// Return value     : Result of the division (as a UWord32), i.e., the
//                    integer part of num/den.
//

//
// ANS_YL_Spl_DivW32W16(...)
//
// Divides a Word32 |num| by a Word16 |den|.
//
// If |den|==0, (Word32)0x7FFFFFFF is returned.
//
// Input:
//      - num       : Numerator
//      - den       : Denominator
//
// Return value     : Result of the division (as a Word32), i.e., the
//                    integer part of num/den.
//

//
// ANS_YL_Spl_DivW32W16ResW16(...)
//
// Divides a Word32 |num| by a Word16 |den|, assuming that the
// result is less than 32768, otherwise an unpredictable result will occur.
//
// If |den|==0, (Word16)0x7FFF is returned.
//
// Input:
//      - num       : Numerator
//      - den       : Denominator
//
// Return value     : Result of the division (as a Word16), i.e., the
//                    integer part of num/den.
//

//
// ANS_YL_Spl_DivResultInQ31(...)
//
// Divides a Word32 |num| by a Word16 |den|, assuming that the
// absolute value of the denominator is larger than the numerator, otherwise
// an unpredictable result will occur.
//
// Input:
//      - num       : Numerator
//      - den       : Denominator
//
// Return value     : Result of the division in Q31.
//

//
// ANS_YL_Spl_DivW32HiLow(...)
//
// Divides a Word32 |num| by a denominator in hi, low format. The
// absolute value of the denominator has to be larger (or equal to) the
// numerator.
//
// Input:
//      - num       : Numerator
//      - den_hi    : High part of denominator
//      - den_low   : Low part of denominator
//
// Return value     : Divided value in Q31
//

//
// ANS_YL_Spl_Energy(...)
//
// Calculates the energy of a vector
//
// Input:
//      - vector        : Vector which the energy should be calculated on
//      - vector_length : Number of samples in vector
//
// Output:
//      - scale_factor  : Number of left bit shifts needed to get the physical
//                        energy value, i.e, to get the Q0 value
//
// Return value         : Energy value in Q(-|scale_factor|)
//

//
// Spl_FilterAR(...)
//
// Performs a 32-bit AR filtering on a vector in Q12
//
// Input:
//  - ar_coef                   : AR-coefficient vector (values in Q12),
//                                ar_coef[0] must be 4096.
//  - ar_coef_length            : Number of coefficients in |ar_coef|.
//  - in_vector                 : Vector to be filtered.
//  - in_vector_length          : Number of samples in |in_vector|.
//  - filter_state              : Current state (higher part) of the filter.
//  - filter_state_length       : Length (in samples) of |filter_state|.
//  - filter_state_low          : Current state (lower part) of the filter.
//  - filter_state_low_length   : Length (in samples) of |filter_state_low|.
//  - out_vector_low_length     : Maximum length (in samples) of
//                                |out_vector_low|.
//
// Output:
//  - filter_state              : Updated state (upper part) vector.
//  - filter_state_low          : Updated state (lower part) vector.
//  - out_vector                : Vector containing the upper part of the
//                                filtered values.
//  - out_vector_low            : Vector containing the lower part of the
//                                filtered values.
//
// Return value                 : Number of samples in the |out_vector|.
//

//
// Spl_FilterMAFastQ12(...)
//
// Performs a MA filtering on a vector in Q12
//
// Input:
//      - in_vector         : Input samples (state in positions
//                            in_vector[-order] .. in_vector[-1])
//      - ma_coef           : Filter coefficients (in Q12)
//      - ma_coef_length    : Number of B coefficients (order+1)
//      - vector_length     : Number of samples to be filtered
//
// Output:
//      - out_vector        : Filtered samples
//

//
// Spl_FilterARFastQ12(...)
//
// Performs a AR filtering on a vector in Q12
//
// Input:
//      - in_vector         : Input samples
//      - out_vector        : State information in positions
//                            out_vector[-order] .. out_vector[-1]
//      - ar_coef           : Filter coefficients (in Q12)
//      - ar_coef_length    : Number of B coefficients (order+1)
//      - vector_length     : Number of samples to be filtered
//
// Output:
//      - out_vector        : Filtered samples
//

//
// Spl_DownsampleFast(...)
//
// Performs a MA down sampling filter on a vector
//
// Input:
//      - in_vector         : Input samples (state in positions
//                            in_vector[-order] .. in_vector[-1])
//      - in_vector_length  : Number of samples in |in_vector| to be filtered.
//                            This must be at least
//                            |delay| + |factor|*(|out_vector_length|-1) + 1)
//      - out_vector_length : Number of down sampled samples desired
//      - ma_coef           : Filter coefficients (in Q12)
//      - ma_coef_length    : Number of B coefficients (order+1)
//      - factor            : Decimation factor
//      - delay             : Delay of filter (compensated for in out_vector)
//
// Output:
//      - out_vector        : Filtered samples
//
// Return value             : 0 if OK, -1 if |in_vector| is too short
//

//
// Spl_DotProductWithScale(...)
//
// Calculates the dot product between two (Word16) vectors
//
// Input:
//      - vector1       : Vector 1
//      - vector2       : Vector 2
//      - vector_length : Number of samples used in the dot product
//      - scaling       : The number of right bit shifts to apply on each term
//                        during calculation to avoid overflow, i.e., the
//                        output will be in Q(-|scaling|)
//
// Return value         : The dot product in Q(-scaling)
//

//
// ANS_YL_Spl_ComplexIFFT(...)
//
// Complex Inverse FFT
//
// Computes an inverse complex 2^|stages|-point FFT on the input vector, which
// is in bit-reversed order. The original content of the vector is destroyed in
// the process, since the input is overwritten by the output, normal-ordered,
// FFT vector. With X as the input complex vector, y as the output complex
// vector and with M = 2^|stages|, the following is computed:
//
//        M-1
// y(k) = sum[X(i)*[cos(2*pi*i*k/M) + j*sin(2*pi*i*k/M)]]
//        i=0
//
// The implementations are optimized for speed, not for code size. It uses the
// decimation-in-time algorithm with radix-2 butterfly technique.
//
// Input:
//      - vector    : In pointer to complex vector containing 2^|stages|
//                    real elements interleaved with 2^|stages| imaginary
//                    elements.
//                    [ReImReImReIm....]
//                    The elements are in Q(-scale) domain, see more on Return
//                    Value below.
//
//      - stages    : Number of FFT stages. Must be at least 3 and at most 10,
//                    since the table Spl_kSinTable1024[] is 1024
//                    elements long.
//
//      - mode      : This parameter gives the user to choose how the FFT
//                    should work.
//                    mode==0: Low-complexity and Low-accuracy mode
//                    mode==1: High-complexity and High-accuracy mode
//
// Output:
//      - vector    : Out pointer to the FFT vector (the same as input).
//
// Return Value     : The scale value that tells the number of left bit shifts
//                    that the elements in the |vector| should be shifted with
//                    in order to get Q0 values, i.e. the physically correct
//                    values. The scale parameter is always 0 or positive,
//                    except if N>1024 (|stages|>10), which returns a scale
//                    value of -1, indicating error.
//

//
// ANS_YL_Spl_ComplexFFT(...)
//
// Complex FFT
//
// Computes a complex 2^|stages|-point FFT on the input vector, which is in
// bit-reversed order. The original content of the vector is destroyed in
// the process, since the input is overwritten by the output, normal-ordered,
// FFT vector. With x as the input complex vector, Y as the output complex
// vector and with M = 2^|stages|, the following is computed:
//
//              M-1
// Y(k) = 1/M * sum[x(i)*[cos(2*pi*i*k/M) + j*sin(2*pi*i*k/M)]]
//              i=0
//
// The implementations are optimized for speed, not for code size. It uses the
// decimation-in-time algorithm with radix-2 butterfly technique.
//
// This routine prevents overflow by scaling by 2 before each FFT stage. This is
// a fixed scaling, for proper normalization - there will be log2(n) passes, so
// this results in an overall factor of 1/n, distributed to maximize arithmetic
// accuracy.
//
// Input:
//      - vector    : In pointer to complex vector containing 2^|stages| real
//                    elements interleaved with 2^|stages| imaginary elements.
//                    [ReImReImReIm....]
//                    The output is in the Q0 domain.
//
//      - stages    : Number of FFT stages. Must be at least 3 and at most 10,
//                    since the table Spl_kSinTable1024[] is 1024
//                    elements long.
//
//      - mode      : This parameter gives the user to choose how the FFT
//                    should work.
//                    mode==0: Low-complexity and Low-accuracy mode
//                    mode==1: High-complexity and High-accuracy mode
//
// Output:
//      - vector    : The output FFT vector is in the Q0 domain.
//
// Return value     : The scale parameter is always 0, except if N>1024,
//                    which returns a scale value of -1, indicating error.
//

//
// ANS_YL_Spl_ComplexBitReverse(...)
//
// Complex Bit Reverse
//
// This function bit-reverses the position of elements in the complex input
// vector into the output vector.
//
// If you bit-reverse a linear-order array, you obtain a bit-reversed order
// array. If you bit-reverse a bit-reversed order array, you obtain a
// linear-order array.
//
// Input:
//      - vector    : In pointer to complex vector containing 2^|stages| real
//                    elements interleaved with 2^|stages| imaginary elements.
//                    [ReImReImReIm....]
//      - stages    : Number of FFT stages. Must be at least 3 and at most 10,
//                    since the table Spl_kSinTable1024[] is 1024
//                    elements long.
//
// Output:
//      - vector    : Out pointer to complex vector in bit-reversed order.
//                    The input vector is over written.
//

//
// Spl_AnalysisQMF(...)
//
// Splits a 0-2*F Hz signal into two sub bands: 0-F Hz and F-2*F Hz. The
// current version has F = 8000, therefore, a super-wideband audio signal is
// split to lower-band 0-8 kHz and upper-band 8-16 kHz.
//
// Input:
//      - in_data       : Wide band speech signal, 320 samples (10 ms)
//
// Input & Output:
//      - filter_state1 : Filter state for first All-pass filter
//      - filter_state2 : Filter state for second All-pass filter
//
// Output:
//      - low_band      : Lower-band signal 0-8 kHz band, 160 samples (10 ms)
//      - high_band     : Upper-band signal 8-16 kHz band (flipped in frequency
//                        domain), 160 samples (10 ms)
//

//
// Spl_SynthesisQMF(...)
//
// Combines the two sub bands (0-F and F-2*F Hz) into a signal of 0-2*F
// Hz, (current version has F = 8000 Hz). So the filter combines lower-band
// (0-8 kHz) and upper-band (8-16 kHz) channels to obtain super-wideband 0-16
// kHz audio.
//
// Input:
//      - low_band      : The signal with the 0-8 kHz band, 160 samples (10 ms)
//      - high_band     : The signal with the 8-16 kHz band, 160 samples (10 ms)
//
// Input & Output:
//      - filter_state1 : Filter state for first All-pass filter
//      - filter_state2 : Filter state for second All-pass filter
//
// Output:
//      - out_data      : Super-wideband speech signal, 0-16 kHz
//

// Word16 Spl_SatW32ToW16(...)
//
// This function saturates a 32-bit word into a 16-bit word.
// 
// Input:
//      - value32   : The value of a 32-bit word.
//
// Output:
//      - out16     : the saturated 16-bit word.
//

// Word32 MulAccumW16(...)
//
// This function multiply a 16-bit word by a 16-bit word, and accumulate this
// value to a 32-bit integer.
// 
// Input:
//      - a    : The value of the first 16-bit word.
//      - b    : The value of the second 16-bit word.
//      - c    : The value of an 32-bit integer.
//
// Return Value: The value of a * b + c.
//

// Word16 Spl_get_version(...)
//
// This function gives the version string of the Signal Processing Library.
//
// Input:
//      - length_in_bytes   : The size of Allocated space (in Bytes) where
//                            the version number is written to (in string format).
//
// Output:
//      - version           : Pointer to a buffer where the version number is written to.
//
