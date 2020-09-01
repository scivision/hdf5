/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF5/releases.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * This file contains public declarations for the HDF5 module.
 */
#ifndef _H5public_H
#define _H5public_H

/* Include files for public use... */
/*
 * Since H5pubconf.h is a generated header file, it is messy to try
 * to put a #ifndef _H5pubconf_H ... #endif guard in it.
 * HDF5 has set an internal rule that it is being included here.
 * Source files should NOT include H5pubconf.h directly but include
 * it via H5public.h.  The #ifndef _H5public_H guard above would
 * prevent repeated include.
 */
#include "H5pubconf.h"        /*from configure                             */

/* API Version macro wrapper definitions */
#include "H5version.h"

#ifdef H5_HAVE_FEATURES_H
#include <features.h>           /*for setting POSIX, BSD, etc. compatibility */
#endif
#ifdef H5_HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef H5_STDC_HEADERS
#   include <limits.h>        /*for H5T_NATIVE_CHAR defn in H5Tpublic.h    */
#   include <stdarg.h>      /*for variadic functions in H5VLpublic.h     */
#endif
#ifndef __cplusplus
# ifdef H5_HAVE_STDINT_H
#   include <stdint.h>        /*for C9x types                     */
# endif
#else
# ifdef H5_HAVE_STDINT_H_CXX
#   include <stdint.h>        /*for C9x types    when include from C++         */
# endif
#endif
#ifdef H5_HAVE_INTTYPES_H
#   include <inttypes.h>        /* C99/POSIX.1 header for uint64_t, PRIu64 */
#endif
#ifdef H5_HAVE_STDDEF_H
#   include <stddef.h>
#endif
#ifdef H5_HAVE_PARALLEL
/* Don't link against MPI C++ bindings */
#   define MPICH_SKIP_MPICXX 1
#   define OMPI_SKIP_MPICXX 1
#   include <mpi.h>
#ifndef MPI_FILE_NULL        /*MPIO may be defined in mpi.h already       */
#   include <mpio.h>
#endif
#endif


/* Include the Windows API adapter header early */
#include "H5api_adpt.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Macros for enabling/disabling particular GCC warnings */
/* (see the following web-sites for more info:
 *      http://www.dbp-consulting.com/tutorials/SuppressingGCCWarnings.html
 *      http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html#Diagnostic-Pragmas
 */
/* These pragmas are only implemented usefully in gcc 4.6+ */
#if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406
    #define H5_GCC_DIAG_STR(s) #s
    #define H5_GCC_DIAG_JOINSTR(x,y) H5_GCC_DIAG_STR(x ## y)
    #define H5_GCC_DIAG_DO_PRAGMA(x) _Pragma (#x)
    #define H5_GCC_DIAG_PRAGMA(x) H5_GCC_DIAG_DO_PRAGMA(GCC diagnostic x)

    #define H5_GCC_DIAG_OFF(x) H5_GCC_DIAG_PRAGMA(push) H5_GCC_DIAG_PRAGMA(ignored H5_GCC_DIAG_JOINSTR(-W,x))
    #define H5_GCC_DIAG_ON(x) H5_GCC_DIAG_PRAGMA(pop)
#else
    #define H5_GCC_DIAG_OFF(x)
    #define H5_GCC_DIAG_ON(x)
#endif

/* Version numbers */
#define H5_VERS_MAJOR    1    /* For major interface/format changes           */
#define H5_VERS_MINOR    13    /* For minor interface/format changes           */
#define H5_VERS_RELEASE    0    /* For tweaks, bug-fixes, or development     */
#define H5_VERS_SUBRELEASE ""    /* For pre-releases like snap0       */
                /* Empty string for real releases.           */
#define H5_VERS_INFO    "HDF5 library version: 1.13.0"      /* Full version string */

#define H5check()    H5check_version(H5_VERS_MAJOR,H5_VERS_MINOR,          \
                        H5_VERS_RELEASE)

