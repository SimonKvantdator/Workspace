/* This file is here to prevent a file conflict on multiarch systems.  A
 * conflict will occur because NTL/gmp_aux has arch-specific definitions.
 *
 * DO NOT INCLUDE THE NEW FILE DIRECTLY -- ALWAYS INCLUDE THIS ONE INSTEAD. */

#ifndef NTL_gmp_aux_MULTILIB_H
#define NTL_gmp_aux_MULTILIB_H
#include <bits/wordsize.h>

#if __WORDSIZE == 32
#include "NTL/gmp_aux-32.h"
#elif __WORDSIZE == 64
#include "NTL/gmp_aux-64.h"
#else
#error "unexpected value for __WORDSIZE macro"
#endif

#endif
