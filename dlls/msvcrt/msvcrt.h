/*
 * Copyright 2001 Jon Griffiths
 * Copyright 2004 Dimitrie O. Paun
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef __WINE_MSVCRT_H
#define __WINE_MSVCRT_H

#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "windef.h"
#include "winbase.h"

#define DBL80_MAX_10_EXP 4932
#define DBL80_MIN_10_EXP -4951

typedef void (__cdecl *terminate_function)(void);
typedef void (__cdecl *unexpected_function)(void);
typedef void (__cdecl *_se_translator_function)(unsigned int code, struct _EXCEPTION_POINTERS *info);
void __cdecl terminate(void);

typedef void (__cdecl *MSVCRT_invalid_parameter_handler)(const wchar_t*, const wchar_t*, const wchar_t*, unsigned, uintptr_t);
typedef void (__cdecl *MSVCRT_purecall_handler)(void);
typedef void (__cdecl *MSVCRT_security_error_handler)(int, void *);

typedef struct {ULONG x80[3];} MSVCRT__LDOUBLE; /* Intel 80 bit FP format has sizeof() 12 */

typedef struct __lc_time_data {
    union {
        const char *str[43];
        struct {
            const char *short_wday[7];
            const char *wday[7];
            const char *short_mon[12];
            const char *mon[12];
            const char *am;
            const char *pm;
            const char *short_date;
            const char *date;
            const char *time;
        } names;
    } str;
#if _MSVCR_VER < 110
    LCID lcid;
#endif
    int unk;
    int refcount;
    union {
        const wchar_t *wstr[43];
        struct {
            const wchar_t *short_wday[7];
            const wchar_t *wday[7];
            const wchar_t *short_mon[12];
            const wchar_t *mon[12];
            const wchar_t *am;
            const wchar_t *pm;
            const wchar_t *short_date;
            const wchar_t *date;
            const wchar_t *time;
        } names;
    } wstr;
#if _MSVCR_VER >= 110
    const wchar_t *locname;
#endif
    char data[1];
} __lc_time_data;

typedef struct threadmbcinfostruct {
    int refcount;
    int mbcodepage;
    int ismbcodepage;
    int mblcid;
    unsigned short mbulinfo[6];
    unsigned char mbctype[257];
    unsigned char mbcasemap[256];
} threadmbcinfo;

typedef struct _frame_info
{
    void *object;
    struct _frame_info *next;
} frame_info;

typedef struct
{
    frame_info frame_info;
    EXCEPTION_RECORD *rec;
    CONTEXT *context;
} cxx_frame_info;

frame_info* __cdecl _CreateFrameInfo(frame_info *fi, void *obj);
BOOL __cdecl __CxxRegisterExceptionObject(EXCEPTION_POINTERS*, cxx_frame_info*);
void __cdecl __CxxUnregisterExceptionObject(cxx_frame_info*, BOOL);
void CDECL __DestructExceptionObject(EXCEPTION_RECORD*);

/* TLS data */
extern DWORD msvcrt_tls_index DECLSPEC_HIDDEN;

#define LOCALE_FREE     0x1
#define LOCALE_THREAD   0x2

/* Keep in sync with msvcr90/tests/msvcr90.c */
struct __thread_data {
    DWORD                           tid;
    HANDLE                          handle;
    int                             thread_errno;
    __msvcrt_ulong                  thread_doserrno;
    int                             unk1;
    unsigned int                    random_seed;        /* seed for rand() */
    char                           *strtok_next;        /* next ptr for strtok() */
    wchar_t                        *wcstok_next;        /* next ptr for wcstok() */
    unsigned char                  *mbstok_next;        /* next ptr for mbstok() */
    char                           *strerror_buffer;    /* buffer for strerror */
    wchar_t                        *wcserror_buffer;    /* buffer for wcserror */
    char                           *tmpnam_buffer;      /* buffer for tmpname() */
    wchar_t                        *wtmpnam_buffer;     /* buffer for wtmpname() */
    void                           *unk2[2];
    char                           *asctime_buffer;     /* buffer for asctime */
    wchar_t                        *wasctime_buffer;    /* buffer for wasctime */
    struct tm                      *time_buffer;        /* buffer for localtime/gmtime */
    char                           *efcvt_buffer;       /* buffer for ecvt/fcvt */
    int                             unk3[2];
    void                           *unk4[3];
    EXCEPTION_POINTERS             *xcptinfo;
    int                             fpecode;
    pthreadmbcinfo                  mbcinfo;
    pthreadlocinfo                  locinfo;
    int                             locale_flags;
    int                             unk5[1];
    terminate_function              terminate_handler;
    unexpected_function             unexpected_handler;
    _se_translator_function         se_translator;      /* preserve offset to exc_record and processing_throw */
    void                           *unk6;
    EXCEPTION_RECORD               *exc_record;
    CONTEXT                        *ctx_record;
    int                             processing_throw;
    frame_info                     *frame_info_head;
    void                           *unk8[6];
    LCID                            cached_lcid;
    BOOL                            cached_sname;
    int                             unk9[2];
    DWORD                           cached_cp;
    char                            cached_locale[131];
    void                           *unk10[100];
#if _MSVCR_VER >= 140
    MSVCRT_invalid_parameter_handler invalid_parameter_handler;
#endif
};

typedef struct __thread_data thread_data_t;

extern thread_data_t *CDECL msvcrt_get_thread_data(void) DECLSPEC_HIDDEN;

LCID locale_to_LCID(const char*, unsigned short*, BOOL*) DECLSPEC_HIDDEN;
extern _locale_t MSVCRT_locale DECLSPEC_HIDDEN;
extern __lc_time_data cloc_time_data DECLSPEC_HIDDEN;
extern unsigned int MSVCRT___lc_codepage;
extern int MSVCRT___lc_collate_cp;
extern WORD MSVCRT__ctype [257];
extern BOOL initial_locale DECLSPEC_HIDDEN;
extern WORD *MSVCRT__pwctype;