/* macros for comparing the version */
#define H5_VERSION_GE(Maj,Min,Rel) \
       (((H5_VERS_MAJOR==Maj) && (H5_VERS_MINOR==Min) && (H5_VERS_RELEASE>=Rel)) || \
        ((H5_VERS_MAJOR==Maj) && (H5_VERS_MINOR>Min)) || \
        (H5_VERS_MAJOR>Maj))

#define H5_VERSION_LE(Maj,Min,Rel) \
       (((H5_VERS_MAJOR==Maj) && (H5_VERS_MINOR==Min) && (H5_VERS_RELEASE<=Rel)) || \
        ((H5_VERS_MAJOR==Maj) && (H5_VERS_MINOR<Min)) || \
        (H5_VERS_MAJOR<Maj))

/*
 * Status return values.  Failed integer functions in HDF5 result almost
 * always in a negative value (unsigned failing functions sometimes return
 * zero for failure) while successful return is non-negative (often zero).
 * The negative failure value is most commonly -1, but don't bet on it.  The
 * proper way to detect failure is something like:
 *
 *     if((dset = H5Dopen2(file, name)) < 0)
 *        fprintf(stderr, "unable to open the requested dataset\n");
 */
typedef int herr_t;


/*
 * Boolean type.  Successful return values are zero (false) or positive
 * (true). The typical true value is 1 but don't bet on it.  Boolean
 * functions cannot fail.  Functions that return `htri_t' however return zero
 * (false), positive (true), or negative (failure). The proper way to test
 * for truth from a htri_t function is:
 *
 *     if ((retval = H5Tcommitted(type))>0) {
 *        printf("data type is committed\n");
 *    } else if (!retval) {
 *         printf("data type is not committed\n");
 *    } else {
 *         printf("error determining whether data type is committed\n");
 *    }
 */
#ifdef H5_HAVE_STDBOOL_H
  #include <stdbool.h>
#else /* H5_HAVE_STDBOOL_H */
  #ifndef __cplusplus
    #if defined(H5_SIZEOF_BOOL) && (H5_SIZEOF_BOOL != 0)
      #define bool    _Bool
    #else
      #define bool    unsigned int
    #endif
    #define true    1
    #define false   0
  #endif /* __cplusplus */
#endif /* H5_HAVE_STDBOOL_H */
typedef bool hbool_t;
typedef int htri_t;

/* Define the ssize_t type if it not is defined */
#if H5_SIZEOF_SSIZE_T==0
/* Undefine this size, we will re-define it in one of the sections below */
#undef H5_SIZEOF_SSIZE_T
#if H5_SIZEOF_SIZE_T==H5_SIZEOF_INT
typedef int ssize_t;
#       define H5_SIZEOF_SSIZE_T H5_SIZEOF_INT
#elif H5_SIZEOF_SIZE_T==H5_SIZEOF_LONG
typedef long ssize_t;
#       define H5_SIZEOF_SSIZE_T H5_SIZEOF_LONG
#elif H5_SIZEOF_SIZE_T==H5_SIZEOF_LONG_LONG
typedef long long ssize_t;
#       define H5_SIZEOF_SSIZE_T H5_SIZEOF_LONG_LONG
#else /* Can't find matching type for ssize_t */
#   error "nothing appropriate for ssize_t"
#endif
#endif

/*
 * The sizes of file objects have their own types defined here, use a 64-bit
 * type.
 */
#if H5_SIZEOF_LONG_LONG >= 8
H5_GCC_DIAG_OFF(long-long)
typedef unsigned long long     hsize_t;
typedef signed long long    hssize_t;
H5_GCC_DIAG_ON(long-long)
#       define H5_SIZEOF_HSIZE_T H5_SIZEOF_LONG_LONG
#       define H5_SIZEOF_HSSIZE_T H5_SIZEOF_LONG_LONG
#else
#   error "nothing appropriate for hsize_t"
#endif
#define HSIZE_UNDEF             ((hsize_t)(hssize_t)(-1))

