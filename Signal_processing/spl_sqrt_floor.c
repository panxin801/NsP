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
 * This file contains the function ANS_YL_Spl_SqrtFloor().
 * The description header can be found in signal_processing_library.h
 *
 */

#include "signal_processing_library.h"

#define SPL_SQRT_ITER(N)                 \
  try1 = root + (1 << (N));                     \
  if (value >= try1 << (N))                     \
  {                                             \
    value -= try1 << (N);                       \
    root |= 2 << (N);                           \
  }

// (out) Square root of input parameter
Word32 ANS_YL_Spl_SqrtFloor(Word32 value)
{
    // new routine for performance, 4 cycles/bit in ARM
    // output precision is 16 bits

    Word32 root = 0, try1;

    SPL_SQRT_ITER (15);
    SPL_SQRT_ITER (14);
    SPL_SQRT_ITER (13);
    SPL_SQRT_ITER (12);
    SPL_SQRT_ITER (11);
    SPL_SQRT_ITER (10);
    SPL_SQRT_ITER ( 9);
    SPL_SQRT_ITER ( 8);
    SPL_SQRT_ITER ( 7);
    SPL_SQRT_ITER ( 6);
    SPL_SQRT_ITER ( 5);
    SPL_SQRT_ITER ( 4);
    SPL_SQRT_ITER ( 3);
    SPL_SQRT_ITER ( 2);
    SPL_SQRT_ITER ( 1);
    SPL_SQRT_ITER ( 0);

    return root >> 1;
}