void msvcrt_set_errno(int) DECLSPEC_HIDDEN;
#if _MSVCR_VER >= 80
typedef enum {
    EXCEPTION_BAD_ALLOC,
#if _MSVCR_VER >= 100
    EXCEPTION_SCHEDULER_RESOURCE_ALLOCATION_ERROR,
    EXCEPTION_IMPROPER_LOCK,
    EXCEPTION_INVALID_SCHEDULER_POLICY_KEY,
    EXCEPTION_INVALID_SCHEDULER_POLICY_VALUE,
    EXCEPTION_INVALID_SCHEDULER_POLICY_THREAD_SPECIFICATION,
    EXCEPTION_IMPROPER_SCHEDULER_ATTACH,
    EXCEPTION_IMPROPER_SCHEDULER_DETACH,
#endif
} exception_type;
void throw_exception(exception_type, HRESULT, const char*) DECLSPEC_HIDDEN;
#endif

void __cdecl _purecall(void);
void __cdecl _amsg_exit(int errnum);

extern char **MSVCRT__environ;
extern wchar_t **MSVCRT__wenviron;

extern char ** msvcrt_SnapshotOfEnvironmentA(char **) DECLSPEC_HIDDEN;
extern wchar_t ** msvcrt_SnapshotOfEnvironmentW(wchar_t **) DECLSPEC_HIDDEN;

wchar_t *msvcrt_wstrdupa(const char *) DECLSPEC_HIDDEN;

extern unsigned int MSVCRT__commode;

/* FIXME: This should be declared in new.h but it's not an extern "C" so
 * it would not be much use anyway. Even for Winelib applications.
 */
void* __cdecl operator_new(size_t);
void __cdecl operator_delete(void*);
int __cdecl _set_new_mode(int mode);

typedef void* (__cdecl *malloc_func_t)(size_t);
typedef void  (__cdecl *free_func_t)(void*);

/* Setup and teardown multi threaded locks */
extern void msvcrt_init_mt_locks(void) DECLSPEC_HIDDEN;
extern void msvcrt_free_locks(void) DECLSPEC_HIDDEN;

extern void msvcrt_init_exception(void*) DECLSPEC_HIDDEN;
extern BOOL msvcrt_init_locale(void) DECLSPEC_HIDDEN;
extern void msvcrt_init_math(void*) DECLSPEC_HIDDEN;
extern void msvcrt_init_io(void) DECLSPEC_HIDDEN;
extern void msvcrt_free_io(void) DECLSPEC_HIDDEN;
extern void msvcrt_init_console(void) DECLSPEC_HIDDEN;
extern void msvcrt_free_console(void) DECLSPEC_HIDDEN;
extern void msvcrt_init_args(void) DECLSPEC_HIDDEN;
extern void msvcrt_free_args(void) DECLSPEC_HIDDEN;
extern void msvcrt_init_signals(void) DECLSPEC_HIDDEN;
extern void msvcrt_free_signals(void) DECLSPEC_HIDDEN;
extern void msvcrt_free_popen_data(void) DECLSPEC_HIDDEN;
extern BOOL msvcrt_init_heap(void) DECLSPEC_HIDDEN;
extern void msvcrt_destroy_heap(void) DECLSPEC_HIDDEN;
extern void msvcrt_init_clock(void) DECLSPEC_HIDDEN;

#if _MSVCR_VER >= 100
extern void msvcrt_init_scheduler(void*) DECLSPEC_HIDDEN;
extern void msvcrt_free_scheduler(void) DECLSPEC_HIDDEN;
extern void msvcrt_free_scheduler_thread(void) DECLSPEC_HIDDEN;
#endif

extern unsigned msvcrt_create_io_inherit_block(WORD*, BYTE**) DECLSPEC_HIDDEN;

extern unsigned int __cdecl _control87(unsigned int, unsigned int);

/* run-time error codes */
#define _RT_STACK       0
#define _RT_NULLPTR     1
#define _RT_FLOAT       2
#define _RT_INTDIV      3
#define _RT_EXECMEM     5
#define _RT_EXECFORM    6
#define _RT_EXECENV     7
#define _RT_SPACEARG    8
#define _RT_SPACEENV    9
#define _RT_ABORT       10
#define _RT_NPTR        12
#define _RT_FPTR        13
#define _RT_BREAK       14
#define _RT_INT         15
#define _RT_THREAD      16
#define _RT_LOCK        17
#define _RT_HEAP        18
#define _RT_OPENCON     19
#define _RT_QWIN        20
#define _RT_NOMAIN      21
#define _RT_NONCONT     22
#define _RT_INVALDISP   23
#define _RT_ONEXIT      24
#define _RT_PUREVIRT    25
#define _RT_STDIOINIT   26
#define _RT_LOWIOINIT   27
#define _RT_HEAPINIT    28
#define _RT_DOMAIN      120
#define _RT_SING        121
#define _RT_TLOSS       122
#define _RT_CRNL        252
#define _RT_BANNER      255

extern FILE MSVCRT__iob[];

typedef struct _complex _Dcomplex;

struct MSVCRT__heapinfo {
  int*           _pentry;
  size_t         _size;
  int            _useflag;
};