/*
 * File addresses have their own types.
 */
#if H5_SIZEOF_INT >= 8
    typedef unsigned                haddr_t;
#   define HADDR_UNDEF              ((haddr_t)(-1))
#   define H5_SIZEOF_HADDR_T        H5_SIZEOF_INT
#   ifdef H5_HAVE_PARALLEL
#       define HADDR_AS_MPI_TYPE    MPI_UNSIGNED
#   endif  /* H5_HAVE_PARALLEL */
#elif H5_SIZEOF_LONG >= 8
    typedef unsigned long           haddr_t;
#   define HADDR_UNDEF              ((haddr_t)(long)(-1))
#   define H5_SIZEOF_HADDR_T        H5_SIZEOF_LONG
#   ifdef H5_HAVE_PARALLEL
#       define HADDR_AS_MPI_TYPE    MPI_UNSIGNED_LONG
#   endif  /* H5_HAVE_PARALLEL */
#elif H5_SIZEOF_LONG_LONG >= 8
    typedef unsigned long long      haddr_t;
#   define HADDR_UNDEF              ((haddr_t)(long long)(-1))
#   define H5_SIZEOF_HADDR_T        H5_SIZEOF_LONG_LONG
#   ifdef H5_HAVE_PARALLEL
#       define HADDR_AS_MPI_TYPE    MPI_LONG_LONG_INT
#   endif  /* H5_HAVE_PARALLEL */
#else
#   error "nothing appropriate for haddr_t"
#endif
#if H5_SIZEOF_HADDR_T == H5_SIZEOF_INT
#   define H5_PRINTF_HADDR_FMT  "%u"
#elif H5_SIZEOF_HADDR_T == H5_SIZEOF_LONG
#   define H5_PRINTF_HADDR_FMT  "%lu"
#elif H5_SIZEOF_HADDR_T == H5_SIZEOF_LONG_LONG
#   define H5_PRINTF_HADDR_FMT  "%" H5_PRINTF_LL_WIDTH "u"
#else
#   error "nothing appropriate for H5_PRINTF_HADDR_FMT"
#endif
#define HADDR_MAX        (HADDR_UNDEF-1)

/* uint32_t type is used for creation order field for messages.  It may be
 * defined in Posix.1g, otherwise it is defined here.
 */
#if H5_SIZEOF_UINT32_T>=4
#elif H5_SIZEOF_SHORT>=4
    typedef short uint32_t;
#   undef H5_SIZEOF_UINT32_T
#   define H5_SIZEOF_UINT32_T H5_SIZEOF_SHORT
#elif H5_SIZEOF_INT>=4
    typedef unsigned int uint32_t;
#   undef H5_SIZEOF_UINT32_T
#   define H5_SIZEOF_UINT32_T H5_SIZEOF_INT
#elif H5_SIZEOF_LONG>=4
    typedef unsigned long uint32_t;
#   undef H5_SIZEOF_UINT32_T
#   define H5_SIZEOF_UINT32_T H5_SIZEOF_LONG
#else
#   error "nothing appropriate for uint32_t"
#endif

/* int64_t type is used for creation order field for links.  It may be
 * defined in Posix.1g, otherwise it is defined here.
 */
#if H5_SIZEOF_INT64_T>=8
#elif H5_SIZEOF_INT>=8
    typedef int int64_t;
#   undef H5_SIZEOF_INT64_T
#   define H5_SIZEOF_INT64_T H5_SIZEOF_INT
#elif H5_SIZEOF_LONG>=8
    typedef long int64_t;
#   undef H5_SIZEOF_INT64_T
#   define H5_SIZEOF_INT64_T H5_SIZEOF_LONG
#elif H5_SIZEOF_LONG_LONG>=8
    typedef long long int64_t;
#   undef H5_SIZEOF_INT64_T
#   define H5_SIZEOF_INT64_T H5_SIZEOF_LONG_LONG
#else
#   error "nothing appropriate for int64_t"
#endif

