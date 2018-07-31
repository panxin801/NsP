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
 * This file contains implementations of the divisions
 * ANS_YL_Spl_DivU32U16()
 * ANS_YL_Spl_DivW32W16()
 * ANS_YL_Spl_DivW32W16ResW16()
 * ANS_YL_Spl_DivResultInQ31()
 * ANS_YL_Spl_DivW32HiLow()
 *
 * The description header can be found in signal_processing_library.h
 *
 */

#include "signal_processing_library.h"

UWord32 ANS_YL_Spl_DivU32U16(UWord32 num, UWord16 den)
{
    // Guard against division with 0
    if (den != 0)
    {
        return (UWord32)(num / den);
    } else
    {
        return (UWord32)0xFFFFFFFF;
    }
}

Word32 ANS_YL_Spl_DivW32W16(Word32 num, Word16 den)
{
    // Guard against division with 0
    if (den != 0)
    {
        return (Word32)(num / den);
    } else
    {
        return (Word32)0x7FFFFFFF;
    }
}

Word16 ANS_YL_Spl_DivW32W16ResW16(Word32 num, Word16 den)
{
    // Guard against division with 0
    if (den != 0)
    {
        return (Word16)(num / den);
    } else
    {
        return (Word16)0x7FFF;
    }
}

Word32 ANS_YL_Spl_DivResultInQ31(Word32 num, Word32 den)
{
    Word32 L_num = num;
    Word32 L_den = den;
    Word32 div = 0;
    int k = 31;
    int change_sign = 0;

    if (num == 0)
        return 0;

    if (num < 0)
    {
        change_sign++;
        L_num = -num;
    }
    if (den < 0)
    {
        change_sign++;
        L_den = -den;
    }
    while (k--)
    {
        div <<= 1;
        L_num <<= 1;
        if (L_num >= L_den)
        {
            L_num -= L_den;
            div++;
        }
    }
    if (change_sign == 1)
    {
        div = -div;
    }
    return div;
}

Word32 ANS_YL_Spl_DivW32HiLow(Word32 num, Word16 den_hi,
                                    Word16 den_low)
{
    Word16 approx, tmp_hi, tmp_low, num_hi, num_low;
    Word32 tmpW32;

    approx = (Word16)ANS_YL_Spl_DivW32W16((Word32)0x1FFFFFFF, den_hi);
    // result in Q14 (Note: 3FFFFFFF = 0.5 in Q30)

    // tmpW32 = 1/den = approx * (2.0 - den * approx) (in Q30)
    tmpW32 = (SPL_MUL_16_16(den_hi, approx) << 1)
            + ((SPL_MUL_16_16(den_low, approx) >> 15) << 1);
    // tmpW32 = den * approx

    tmpW32 = (Word32)0x7fffffffL - tmpW32; // result in Q30 (tmpW32 = 2.0-(den*approx))

    // Store tmpW32 in hi and low format
    tmp_hi = (Word16)SPL_RSHIFT_W32(tmpW32, 16);
    tmp_low = (Word16)SPL_RSHIFT_W32((tmpW32
            - SPL_LSHIFT_W32((Word32)tmp_hi, 16)), 1);

    // tmpW32 = 1/den in Q29
    tmpW32 = ((SPL_MUL_16_16(tmp_hi, approx) + (SPL_MUL_16_16(tmp_low, approx)
            >> 15)) << 1);

    // 1/den in hi and low format
    tmp_hi = (Word16)SPL_RSHIFT_W32(tmpW32, 16);
    tmp_low = (Word16)SPL_RSHIFT_W32((tmpW32
            - SPL_LSHIFT_W32((Word32)tmp_hi, 16)), 1);

    // Store num in hi and low format
    num_hi = (Word16)SPL_RSHIFT_W32(num, 16);
    num_low = (Word16)SPL_RSHIFT_W32((num
            - SPL_LSHIFT_W32((Word32)num_hi, 16)), 1);

    // num * (1/den) by 32 bit multiplication (result in Q28)

    tmpW32 = (SPL_MUL_16_16(num_hi, tmp_hi) + (SPL_MUL_16_16(num_hi, tmp_low)
            >> 15) + (SPL_MUL_16_16(num_low, tmp_hi) >> 15));

    // Put result in Q31 (convert from Q28)
    tmpW32 = SPL_LSHIFT_W32(tmpW32, 3);

    return tmpW32;
}