#ifdef __i386__
struct MSVCRT___JUMP_BUFFER {
    unsigned long Ebp;
    unsigned long Ebx;
    unsigned long Edi;
    unsigned long Esi;
    unsigned long Esp;
    unsigned long Eip;
    unsigned long Registration;
    unsigned long TryLevel;
    /* Start of new struct members */
    unsigned long Cookie;
    unsigned long UnwindFunc;
    unsigned long UnwindData[6];
};
#elif defined(__x86_64__)
struct MSVCRT__SETJMP_FLOAT128
{
    unsigned __int64 DECLSPEC_ALIGN(16) Part[2];
};
struct MSVCRT___JUMP_BUFFER
{
    unsigned __int64 Frame;
    unsigned __int64 Rbx;
    unsigned __int64 Rsp;
    unsigned __int64 Rbp;
    unsigned __int64 Rsi;
    unsigned __int64 Rdi;
    unsigned __int64 R12;
    unsigned __int64 R13;
    unsigned __int64 R14;
    unsigned __int64 R15;
    unsigned __int64 Rip;
    unsigned __int64 Spare;
    struct MSVCRT__SETJMP_FLOAT128 Xmm6;
    struct MSVCRT__SETJMP_FLOAT128 Xmm7;
    struct MSVCRT__SETJMP_FLOAT128 Xmm8;
    struct MSVCRT__SETJMP_FLOAT128 Xmm9;
    struct MSVCRT__SETJMP_FLOAT128 Xmm10;
    struct MSVCRT__SETJMP_FLOAT128 Xmm11;
    struct MSVCRT__SETJMP_FLOAT128 Xmm12;
    struct MSVCRT__SETJMP_FLOAT128 Xmm13;
    struct MSVCRT__SETJMP_FLOAT128 Xmm14;
    struct MSVCRT__SETJMP_FLOAT128 Xmm15;
};
#elif defined(__arm__)
struct MSVCRT___JUMP_BUFFER
{
    unsigned long Frame;
    unsigned long R4;
    unsigned long R5;
    unsigned long R6;
    unsigned long R7;
    unsigned long R8;
    unsigned long R9;
    unsigned long R10;
    unsigned long R11;
    unsigned long Sp;
    unsigned long Pc;
    unsigned long Fpscr;
    unsigned long long D[8];
};
#elif defined(__aarch64__)
struct MSVCRT___JUMP_BUFFER
{
    unsigned __int64 Frame;
    unsigned __int64 Reserved;
    unsigned __int64 X19;
    unsigned __int64 X20;
    unsigned __int64 X21;
    unsigned __int64 X22;
    unsigned __int64 X23;
    unsigned __int64 X24;
    unsigned __int64 X25;
    unsigned __int64 X26;
    unsigned __int64 X27;
    unsigned __int64 X28;
    unsigned __int64 Fp;
    unsigned __int64 Lr;
    unsigned __int64 Sp;
    unsigned long Fpcr;
    unsigned long Fpsr;
    double D[8];
};
#endif /* __i386__ */

struct MSVCRT__finddata32_t {
  unsigned int attrib;
  __time32_t time_create;
  __time32_t time_access;
  __time32_t time_write;
  _fsize_t size;
  char name[260];
};

struct MSVCRT__finddata32i64_t {
  unsigned int attrib;
  __time32_t time_create;
  __time32_t time_access;
  __time32_t time_write;
  __int64 DECLSPEC_ALIGN(8) size;
  char name[260];
};

struct MSVCRT__finddata64i32_t {
  unsigned int attrib;
  __time64_t time_create;
  __time64_t time_access;
  __time64_t time_write;
  _fsize_t size;
  char name[260];
};

struct MSVCRT__finddata64_t {
  unsigned int attrib;
  __time64_t time_create;
  __time64_t time_access;
  __time64_t time_write;
  __int64 DECLSPEC_ALIGN(8) size;
  char              name[260];
};

struct MSVCRT__wfinddata32_t {
  unsigned int attrib;
  __time32_t time_create;
  __time32_t time_access;
  __time32_t time_write;
  _fsize_t size;
  wchar_t name[260];
};

struct MSVCRT__wfinddata32i64_t {
  unsigned int attrib;
  __time32_t time_create;
  __time32_t time_access;
  __time32_t time_write;
  __int64 DECLSPEC_ALIGN(8) size;
  wchar_t name[260];
};

struct MSVCRT__wfinddata64i32_t {
  unsigned int attrib;
  __time64_t time_create;
  __time64_t time_access;
  __time64_t time_write;
  _fsize_t size;
  wchar_t name[260];
};

struct MSVCRT__wfinddata64_t {
  unsigned int attrib;
  __time64_t time_create;
  __time64_t time_access;
  __time64_t time_write;
  __int64 DECLSPEC_ALIGN(8) size;
  wchar_t name[260];
};

struct MSVCRT__stat32 {
  _dev_t st_dev;
  _ino_t st_ino;
  unsigned short st_mode;
  short st_nlink;
  short st_uid;
  short st_gid;
  _dev_t st_rdev;
  _off_t st_size;
  __time32_t st_atime;
  __time32_t st_mtime;
  __time32_t st_ctime;
};

struct MSVCRT__stat32i64 {
  _dev_t st_dev;
  _ino_t st_ino;
  unsigned short st_mode;
  short st_nlink;
  short st_uid;
  short st_gid;
  _dev_t st_rdev;
  __int64 DECLSPEC_ALIGN(8) st_size;
  __time32_t st_atime;
  __time32_t st_mtime;
  __time32_t st_ctime;
};

struct MSVCRT__stat64i32 {
  _dev_t st_dev;
  _ino_t st_ino;
  unsigned short st_mode;
  short st_nlink;
  short st_uid;
  short st_gid;
  _dev_t st_rdev;
  _off_t st_size;
  __time64_t st_atime;
  __time64_t st_mtime;
  __time64_t st_ctime;
};

struct MSVCRT__stat64 {
  _dev_t st_dev;
  _ino_t st_ino;
  unsigned short st_mode;
  short st_nlink;
  short st_uid;
  short st_gid;
  _dev_t st_rdev;
  __int64 DECLSPEC_ALIGN(8) st_size;
  __time64_t st_atime;
  __time64_t st_mtime;
  __time64_t st_ctime;
};

