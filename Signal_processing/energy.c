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
 * This file contains the function ANS_YL_Spl_Energy().
 * The description header can be found in signal_processing_library.h
 *
 */

#include "signal_processing_library.h"

Word32 ANS_YL_Spl_Energy(Word16* vector, int vector_length, int* scale_factor)
{
    Word32 en = 0;
    int i;
    int scaling = ANS_YL_Spl_GetScalingSquare(vector, vector_length, vector_length);
    int looptimes = vector_length;
    Word16 *vectorptr = vector;

    for (i = 0; i < looptimes; i++)
    {
        en += SPL_MUL_16_16_RSFT(*vectorptr, *vectorptr, scaling);
        vectorptr++;
    }
    *scale_factor = scaling;

    return en;
}