/* uint64_t type is used for fields for H5O_info_t.  It may be
 * defined in Posix.1g, otherwise it is defined here.
 */
#if H5_SIZEOF_UINT64_T>=8
#elif H5_SIZEOF_INT>=8
    typedef unsigned uint64_t;
#   undef H5_SIZEOF_UINT64_T
#   define H5_SIZEOF_UINT64_T H5_SIZEOF_INT
#elif H5_SIZEOF_LONG>=8
    typedef unsigned long uint64_t;
#   undef H5_SIZEOF_UINT64_T
#   define H5_SIZEOF_UINT64_T H5_SIZEOF_LONG
#elif H5_SIZEOF_LONG_LONG>=8
    typedef unsigned long long uint64_t;
#   undef H5_SIZEOF_UINT64_T
#   define H5_SIZEOF_UINT64_T H5_SIZEOF_LONG_LONG
#else
#   error "nothing appropriate for uint64_t"
#endif

/** Common iteration orders */
typedef enum {
    H5_ITER_UNKNOWN = -1,      /**< Unknown order */
    H5_ITER_INC,               /**< Increasing order */
    H5_ITER_DEC,               /**< Decreasing order */
    H5_ITER_NATIVE,            /**< No particular order, whatever is fastest */
    H5_ITER_N                  /**< Number of iteration orders */
} H5_iter_order_t;

/* Iteration callback values */
/* (Actually, any positive value will cause the iterator to stop and pass back
 *      that positive value to the function that called the iterator)
 */
#define H5_ITER_ERROR   (-1)
#define H5_ITER_CONT    (0)
#define H5_ITER_STOP    (1)

/**
 * The types of indices on links in groups/attributes on objects.
 * Primarily used for "<do> <foo> by index" routines and for iterating over
 * links in groups/attributes on objects.
 */
typedef enum H5_index_t {
    H5_INDEX_UNKNOWN = -1,    /**< Unknown index type                */
    H5_INDEX_NAME,            /**< Index on names                    */
    H5_INDEX_CRT_ORDER,       /**< Index on creation order           */
    H5_INDEX_N                /**< Number of indices defined         */
} H5_index_t;

/**
 * Storage info struct used by H5O_info_t and H5F_info_t
 */
//! [H5_ih_info_t_snip]
typedef struct H5_ih_info_t {
    hsize_t     index_size;     /**< btree and/or list */
    hsize_t     heap_size;
} H5_ih_info_t;
//! [H5_ih_info_t_snip]

/* Tokens are unique and permanent identifiers that are
 * used to reference HDF5 objects in a container. */

/** The maximum size allowed for tokens */
#define H5O_MAX_TOKEN_SIZE      (16)    /* Allows for 128-bit tokens */

/**
 * Type for object tokens
 * \internal (Hoisted here, since it's used by both the
 *            H5Lpublic.h and H5Opublic.h headers) */
//! [H5O_token_t_snip]
typedef struct H5O_token_t {
    uint8_t __data[H5O_MAX_TOKEN_SIZE];
} H5O_token_t;
//! [H5O_token_t_snip]

/**
 * Allocation statistics info struct
 */
typedef struct H5_alloc_stats_t {
    unsigned long long total_alloc_bytes; /**< Running count of total # of bytes allocated */
    size_t curr_alloc_bytes;           /**< Current # of bytes allocated */
    size_t peak_alloc_bytes;           /**< Peak # of bytes allocated */
    size_t max_block_size;             /**< Largest block allocated */
    size_t total_alloc_blocks_count;   /**< Running count of total # of blocks allocated */
    size_t curr_alloc_blocks_count;    /**< Current # of blocks allocated */
    size_t peak_alloc_blocks_count;    /**< Peak # of blocks allocated */
} H5_alloc_stats_t;