#ifdef _WIN64
#define MSVCRT__finddata_t     MSVCRT__finddata64i32_t
#define MSVCRT__finddatai64_t  MSVCRT__finddata64_t
#define MSVCRT__wfinddata_t    MSVCRT__wfinddata64i32_t
#define MSVCRT__wfinddatai64_t MSVCRT__wfinddata64_t
#define MSVCRT__stat           MSVCRT__stat64i32
#define MSVCRT__stati64        MSVCRT__stat64
#else
#define MSVCRT__finddata_t     MSVCRT__finddata32_t
#define MSVCRT__finddatai64_t  MSVCRT__finddata32i64_t
#define MSVCRT__wfinddata_t    MSVCRT__wfinddata32_t
#define MSVCRT__wfinddatai64_t MSVCRT__wfinddata32i64_t
#define MSVCRT__stat           MSVCRT__stat32
#define MSVCRT__stati64        MSVCRT__stat32i64
#endif

#define MSVCRT_TMP_MAX   0x7fff
#define MSVCRT_TMP_MAX_S 0x7fffffff
#define MSVCRT_RAND_MAX  0x7fff
#define MSVCRT_BUFSIZ    512

#define MSVCRT_SEEK_SET  0
#define MSVCRT_SEEK_CUR  1
#define MSVCRT_SEEK_END  2

#define MSVCRT_NO_CONSOLE_FD (-2)
#define MSVCRT_NO_CONSOLE ((HANDLE)MSVCRT_NO_CONSOLE_FD)

#define MSVCRT_STDIN_FILENO  0
#define MSVCRT_STDOUT_FILENO 1
#define MSVCRT_STDERR_FILENO 2

/* more file._flag flags, but these conflict with Unix */
#define MSVCRT__IOFBF    0x0000
#define MSVCRT__IONBF    0x0004
#define MSVCRT__IOLBF    0x0040

#define MSVCRT_FILENAME_MAX 260
#define MSVCRT_DRIVE_MAX    3
#define MSVCRT_FNAME_MAX    256
#define MSVCRT_DIR_MAX      256
#define MSVCRT_EXT_MAX      256
#define MSVCRT_PATH_MAX     260
#define MSVCRT_stdin       (MSVCRT__iob+MSVCRT_STDIN_FILENO)
#define MSVCRT_stdout      (MSVCRT__iob+MSVCRT_STDOUT_FILENO)
#define MSVCRT_stderr      (MSVCRT__iob+MSVCRT_STDERR_FILENO)

#define MSVCRT__P_WAIT    0
#define MSVCRT__P_NOWAIT  1
#define MSVCRT__P_OVERLAY 2
#define MSVCRT__P_NOWAITO 3
#define MSVCRT__P_DETACH  4

#define MSVCRT__HEAPEMPTY      -1
#define MSVCRT__HEAPOK         -2
#define MSVCRT__HEAPBADBEGIN   -3
#define MSVCRT__HEAPBADNODE    -4
#define MSVCRT__HEAPEND        -5
#define MSVCRT__HEAPBADPTR     -6

#define MSVCRT__FREEENTRY      0
#define MSVCRT__USEDENTRY      1

#define MSVCRT__OUT_TO_DEFAULT 0
#define MSVCRT__OUT_TO_STDERR  1
#define MSVCRT__OUT_TO_MSGBOX  2
#define MSVCRT__REPORT_ERRMODE 3

#define MSVCRT__IOREAD   0x0001
#define MSVCRT__IOWRT    0x0002
#define MSVCRT__IOMYBUF  0x0008
#define MSVCRT__IOEOF    0x0010
#define MSVCRT__IOERR    0x0020
#define MSVCRT__IOSTRG   0x0040
#define MSVCRT__IORW     0x0080
#define MSVCRT__USERBUF  0x0100
#define MSVCRT__IOCOMMIT 0x4000

#define MSVCRT__S_IEXEC  0x0040
#define MSVCRT__S_IWRITE 0x0080
#define MSVCRT__S_IREAD  0x0100
#define MSVCRT__S_IFIFO  0x1000
#define MSVCRT__S_IFCHR  0x2000
#define MSVCRT__S_IFDIR  0x4000
#define MSVCRT__S_IFREG  0x8000
#define MSVCRT__S_IFMT   0xF000

#define MSVCRT__LK_UNLCK  0
#define MSVCRT__LK_LOCK   1
#define MSVCRT__LK_NBLCK  2
#define MSVCRT__LK_RLCK   3
#define MSVCRT__LK_NBRLCK 4

#define	MSVCRT__SH_COMPAT	0x00	/* Compatibility */
#define	MSVCRT__SH_DENYRW	0x10	/* Deny read/write */
#define	MSVCRT__SH_DENYWR	0x20	/* Deny write */
#define	MSVCRT__SH_DENYRD	0x30	/* Deny read */
#define	MSVCRT__SH_DENYNO	0x40	/* Deny nothing */

#define MSVCRT__O_RDONLY        0
#define MSVCRT__O_WRONLY        1
#define MSVCRT__O_RDWR          2
#define MSVCRT__O_ACCMODE       (MSVCRT__O_RDONLY|MSVCRT__O_WRONLY|MSVCRT__O_RDWR)
#define MSVCRT__O_APPEND        0x0008
#define MSVCRT__O_RANDOM        0x0010
#define MSVCRT__O_SEQUENTIAL    0x0020
#define MSVCRT__O_TEMPORARY     0x0040
#define MSVCRT__O_NOINHERIT     0x0080
#define MSVCRT__O_CREAT         0x0100
#define MSVCRT__O_TRUNC         0x0200
#define MSVCRT__O_EXCL          0x0400
#define MSVCRT__O_SHORT_LIVED   0x1000
#define MSVCRT__O_TEXT          0x4000
#define MSVCRT__O_BINARY        0x8000
#define MSVCRT__O_RAW           MSVCRT__O_BINARY
#define MSVCRT__O_WTEXT         0x10000
#define MSVCRT__O_U16TEXT       0x20000
#define MSVCRT__O_U8TEXT        0x40000

