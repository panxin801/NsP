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
 * This file contains the implementation of functions
 * ANS_YL_Spl_MemSetW16()
 * ANS_YL_Spl_MemSetW32()
 * ANS_YL_Spl_MemCpyReversedOrder()
 * ANS_YL_Spl_CopyFromEndW16()
 * ANS_YL_Spl_ZerosArrayW16()
 * ANS_YL_Spl_ZerosArrayW32()
 * ANS_YL_Spl_OnesArrayW16()
 * ANS_YL_Spl_OnesArrayW32()
 *
 * The description header can be found in signal_processing_library.h
 *
 */

#include <string.h>
#include "signal_processing_library.h"


void ANS_YL_Spl_MemSetW16(Word16 *ptr, Word16 set_value, int length)
{
    int j;
    Word16 *arrptr = ptr;

    for (j = length; j > 0; j--)
    {
        *arrptr++ = set_value;
    }
}

void ANS_YL_Spl_MemSetW32(Word32 *ptr, Word32 set_value, int length)
{
    int j;
    Word32 *arrptr = ptr;

    for (j = length; j > 0; j--)
    {
        *arrptr++ = set_value;
    }
}

void ANS_YL_Spl_MemCpyReversedOrder(Word16* dest, Word16* source, int length)
{
    int j;
    Word16* destPtr = dest;
    Word16* sourcePtr = source;

    for (j = 0; j < length; j++)
    {
        *destPtr-- = *sourcePtr++;
    }
}

Word16 ANS_YL_Spl_CopyFromEndW16(G_CONST Word16 *vector_in,
                                       Word16 length,
                                       Word16 samples,
                                       Word16 *vector_out)
{
    // Copy the last <samples> of the input vector to vector_out
    SPL_MEMCPY_W16(vector_out, &vector_in[length - samples], samples);

    return samples;
}

Word16 ANS_YL_Spl_ZerosArrayW16(Word16 *vector, Word16 length)
{
    ANS_YL_Spl_MemSetW16(vector, 0, length);
    return length;
}

Word16 ANS_YL_Spl_ZerosArrayW32(Word32 *vector, Word16 length)
{
    ANS_YL_Spl_MemSetW32(vector, 0, length);
    return length;
}

Word16 ANS_YL_Spl_OnesArrayW16(Word16 *vector, Word16 length)
{
    Word16 i;
    Word16 *tmpvec = vector;
    for (i = 0; i < length; i++)
    {
        *tmpvec++ = 1;
    }
    return length;
}

Word16 ANS_YL_Spl_OnesArrayW32(Word32 *vector, Word16 length)
{
    Word16 i;
    Word32 *tmpvec = vector;
    for (i = 0; i < length; i++)
    {
        *tmpvec++ = 1;
    }
    return length;
}