/* Functions in H5.c */
/**
 * \ingroup H5
 * \brief Initializes the HDF5 library
 * \return \herr_t
 *
 * \details H5open() initializes the HDF5 library.
 *
 * \details When the HDF5 library is used in a C application, the library is
 *          automatically initialized when the first HDf5 function call is
 *          issued. If one finds that an HDF5 library function is failing
 *          inexplicably, H5open() can be called first. It is safe to call
 *          H5open() before an application issues any other function calls to
 *          the HDF5 library as there are no damaging side effects in calling
 *          it more than once.
 */
H5_DLL herr_t H5open(void);
/**
 * \ingroup H5
 * \brief Flushes all data to disk, closes all open objects, and releases memory
 * \return \herr_t
 *
 * \details H5close() flushes all data to disk, closes all open HDF5 objects,
 *          and cleans up all memory used by the HDF5 library. This function is
 *          generally called when the application calls exit(), but may be
 *          called earlier in the event of an emergency shutdown or out of a
 *          desire to free all resources used by the HDF5 library.
 */
H5_DLL herr_t H5close(void);
/**
 * \ingroup H5
 * \brief Instructs library not to install atexit() cleanup routine
 * \return \herr_t
 *
 * \details H5dont_atexit() indicates to the library that an atexit() cleanup
 *          routine should not be installed. The major purpose for using this
 *          function is in situations where the library is dynamically linked
 *          into an application and is un-linked from the application before
 *          exit() gets called. In those situations, a routine installed with
 *          atexit() would jump to a routine which was no longer in memory,
 *          causing errors.
 *
 * \attention In order to be effective, this routine \Emph{must} be called
 *            before any other HDF5 function calls, and must be called each
 *            time the library is loaded/linked into the application (the first
 *            time and after it's been un-loaded).
 */
H5_DLL herr_t H5dont_atexit(void);
/**
 * \ingroup H5
 * \brief Garbage collects on all free-lists of all types
 * \return \herr_t
 *
 * \details H5garbage_collect() walks through all garbage collection routines
 *          of the library, freeing any unused memory.
 *
 *          It is not required that H5garbage_collect() be called at any
 *          particular time; it is only necessary in certain situations where
 *          the application has performed actions that cause the library to
 *          allocate many objects. The application should call
 *          H5garbage_collect() if it eventually releases those objects and
 *          wants to reduce the memory used by the library from the peak usage
 *          required.
 *
 * \note The library automatically garbage collects all the free lists when the
 *       application ends.
 */
H5_DLL herr_t H5garbage_collect(void);
H5_DLL herr_t H5set_free_list_limits (int reg_global_lim, int reg_list_lim,
                int arr_global_lim, int arr_list_lim, int blk_global_lim,
                int blk_list_lim);
H5_DLL herr_t H5get_free_list_sizes(size_t *reg_size, size_t *arr_size,
    size_t *blk_size, size_t *fac_size);
H5_DLL herr_t H5get_alloc_stats(H5_alloc_stats_t *stats);
H5_DLL herr_t H5get_libversion(unsigned *majnum, unsigned *minnum,
                unsigned *relnum);
H5_DLL herr_t H5check_version(unsigned majnum, unsigned minnum,
                unsigned relnum);
/**
 * \ingroup H5
 * \brief Determines whether the HDF5 library was built with the thread-safety
 *        feature enabled
 *
 * \param[out] is_ts Boolean value indicating whether the library was built
 *                   with thread-safety enabled
 * \return \herr_t
 *
 * \details The HDF5 library, although not internally multi-threaded, can be
 *          built with a thread-safety feature enabled that protects internal
 *          data structures with a mutex. In certain circumstances, it may be
 *          useful to determine, at run-time, whether the linked HDF5 library
 *          was built with the thread-safety feature enabled.
 */
H5_DLL herr_t H5is_library_threadsafe(hbool_t *is_ts);
H5_DLL herr_t H5free_memory(void *mem);
H5_DLL void *H5allocate_memory(size_t size, hbool_t clear);
H5_DLL void *H5resize_memory(void *mem, size_t size);

#ifdef __cplusplus
}
#endif
#endif /* _H5public_H */