/* _statusfp bit flags */
#define MSVCRT__SW_INEXACT      0x00000001 /* inexact (precision) */
#define MSVCRT__SW_UNDERFLOW    0x00000002 /* underflow */
#define MSVCRT__SW_OVERFLOW     0x00000004 /* overflow */
#define MSVCRT__SW_ZERODIVIDE   0x00000008 /* zero divide */
#define MSVCRT__SW_INVALID      0x00000010 /* invalid */

#define MSVCRT__SW_UNEMULATED     0x00000040  /* unemulated instruction */
#define MSVCRT__SW_SQRTNEG        0x00000080  /* square root of a neg number */
#define MSVCRT__SW_STACKOVERFLOW  0x00000200  /* FP stack overflow */
#define MSVCRT__SW_STACKUNDERFLOW 0x00000400  /* FP stack underflow */

#define MSVCRT__SW_DENORMAL     0x00080000 /* denormal status bit */

/* fpclass constants */
#define MSVCRT__FPCLASS_SNAN 0x0001  /* Signaling "Not a Number" */
#define MSVCRT__FPCLASS_QNAN 0x0002  /* Quiet "Not a Number" */
#define MSVCRT__FPCLASS_NINF 0x0004  /* Negative Infinity */
#define MSVCRT__FPCLASS_NN   0x0008  /* Negative Normal */
#define MSVCRT__FPCLASS_ND   0x0010  /* Negative Denormal */
#define MSVCRT__FPCLASS_NZ   0x0020  /* Negative Zero */
#define MSVCRT__FPCLASS_PZ   0x0040  /* Positive Zero */
#define MSVCRT__FPCLASS_PD   0x0080  /* Positive Denormal */
#define MSVCRT__FPCLASS_PN   0x0100  /* Positive Normal */
#define MSVCRT__FPCLASS_PINF 0x0200  /* Positive Infinity */

/* fpclassify constants */
#define MSVCRT_FP_INFINITE   1
#define MSVCRT_FP_NAN        2
#define MSVCRT_FP_NORMAL    -1
#define MSVCRT_FP_SUBNORMAL -2
#define MSVCRT_FP_ZERO       0

#define MSVCRT__MCW_EM        0x0008001f
#define MSVCRT__MCW_IC        0x00040000
#define MSVCRT__MCW_RC        0x00000300
#define MSVCRT__MCW_PC        0x00030000
#define MSVCRT__MCW_DN        0x03000000

#define MSVCRT__EM_INVALID    0x00000010
#define MSVCRT__EM_DENORMAL   0x00080000
#define MSVCRT__EM_ZERODIVIDE 0x00000008
#define MSVCRT__EM_OVERFLOW   0x00000004
#define MSVCRT__EM_UNDERFLOW  0x00000002
#define MSVCRT__EM_INEXACT    0x00000001
#define MSVCRT__IC_AFFINE     0x00040000
#define MSVCRT__IC_PROJECTIVE 0x00000000
#define MSVCRT__RC_CHOP       0x00000300
#define MSVCRT__RC_UP         0x00000200
#define MSVCRT__RC_DOWN       0x00000100
#define MSVCRT__RC_NEAR       0x00000000
#define MSVCRT__PC_24         0x00020000
#define MSVCRT__PC_53         0x00010000
#define MSVCRT__PC_64         0x00000000
#define MSVCRT__DN_SAVE       0x00000000
#define MSVCRT__DN_FLUSH      0x01000000
#define MSVCRT__DN_FLUSH_OPERANDS_SAVE_RESULTS 0x02000000
#define MSVCRT__DN_SAVE_OPERANDS_FLUSH_RESULTS 0x03000000
#define MSVCRT__EM_AMBIGUOUS  0x80000000

#define MSVCRT_CLOCKS_PER_SEC 1000

/* signals */
#define MSVCRT_SIGINT   2
#define MSVCRT_SIGILL   4
#define MSVCRT_SIGFPE   8
#define MSVCRT_SIGSEGV  11
#define MSVCRT_SIGTERM  15
#define MSVCRT_SIGBREAK 21
#define MSVCRT_SIGABRT  22
#define MSVCRT_NSIG     (MSVCRT_SIGABRT + 1)

typedef void (__cdecl *MSVCRT___sighandler_t)(int);

#define MSVCRT_SIG_DFL ((MSVCRT___sighandler_t)0)
#define MSVCRT_SIG_IGN ((MSVCRT___sighandler_t)1)
#define MSVCRT_SIG_ERR ((MSVCRT___sighandler_t)-1)

#define MSVCRT__FPE_INVALID            0x81
#define MSVCRT__FPE_DENORMAL           0x82
#define MSVCRT__FPE_ZERODIVIDE         0x83
#define MSVCRT__FPE_OVERFLOW           0x84
#define MSVCRT__FPE_UNDERFLOW          0x85
#define MSVCRT__FPE_INEXACT            0x86
#define MSVCRT__FPE_UNEMULATED         0x87
#define MSVCRT__FPE_SQRTNEG            0x88
#define MSVCRT__FPE_STACKOVERFLOW      0x8a
#define MSVCRT__FPE_STACKUNDERFLOW     0x8b
#define MSVCRT__FPE_EXPLICITGEN        0x8c

#define MSVCRT__TRUNCATE ((size_t)-1)

#define _MAX__TIME64_T    (((__time64_t)0x00000007 << 32) | 0x93406FFF)

/* _set_abort_behavior codes */
#define MSVCRT__WRITE_ABORT_MSG    1
#define MSVCRT__CALL_REPORTFAULT   2

/* _get_output_format return code */
#define MSVCRT__TWO_DIGIT_EXPONENT 0x1

