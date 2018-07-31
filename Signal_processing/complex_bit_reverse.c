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
 * This file contains the function ANS_YL_ANS_YL_Spl_ComplexBitReverse().
 * The description header can be found in signal_processing_library.h
 *
 */

#include "signal_processing_library.h"

void ANS_YL_Spl_ComplexBitReverse(Word16 frfi[], int stages)
{
    int mr, nn, n, l, m;
    Word16 tr, ti;

    n = 1 << stages;

    mr = 0;
    nn = n - 1;

    // decimation in time - re-order data
    for (m = 1; m <= nn; ++m)
    {
        l = n;
        do
        {
            l >>= 1;
        } while (mr + l > nn);
        mr = (mr & (l - 1)) + l;

        if (mr <= m)
            continue;

        tr = frfi[2 * m];
        frfi[2 * m] = frfi[2 * mr];
        frfi[2 * mr] = tr;

        ti = frfi[2 * m + 1];
        frfi[2 * m + 1] = frfi[2 * mr + 1];
        frfi[2 * mr + 1] = ti;
    }
}
