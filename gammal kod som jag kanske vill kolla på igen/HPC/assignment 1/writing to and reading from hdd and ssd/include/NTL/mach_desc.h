/* This file is here to prevent a file conflict on multiarch systems.  A
 * conflict will occur because NTL/mach_desc has arch-specific definitions.
 *
 * DO NOT INCLUDE THE NEW FILE DIRECTLY -- ALWAYS INCLUDE THIS ONE INSTEAD. */

#ifndef NTL_mach_desc_MULTILIB_H
#define NTL_mach_desc_MULTILIB_H
#include <bits/wordsize.h>

#if __WORDSIZE == 32
#include "NTL/mach_desc-32.h"
#elif __WORDSIZE == 64
#include "NTL/mach_desc-64.h"
#else
#error "unexpected value for __WORDSIZE macro"
#endif

#endif