#define MSVCRT__NLSCMPERROR ((unsigned int)0x7fffffff)

int __cdecl      MSVCRT_atoi(const char *str);
int __cdecl      MSVCRT_isalpha(int c);
int __cdecl      MSVCRT_isdigit(int c);
int __cdecl      MSVCRT_isspace(int c);
int __cdecl      MSVCRT_iswalpha(wint_t);
int __cdecl      MSVCRT_iswspace(wint_t);
int __cdecl      MSVCRT_iswdigit(wint_t);
int __cdecl      MSVCRT_isleadbyte(int);
int __cdecl      MSVCRT__isleadbyte_l(int, _locale_t);
int __cdecl      MSVCRT__isspace_l(int, _locale_t);
int __cdecl      MSVCRT__iswspace_l(wchar_t, _locale_t);

void __cdecl     MSVCRT__lock_file(FILE*);
void __cdecl     MSVCRT__unlock_file(FILE*);
int __cdecl      MSVCRT_fgetc(FILE*);
int __cdecl      MSVCRT__fgetc_nolock(FILE*);
int __cdecl      MSVCRT__fputc_nolock(int,FILE*);
int __cdecl      MSVCRT_ungetc(int,FILE*);
int __cdecl      MSVCRT__ungetc_nolock(int,FILE*);
wint_t __cdecl MSVCRT_fgetwc(FILE*);
wint_t __cdecl MSVCRT__fgetwc_nolock(FILE*);
wint_t __cdecl MSVCRT__fputwc_nolock(wint_t,FILE*);
wint_t __cdecl MSVCRT_ungetwc(wint_t,FILE*);
wint_t __cdecl MSVCRT__ungetwc_nolock(wint_t, FILE*);
int __cdecl      MSVCRT__fseeki64_nolock(FILE*,__int64,int);
__int64 __cdecl  MSVCRT__ftelli64(FILE* file);
__int64 __cdecl  MSVCRT__ftelli64_nolock(FILE*);
void __cdecl     MSVCRT__exit(int);
void __cdecl     MSVCRT_abort(void);
char* __cdecl    MSVCRT_getenv(const char*);
size_t __cdecl MSVCRT__fread_nolock(void*,size_t,size_t,FILE*);
size_t __cdecl MSVCRT__fread_nolock_s(void*,size_t,size_t,size_t,FILE*);
size_t __cdecl MSVCRT__fwrite_nolock(const void*,size_t,size_t,FILE*);
int __cdecl      MSVCRT_fclose(FILE*);
int __cdecl      MSVCRT__fclose_nolock(FILE*);
int __cdecl      MSVCRT__fflush_nolock(FILE*);
FILE* __cdecl MSVCRT__iob_func(void);
__time32_t __cdecl MSVCRT__time32(__time32_t*);
__time64_t __cdecl MSVCRT__time64(__time64_t*);
FILE*   __cdecl MSVCRT__fdopen(int, const char *);
FILE*   __cdecl MSVCRT__wfdopen(int, const wchar_t *);
int            WINAPIV MSVCRT_fwprintf(FILE *file, const wchar_t *format, ...);
int            __cdecl MSVCRT_vsnwprintf(wchar_t *str, size_t len,
                                       const wchar_t *format, __ms_va_list valist );
int            WINAPIV MSVCRT__snwprintf(wchar_t*, size_t, const wchar_t*, ...);
int            WINAPIV MSVCRT_sprintf(char*,const char*,...);
int            WINAPIV MSVCRT__snprintf(char*,size_t,const char*,...);
int            WINAPIV MSVCRT__scprintf(const char*,...);
int            __cdecl MSVCRT_raise(int sig);
int            __cdecl MSVCRT__set_printf_count_output(int);

#define MSVCRT__ENABLE_PER_THREAD_LOCALE 1
#define MSVCRT__DISABLE_PER_THREAD_LOCALE 2

_locale_t CDECL get_current_locale_noalloc(_locale_t locale) DECLSPEC_HIDDEN;
void CDECL free_locale_noalloc(_locale_t locale) DECLSPEC_HIDDEN;
pthreadlocinfo CDECL get_locinfo(void) DECLSPEC_HIDDEN;
pthreadmbcinfo CDECL get_mbcinfo(void) DECLSPEC_HIDDEN;
threadmbcinfo* create_mbcinfo(int, LCID, threadmbcinfo*) DECLSPEC_HIDDEN;
void free_locinfo(pthreadlocinfo) DECLSPEC_HIDDEN;
void free_mbcinfo(pthreadmbcinfo) DECLSPEC_HIDDEN;
int __cdecl __crtLCMapStringA(LCID, DWORD, const char*, int, char*, int, unsigned int, int) DECLSPEC_HIDDEN;

int            __cdecl MSVCRT__write(int,const void*,unsigned int);
int            __cdecl _getch(void);
int            __cdecl _ismbblead(unsigned int);
int            __cdecl _ismbblead_l(unsigned int, _locale_t);
int            __cdecl _ismbclegal(unsigned int c);
int            __cdecl _ismbstrail(const unsigned char* start, const unsigned char* str);
int            __cdecl MSVCRT_mbtowc(wchar_t*,const char*,size_t);
int            __cdecl MSVCRT_mbtowc_l(wchar_t*,const char*,size_t,_locale_t);
size_t         __cdecl MSVCRT_mbstowcs(wchar_t*,const char*,size_t);
size_t         __cdecl MSVCRT__mbstowcs_l(wchar_t*, const char*, size_t, _locale_t);
int            __cdecl MSVCRT__mbstowcs_s_l(size_t*, wchar_t*,
        size_t, const char*, size_t, _locale_t);
