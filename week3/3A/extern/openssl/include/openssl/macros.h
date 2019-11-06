/*
 * Copyright 2019 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <openssl/opensslconf.h>

#ifndef OPENSSL_MACROS_H
# define OPENSSL_MACROS_H

/*
 * Sometimes OPENSSSL_NO_xxx ends up with an empty file and some compilers
 * don't like that.  This will hopefully silence them.
 */
# define NON_EMPTY_TRANSLATION_UNIT static void *dummy = &dummy;

/*
 * Applications should use -DOPENSSL_API_COMPAT=<version> to suppress the
 * declarations of functions deprecated in or before <version>.  If this is
 * undefined, the value of the macro OPENSSL_API_MIN above is the default.
 *
 * For any version number up until version 1.1.x, <version> is expected to be
 * the calculated version number 0xMNNFFPPSL.  For version numbers 3.0.0 and
 * on, <version> is expected to be only the major version number (i.e. 3 for
 * version 3.0.0).
 */
# ifndef DECLARE_DEPRECATED
#  define DECLARE_DEPRECATED(f)   f;
#  ifdef __GNUC__
#   if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 0)
#    undef DECLARE_DEPRECATED
#    define DECLARE_DEPRECATED(f)    f __attribute__ ((deprecated));
#   endif
#  elif defined(__SUNPRO_C)
#   if (__SUNPRO_C >= 0x5130)
#    undef DECLARE_DEPRECATED
#    define DECLARE_DEPRECATED(f)    f __attribute__ ((deprecated));
#   endif
#  endif
# endif

/*
 * We convert the OPENSSL_API_COMPAT value to an API level.  The API level
 * is the major version number for 3.0.0 and on.  For earlier versions, it
 * uses this scheme, which is close enough for our purposes:
 *
 *      0.x.y   0       (0.9.8 was the last release in this series)
 *      1.0.x   1       (1.0.2 was the last release in this series)
 *      1.1.x   2       (1.1.1 was the last release in this series)
 */

/* In case someone defined both */
# if defined(OPENSSL_API_COMPAT) && defined(OPENSSL_API_LEVEL)
#  error "Disallowed to define both OPENSSL_API_COMPAT and OPENSSL_API_LEVEL"
# endif

# ifndef OPENSSL_API_COMPAT
#  define OPENSSL_API_LEVEL OPENSSL_MIN_API
# else
#  if (OPENSSL_API_COMPAT < 0x1000L) /* Major version numbers up to 16777215 */
#   define OPENSSL_API_LEVEL OPENSSL_API_COMPAT
#  elif (OPENSSL_API_COMPAT & 0xF0000000L) == 0x00000000L
#   define OPENSSL_API_LEVEL 0
#  elif (OPENSSL_API_COMPAT & 0xFFF00000L) == 0x10000000L
#   define OPENSSL_API_LEVEL 1
#  elif (OPENSSL_API_COMPAT & 0xFFF00000L) == 0x10100000L
#   define OPENSSL_API_LEVEL 2
#  else
    /* Major number 3 to 15 */
#   define OPENSSL_API_LEVEL ((OPENSSL_API_COMPAT >> 28) & 0xF)
#  endif
# endif

/*
 * Define API level check macros up to what makes sense.  Since we
 * do future deprecations, we define one API level beyond the current
 * major version number.
 */

# if OPENSSL_API_LEVEL < 4
#  define DEPRECATEDIN_4(f)       DECLARE_DEPRECATED(f)
#  define OPENSSL_API_4 0
# else
#  define DEPRECATEDIN_4(f)
#  define OPENSSL_API_4 1
# endif

# if OPENSSL_API_LEVEL < 3
#  define DEPRECATEDIN_3(f)       DECLARE_DEPRECATED(f)
#  define OPENSSL_API_3 0
# else
#  define DEPRECATEDIN_3(f)
#  define OPENSSL_API_3 1
# endif

# if OPENSSL_API_LEVEL < 2
#  define DEPRECATEDIN_1_1_0(f)   DECLARE_DEPRECATED(f)
#  define OPENSSL_API_1_1_0 0
# else
#  define DEPRECATEDIN_1_1_0(f)
#  define OPENSSL_API_1_1_0 1
# endif

# if OPENSSL_API_LEVEL < 1
#  define DEPRECATEDIN_1_0_0(f)   DECLARE_DEPRECATED(f)
#  define OPENSSL_API_1_0_0 0
# else
#  define DEPRECATEDIN_1_0_0(f)
#  define OPENSSL_API_1_0_0 1
# endif

# if OPENSSL_API_LEVEL < 0
#  define DEPRECATEDIN_0_9_8(f)   DECLARE_DEPRECATED(f)
#  define OPENSSL_API_0_9_8 0
# else
#  define DEPRECATEDIN_0_9_8(f)
#  define OPENSSL_API_0_9_8 1
# endif

# ifndef OPENSSL_FILE
#  ifdef OPENSSL_NO_FILENAMES
#   define OPENSSL_FILE ""
#   define OPENSSL_LINE 0
#  else
#   define OPENSSL_FILE __FILE__
#   define OPENSSL_LINE __LINE__
#  endif
# endif

/*
 * __func__ was standardized in C99, so for any compiler that claims
 * to implement that language level or newer, we assume we can safely
 * use that symbol.
 *
 * GNU C also provides __FUNCTION__ since version 2, which predates
 * C99.  We can, however, only use this if __STDC_VERSION__ exists,
 * as it's otherwise not allowed according to ISO C standards (C90).
 * (compiling with GNU C's -pedantic tells us so)
 *
 * If none of the above applies, we check if the compiler is MSVC,
 * and use __FUNCTION__ if that's the case.
 */
# ifndef OPENSSL_FUNC
#  if defined(__STDC_VERSION__)
#   if __STDC_VERSION__ >= 199901L
#    define OPENSSL_FUNC __func__
#   elif defined(__GNUC__) && __GNUC__ >= 2
#    define OPENSSL_FUNC __FUNCTION__
#   endif
#  elif defined(_MSC_VER)
#    define OPENSSL_FUNC __FUNCTION__
#  endif
/*
 * If all these possibilities are exhausted, we give up and use a
 * static string.
 */
#  ifndef OPENSSL_FUNC
#   define OPENSSL_FUNC "(unknown function)"
#  endif
# endif

#endif  /* OPENSSL_MACROS_H */
