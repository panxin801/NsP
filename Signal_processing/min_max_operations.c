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
 * ANS_YL_Spl_MaxAbsValueW16()
 * ANS_YL_Spl_MaxAbsIndexW16()
 * ANS_YL_Spl_MaxAbsValueW32()
 * ANS_YL_Spl_MaxValueW16()
 * ANS_YL_Spl_MaxIndexW16()
 * ANS_YL_Spl_MaxValueW32()
 * ANS_YL_Spl_MaxIndexW32()
 * ANS_YL_Spl_MinValueW16()
 * ANS_YL_Spl_MinIndexW16()
 * ANS_YL_Spl_MinValueW32()
 * ANS_YL_Spl_MinIndexW32()
 *
 * The description header can be found in signal_processing_library.h.
 *
 */

#include "signal_processing_library.h"

#if !(defined(ANDROID) && defined(ARCH_ARM_NEON))

// Maximum absolute value of word16 vector.
Word16 ANS_YL_Spl_MaxAbsValueW16(const Word16 *vector, Word16 length)
{
    Word32 tempMax = 0;
    Word32 absVal;
    Word16 totMax;
    int i;
    G_CONST Word16 *tmpvector = vector;

    for (i = 0; i < length; i++)
    {
        absVal = SPL_ABS_W32((*tmpvector));
        if (absVal > tempMax)
        {
            tempMax = absVal;
        }
        tmpvector++;
    }
    totMax = (Word16)SPL_MIN(tempMax, SPL_WORD16_MAX);
    return totMax;
}

#endif

// Index of maximum absolute value in a  word16 vector.
Word16 ANS_YL_Spl_MaxAbsIndexW16(G_CONST Word16* vector, Word16 length)
{
    Word16 tempMax;
    Word16 absTemp;
    Word16 tempMaxIndex = 0;
    Word16 i = 0;
    G_CONST Word16 *tmpvector = vector;

    tempMax = SPL_ABS_W16(*tmpvector);
    tmpvector++;
    for (i = 1; i < length; i++)
    {
        absTemp = SPL_ABS_W16(*tmpvector);
        tmpvector++;
        if (absTemp > tempMax)
        {
            tempMax = absTemp;
            tempMaxIndex = i;
        }
    }
    return tempMaxIndex;
}

// Maximum absolute value of word32 vector.
Word32 ANS_YL_Spl_MaxAbsValueW32(G_CONST Word32 *vector, Word16 length)
{
    UWord32 tempMax = 0;
    UWord32 absVal;
    Word32 retval;
    int i;
    G_CONST Word32 *tmpvector = vector;

    for (i = 0; i < length; i++)
    {
        absVal = SPL_ABS_W32((*tmpvector));
        if (absVal > tempMax)
        {
            tempMax = absVal;
        }
        tmpvector++;
    }
    retval = (Word32)(SPL_MIN(tempMax, SPL_WORD32_MAX));
    return retval;
}

// Maximum value of word16 vector.
#ifndef XSCALE_OPT
Word16 ANS_YL_Spl_MaxValueW16(G_CONST Word16* vector, Word16 length)
{
    Word16 tempMax;
    Word16 i;
    G_CONST Word16 *tmpvector = vector;

    tempMax = *tmpvector++;
    for (i = 1; i < length; i++)
    {
        if (*tmpvector++ > tempMax)
            tempMax = vector[i];
    }
    return tempMax;
}
#else
#pragma message(">> ANS_YL_Spl_MaxValueW16 is excluded from this build")
#endif

// Index of maximum value in a word16 vector.
Word16 ANS_YL_Spl_MaxIndexW16(G_CONST Word16 *vector, Word16 length)
{
    Word16 tempMax;
    Word16 tempMaxIndex = 0;
    Word16 i = 0;
    G_CONST Word16 *tmpvector = vector;

    tempMax = *tmpvector++;
    for (i = 1; i < length; i++)
    {
        if (*tmpvector++ > tempMax)
        {
            tempMax = vector[i];
            tempMaxIndex = i;
        }
    }
    return tempMaxIndex;
}

// Maximum value of word32 vector.
#ifndef XSCALE_OPT
Word32 ANS_YL_Spl_MaxValueW32(G_CONST Word32* vector, Word16 length)
{
    Word32 tempMax;
    Word16 i;
    G_CONST Word32 *tmpvector = vector;

    tempMax = *tmpvector++;
    for (i = 1; i < length; i++)
    {
        if (*tmpvector++ > tempMax)
            tempMax = vector[i];
    }
    return tempMax;
}
#else
#pragma message(">> ANS_YL_Spl_MaxValueW32 is excluded from this build")
#endif

// Index of maximum value in a word32 vector.
Word16 ANS_YL_Spl_MaxIndexW32(G_CONST Word32* vector, Word16 length)
{
    Word32 tempMax;
    Word16 tempMaxIndex = 0;
    Word16 i = 0;
    G_CONST Word32 *tmpvector = vector;

    tempMax = *tmpvector++;
    for (i = 1; i < length; i++)
    {
        if (*tmpvector++ > tempMax)
        {
            tempMax = vector[i];
            tempMaxIndex = i;
        }
    }
    return tempMaxIndex;
}

// Minimum value of word16 vector.
Word16 ANS_YL_Spl_MinValueW16(G_CONST Word16 *vector, Word16 length)
{
    Word16 tempMin;
    Word16 i;
    G_CONST Word16 *tmpvector = vector;

    // Find the minimum value
    tempMin = *tmpvector++;
    for (i = 1; i < length; i++)
    {
        if (*tmpvector++ < tempMin)
            tempMin = (vector[i]);
    }
    return tempMin;
}

// Index of minimum value in a word16 vector.
#ifndef XSCALE_OPT
Word16 ANS_YL_Spl_MinIndexW16(G_CONST Word16* vector, Word16 length)
{
    Word16 tempMin;
    Word16 tempMinIndex = 0;
    Word16 i = 0;
    G_CONST Word16* tmpvector = vector;

    // Find index of smallest value
    tempMin = *tmpvector++;
    for (i = 1; i < length; i++)
    {
        if (*tmpvector++ < tempMin)
        {
            tempMin = vector[i];
            tempMinIndex = i;
        }
    }
    return tempMinIndex;
}
#else
#pragma message(">> ANS_YL_Spl_MinIndexW16 is excluded from this build")
#endif

// Minimum value of word32 vector.
Word32 ANS_YL_Spl_MinValueW32(G_CONST Word32 *vector, Word16 length)
{
    Word32 tempMin;
    Word16 i;
    G_CONST Word32 *tmpvector = vector;

    // Find the minimum value
    tempMin = *tmpvector++;
    for (i = 1; i < length; i++)
    {
        if (*tmpvector++ < tempMin)
            tempMin = (vector[i]);
    }
    return tempMin;
}

// Index of minimum value in a word32 vector.
#ifndef XSCALE_OPT
Word16 ANS_YL_Spl_MinIndexW32(G_CONST Word32* vector, Word16 length)
{
    Word32 tempMin;
    Word16 tempMinIndex = 0;
    Word16 i = 0;
    G_CONST Word32 *tmpvector = vector;

    // Find index of smallest value
    tempMin = *tmpvector++;
    for (i = 1; i < length; i++)
    {
        if (*tmpvector++ < tempMin)
        {
            tempMin = vector[i];
            tempMinIndex = i;
        }
    }
    return tempMinIndex;
}
#else
#pragma message(">> ANS_YL_Spl_MinIndexW32 is excluded from this build")
#endif