size_t __cdecl MSVCRT_wcstombs(char*,const wchar_t*,size_t);
size_t __cdecl MSVCRT__wcstombs_l(char*, const wchar_t*, size_t, _locale_t);
intptr_t __cdecl MSVCRT__spawnve(int,const char*,const char* const *,const char* const *);
intptr_t __cdecl MSVRT__spawnvpe(int,const char*,const char* const *,const char* const *);
intptr_t __cdecl MSVCRT__wspawnve(int,const wchar_t*,const wchar_t* const *,const wchar_t* const *);
intptr_t __cdecl MSVCRT__wspawnvpe(int,const wchar_t*,const wchar_t* const *,const wchar_t* const *);
void __cdecl     MSVCRT__searchenv(const char*,const char*,char*);
int __cdecl      MSVCRT__getdrive(void);
char* __cdecl    MSVCRT__strdup(const char*);
char* __cdecl    MSVCRT__strnset(char*,int,size_t);
char* __cdecl    _strset(char*,int);
int __cdecl      _ungetch(int);
int __cdecl      _cputs(const char*);
int WINAPIV      _cprintf(const char*,...);
int WINAPIV      _cwprintf(const wchar_t*,...);
int WINAPIV      MSVCRT_sscanf(const char *, const char *, ...);
char*** __cdecl  MSVCRT___p__environ(void);
int*    __cdecl  __p___mb_cur_max(void);
int*    __cdecl  MSVCRT___p__fmode(void);
wchar_t* __cdecl MSVCRT__wcsdup(const wchar_t*);
size_t __cdecl MSVCRT_strnlen(const char *,size_t);
size_t __cdecl MSVCRT_wcsnlen(const wchar_t*,size_t);
wchar_t*** __cdecl MSVCRT___p__wenviron(void);
INT     __cdecl MSVCRT_wctomb(char*,wchar_t);
int     __cdecl MSVCRT__wctomb_l(char*, wchar_t, _locale_t);
char*   __cdecl MSVCRT__strdate(char* date);
char*   __cdecl MSVCRT__strtime(char* date);
int     __cdecl _setmbcp(int);
int     __cdecl MSVCRT__close(int);
int     __cdecl MSVCRT__dup(int);
int     __cdecl MSVCRT__dup2(int, int);
int     __cdecl MSVCRT__pipe(int *, unsigned int, int);
wchar_t* __cdecl MSVCRT__wgetenv(const wchar_t*);
void __cdecl    MSVCRT__wsearchenv(const wchar_t*, const wchar_t*, wchar_t*);
intptr_t __cdecl MSVCRT__spawnvpe(int, const char*, const char* const*, const char* const*);
int __cdecl      MSVCRT__toupper_l(int,_locale_t);
int __cdecl      MSVCRT__tolower_l(int,_locale_t);
int __cdecl      MSVCRT__towupper_l(wint_t,_locale_t);
int __cdecl      MSVCRT__towlower_l(wint_t,_locale_t);
int __cdecl      MSVCRT__toupper(int); /* only use on lower-case ASCII characters */
int __cdecl      MSVCRT__stricmp(const char*, const char*);
int __cdecl      MSVCRT__strnicmp(const char*, const char*, size_t);
int __cdecl      MSVCRT__strnicoll_l(const char*, const char*, size_t, _locale_t);
int __cdecl      MSVCRT__strncoll_l(const char*, const char*, size_t, _locale_t);
char* __cdecl    MSVCRT_strncat(char*,const char *,size_t);
int __cdecl      MSVCRT_strncmp(const char*, const char*, size_t);
int __cdecl      MSVCRT_strcmp(const char*, const char*);
char* __cdecl    MSVCRT_strrchr(const char *, int);
char* __cdecl    MSVCRT_strstr(const char*, const char*);
__msvcrt_long __cdecl MSVCRT_strtol(const char*, char**, int);
unsigned int __cdecl MSVCRT__get_output_format(void);
char* __cdecl MSVCRT_strtok_s(char*, const char*, char**);
char* __cdecl MSVCRT__itoa(int, char*, int);
int __cdecl MSVCRT_wcsncmp(const wchar_t*, const wchar_t*, size_t);
int __cdecl MSVCRT__wcsnicmp(const wchar_t*, const wchar_t*, size_t);
int __cdecl MSVCRT_towlower(wint_t);
int __cdecl MSVCRT_towupper(wint_t);
int __cdecl MSVCRT__isprint_l(int c, _locale_t locale);
int __cdecl MSVCRT__iswalnum_l(wchar_t, _locale_t);
int __cdecl MSVCRT__iswdigit_l(wchar_t, _locale_t);
int __cdecl MSVCRT__iswgraph_l(wchar_t, _locale_t);
int __cdecl MSVCRT__iswalpha_l(wchar_t, _locale_t);
int __cdecl MSVCRT__iswlower_l(wchar_t, _locale_t);
int __cdecl MSVCRT__iswupper_l(wchar_t, _locale_t);
int __cdecl MSVCRT__iswprint_l(wchar_t, _locale_t);
int __cdecl MSVCRT__iswpunct_l(wchar_t, _locale_t);
size_t __cdecl MSVCRT_wcslen(const wchar_t*);
wchar_t* __cdecl MSVCRT_wcscpy(wchar_t*, const wchar_t*);
wchar_t* __cdecl MSVCRT_wcschr(const wchar_t*, wchar_t);
wchar_t* __cdecl MSVCRT_wcscat(wchar_t*, const wchar_t*);

double __cdecl MSVCRT_floor( double x );
float  __cdecl MSVCRT_floorf( float x );
double __cdecl MSVCRT_frexp( double x, int *exp );
double __cdecl MSVCRT_log10( double x );
double __cdecl MSVCRT_sqrt( double x );
float  __cdecl MSVCRT_sqrtf( float x );

enum fpmod {
    FP_ROUND_ZERO, /* only used when dropped part contains only zeros */
    FP_ROUND_DOWN,
    FP_ROUND_EVEN,
    FP_ROUND_UP,
    FP_VAL_INFINITY,
    FP_VAL_NAN
};

