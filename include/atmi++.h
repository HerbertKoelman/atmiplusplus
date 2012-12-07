/* include/atmi++.h.  Generated from atmi++.h.in by configure.  */
/*
  $Id$

  Helper class for dealing with tuxedo.
*/

/*
 * ATMI++
 *
 * Copyright (C) 2006 - herbert koelman
 *
 * Tuxedo is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Logging Tuxedo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Tuxedo; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

/** @mainpage C++ implementation for ATMI v2.1 - Tuxedo 9.1
 *
 * @author herbert koelman, eric belloni
 *
 * @section what_it_does What it does
 *
 * simple C++ wrappers for the ATMI C library, making it easier to write fast and rock solid Tuxedo software. The targeted \
 * implementation should be thread safe and offer multi-context capabilities.
 *
 * @section what_it_implements What it implements
 *
 * The ATMI++ comes as the following list of libraries:
 *  - atmi++.a : set of classes to handle ATMI calls in C++ programs
 *     - atmi::Tuxedo class : base class offers access to common function (like memory allocation).
 *     - atmi::Tp class: extends Tuxedo class with calls dedicated to transaction processing
 *     - atmi::Queue class : extends Tuxedo class with calls dedicated to queuing operations.
 *
 *  - fml++.a : set of classes to handle Fielded Buffers (FML) calls in C++ programs
 *     - atmi::Buffer class : Handle field buffer memory.
 *     - atmi::Field class: base class to manipulate Fields. This is an virtual pure class.
 *     - atmi::template < >TField class templates : 
 *
 *  - atmiutl++.a : set of helper classes to handle concurrency and multithreading
 *     - Thread class: wrapper of the POSIX Threading library
 *     - Mutex class: wrapper to POSIX Mutex handling functions.
 *     - Condition class: wrapper to POSIX Condition handling functions.
 *
 * The library comes with sample code that are gathered into the bin directory :
 * - btest.C: FML buffer sample
 * - qtest.C: /Q access demo
 * - tptest.C: transactional calls samples
 * - mctest.C: multicontext sample code
 */

/* set through configure.ac by autoheader.  */
#define ATMI_VERSION "v2.1 compiled on Linux #1 SMP Wed Nov 23 13:07:52 UTC 2011.2.6.35.14-106.fc14.x86_64 for tuxedo11gR1"

/* Define to 1 if you have the `gettimeofday' function. */
/* #undef HAVE_GETTIMEOFDAY */

/* Define to 1 if you have the <inttypes.h> header file. */
/* #undef HAVE_INTTYPES_H */

/* Define to 1 if you have the `pthread' library (-lpthread). */
/* #undef HAVE_LIBPTHREAD */

/* Define to 1 if you have the `tux' library (-ltux). */
/* #undef HAVE_LIBTUX */

/* Define to 1 if you have the <memory.h> header file. */
/* #undef HAVE_MEMORY_H */

/* Define to 1 if you have the <stdint.h> header file. */
/* #undef HAVE_STDINT_H */

/* Define to 1 if you have the <stdlib.h> header file. */
/* #undef HAVE_STDLIB_H */

/* Define to 1 if you have the <strings.h> header file. */
/* #undef HAVE_STRINGS_H */

/* Define to 1 if you have the <string.h> header file. */
/* #undef HAVE_STRING_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
/* #undef HAVE_SYS_STAT_H */

/* Define to 1 if you have the <sys/time.h> header file. */
/* #undef HAVE_SYS_TIME_H */

/* Define to 1 if you have the <sys/types.h> header file. */
/* #undef HAVE_SYS_TYPES_H */

/* Define to 1 if you have the <unistd.h> header file. */
/* #undef HAVE_UNISTD_H */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "herbert.koelman@me.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "atmi++"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "atmi++ 2.1"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "http://redmine.urbix-software.fr/projects/atmiplusplus"

/* Define to the version of this package. */
#define PACKAGE_VERSION "2.1"

/* Define as the return type of signal handlers (`int' or `void'). */
/* #undef RETSIGTYPE */

/* Define to 1 if you have the ANSI C header files. */
/* #undef STDC_HEADERS */

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
/* #undef TIME_WITH_SYS_TIME */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

#ifndef _ATMI_PLUS_PLUS_H_
#define _ATMI_PLUS_PLUS_H_

/* #include <config.h> */
#include <Exceptions.h>
#include <Tuxedo.h>
#include <Buffers.h>

#endif

