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
 * This file contains the function ANS_YL_Spl_GetScalingSquare().
 * The description header can be found in signal_processing_library.h
 *
 */

#include "signal_processing_library.h"

int ANS_YL_Spl_GetScalingSquare(Word16 *in_vector, int in_vector_length, int times)
{
    int nbits = Spl_GetSizeInBits(times);
    int i;
    Word16 smax = -1;
    Word16 sabs;
    Word16 *sptr = in_vector;
    int t;
    int looptimes = in_vector_length;

    for (i = looptimes; i > 0; i--)
    {
        sabs = (*sptr > 0 ? *sptr++ : -*sptr++);
        smax = (sabs > smax ? sabs : smax);
    }
    t = Spl_NormW32(SPL_MUL(smax, smax));

    if (smax == 0)
    {
        return 0; // Since norm(0) returns 0
    } else
    {
        return (t > nbits) ? 0 : nbits - t;
    }
}