struct fpnum {
    int sign;
    int exp;
    ULONGLONG m;
    enum fpmod mod;
};
struct fpnum fpnum_parse(wchar_t (*)(void*), void (*)(void*),
        void*, pthreadlocinfo, BOOL) DECLSPEC_HIDDEN;
int fpnum_double(struct fpnum*, double*) DECLSPEC_HIDDEN;
/* Maybe one day we'll enable the invalid parameter handlers with the full set of information (msvcrXXd)
 *      #define MSVCRT_INVALID_PMT(x) MSVCRT_call_invalid_parameter_handler(x, __FUNCTION__, __FILE__, __LINE__, 0)
 *      #define MSVCRT_CHECK_PMT(x)   ((x) ? TRUE : MSVCRT_INVALID_PMT(#x),FALSE)
 * Until this is done, just keep the same semantics for CHECK_PMT(), but without generating / sending
 * any information
 * NB : MSVCRT_call_invalid_parameter_handler is a wrapper around _invalid_parameter in order
 * to do the Ansi to Unicode transformation
 */
#define MSVCRT_INVALID_PMT(x,err)   (*_errno() = (err), _invalid_parameter(NULL, NULL, NULL, 0, 0))
#define MSVCRT_CHECK_PMT_ERR(x,err) ((x) || (MSVCRT_INVALID_PMT( 0, (err) ), FALSE))
#define MSVCRT_CHECK_PMT(x)         MSVCRT_CHECK_PMT_ERR((x), EINVAL)

#define MSVCRT__ARGMAX 100
typedef int (*puts_clbk_a)(void*, int, const char*);
typedef int (*puts_clbk_w)(void*, int, const wchar_t*);
typedef union _printf_arg
{
    void *get_ptr;
    int get_int;
    LONGLONG get_longlong;
    double get_double;
} printf_arg;
typedef printf_arg (*args_clbk)(void*, int, int, __ms_va_list*);
int pf_printf_a(puts_clbk_a, void*, const char*, _locale_t,
        DWORD, args_clbk, void*, __ms_va_list*) DECLSPEC_HIDDEN;
int pf_printf_w(puts_clbk_w, void*, const wchar_t*, _locale_t,
        DWORD, args_clbk, void*, __ms_va_list*) DECLSPEC_HIDDEN;
int create_positional_ctx_a(void*, const char*, __ms_va_list) DECLSPEC_HIDDEN;
int create_positional_ctx_w(void*, const wchar_t*, __ms_va_list) DECLSPEC_HIDDEN;
printf_arg arg_clbk_valist(void*, int, int, __ms_va_list*) DECLSPEC_HIDDEN;
printf_arg arg_clbk_positional(void*, int, int, __ms_va_list*) DECLSPEC_HIDDEN;

extern char* __cdecl __unDName(char *,const char*,int,malloc_func_t,free_func_t,unsigned short int);

/* __unDName/__unDNameEx flags */
#define UNDNAME_COMPLETE                 (0x0000)
#define UNDNAME_NO_LEADING_UNDERSCORES   (0x0001) /* Don't show __ in calling convention */
#define UNDNAME_NO_MS_KEYWORDS           (0x0002) /* Don't show calling convention at all */
#define UNDNAME_NO_FUNCTION_RETURNS      (0x0004) /* Don't show function/method return value */
#define UNDNAME_NO_ALLOCATION_MODEL      (0x0008)
#define UNDNAME_NO_ALLOCATION_LANGUAGE   (0x0010)
#define UNDNAME_NO_MS_THISTYPE           (0x0020)
#define UNDNAME_NO_CV_THISTYPE           (0x0040)
#define UNDNAME_NO_THISTYPE              (0x0060)
#define UNDNAME_NO_ACCESS_SPECIFIERS     (0x0080) /* Don't show access specifier (public/protected/private) */
#define UNDNAME_NO_THROW_SIGNATURES      (0x0100)
#define UNDNAME_NO_MEMBER_TYPE           (0x0200) /* Don't show static/virtual specifier */
#define UNDNAME_NO_RETURN_UDT_MODEL      (0x0400)
#define UNDNAME_32_BIT_DECODE            (0x0800)
#define UNDNAME_NAME_ONLY                (0x1000) /* Only report the variable/method name */
#define UNDNAME_NO_ARGUMENTS             (0x2000) /* Don't show method arguments */
#define UNDNAME_NO_SPECIAL_SYMS          (0x4000)
#define UNDNAME_NO_COMPLEX_TYPE          (0x8000)

#define UCRTBASE_PRINTF_MASK ( \
        _CRT_INTERNAL_PRINTF_LEGACY_VSPRINTF_NULL_TERMINATION | \
        _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR | \
        _CRT_INTERNAL_PRINTF_LEGACY_WIDE_SPECIFIERS | \
        _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY | \
        _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS | \
        _CRT_INTERNAL_PRINTF_STANDARD_ROUNDING )

#define MSVCRT_PRINTF_POSITIONAL_PARAMS                  (0x0100)
#define MSVCRT_PRINTF_INVOKE_INVALID_PARAM_HANDLER       (0x0200)

#define UCRTBASE_SCANF_MASK ( \
        _CRT_INTERNAL_SCANF_SECURECRT | \
        _CRT_INTERNAL_SCANF_LEGACY_WIDE_SPECIFIERS | \
        _CRT_INTERNAL_SCANF_LEGACY_MSVCRT_COMPATIBILITY )

#define COOPERATIVE_TIMEOUT_INFINITE ((unsigned int)-1)
#define COOPERATIVE_WAIT_TIMEOUT     ~0

#define INHERIT_THREAD_PRIORITY 0xF000

#endif /* __WINE_MSVCRT_H */
