/**************************************************************************/ 
/*                                                                        */ 
/*            Copyright (c) 1996-2003 by Express Logic Inc.               */ 
/*                                                                        */ 
/*  This software is copyrighted by and is the sole property of Express   */ 
/*  Logic, Inc.  All rights, title, ownership, or other interests         */ 
/*  in the software remain the property of Express Logic, Inc.  This      */ 
/*  software may only be used in accordance with the corresponding        */ 
/*  license agreement.  Any unauthorized use, duplication, transmission,  */ 
/*  distribution, or disclosure of this software is expressly forbidden.  */ 
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */ 
/*  written consent of Express Logic, Inc.                                */ 
/*                                                                        */ 
/*  Express Logic, Inc. reserves the right to modify this software        */ 
/*  without notice.                                                       */ 
/*                                                                        */ 
/*  Express Logic, Inc.                     info@expresslogic.com         */
/*  11423 West Bernardo Court               http://www.expresslogic.com   */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** ThreadX Component                                                     */
/**                                                                       */
/**   Port Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/ 
/*                                                                        */ 
/*  PORT SPECIFIC C INFORMATION                            RELEASE        */ 
/*                                                                        */ 
/*    tx_port.h                                        Win32/Microsoft    */ 
/*                                                           4.0a         */ 
/*                                                                        */
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    William E. Lamie, Express Logic, Inc.                               */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This file contains data type definitions that make the ThreadX      */ 
/*    real-time kernel function identically on a variety of different     */ 
/*    processor architectures.  For example, the size or number of bits   */ 
/*    in an "int" data type vary between microprocessor architectures and */ 
/*    even C compilers for the same microprocessor.  ThreadX does not     */ 
/*    directly use native C data types.  Instead, ThreadX creates its     */ 
/*    own special types that can be mapped to actual data types by this   */ 
/*    file to guarantee consistency in the interface and functionality.   */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  11-01-1999     William E. Lamie         Initial Win32 Microsoft       */
/*                                            Version 3.0                 */
/*  01-28-2001     William E. Lamie         Changed version string and    */ 
/*                                            comments for version 4.0.   */ 
/*  01-01-2003     William E. Lamie         Modified comment(s), added    */ 
/*                                            timer initialization, and   */ 
/*                                            in-line initialization      */ 
/*                                            define, resulting in        */
/*                                            version 4.0a                */
/*                                                                        */ 
/**************************************************************************/ 

#ifndef TX_PORT
#define TX_PORT


/* Define various constants for the port.  */ 

#define TX_MINIMUM_STACK            200     /* Minimum stack size       */
#define TX_INT_DISABLE              1       /* Disable interrupts value */
#define TX_INT_ENABLE               0       /* Enable interrupt value   */


/* Define the system timer thread's default stack size and priority.  */

#ifndef TX_TIMER_THREAD_STACK_SIZE
#define TX_TIMER_THREAD_STACK_SIZE  4       /* Default timer thread stack size - Not used for Win32  */
#endif
#ifndef TX_TIMER_THREAD_PRIORITY    
#define TX_TIMER_THREAD_PRIORITY    0       /* Default timer thread priority    */ 
#endif

/* Define the in-line initialization constant so that modules with in-line
   initialization capabilities can prevent their initialization from being
   a function call.  */

#define TX_INLINE_INITIALIZATION


/* Define ThreadX specific types for the port.  */ 

typedef void                        VOID;
typedef void *                      VOID_PTR;
typedef char                        CHAR;
typedef char *                      CHAR_PTR;
typedef unsigned char               UCHAR;
typedef unsigned char *             UCHAR_PTR;
typedef int                         INT;
/* typedef int *                       INT_PTR;  Win32 doesn't like this!  */
typedef unsigned int                UINT;
/* typedef unsigned int *              UINT_PTR; Win32 doesn't like this!  */
typedef long                        LONG;
typedef long *                      LONG_PTR;
typedef unsigned long               ULONG;
typedef unsigned long *             ULONG_PTR;

#include <windows.h>


/* Define register constants for the port.  These definitions are 
   prioritized in the order they are defined.  In other words, REG_1
   is assigned to the most used variable, while REG_4 is assigned to
   the least used variable.  */

#define REG_1                       register
#define REG_2                       register
#define REG_3                       register
#define REG_4                       register
#define REG_5                       register


/* Define the port extension field of the thread control block.  Nothing 
   additional is needed for this port so it is defined as white space.  */

#define TX_THREAD_PORT_EXTENSION    HANDLE tx_win32_thread_handle; \
                                    DWORD  tx_win32_thread_id; \
                                    HANDLE tx_win32_thread_run_semaphore; \
                                    UINT   tx_win32_suspension_type; \
                                    UINT   tx_win32_int_disabled_flag;


/* Define ThreadX interrupt lockout and restore macros for protection on 
   access of critical kernel information.  The restore interrupt macro must 
   restore the interrupt posture of the running thread prior to the value 
   present prior to the disable macro.  In most cases, the save area macro
   is used to define a local function save area for the disable and restore
   macros.  */

/* In Win32, this is basically just critical section manipulation.  */

#define TX_INTERRUPT_SAVE_AREA      

#define TX_DISABLE                 EnterCriticalSection(&_tx_win32_critical_section); 
#define TX_RESTORE                 LeaveCriticalSection(&_tx_win32_critical_section);


/* Define the version ID of ThreadX.  This may be utilized by the application.  */

#ifdef  TX_THREAD_INIT
CHAR                            _tx_version_id[] = 
                                    "Copyright (c) 1996-2003 Express Logic Inc. * ThreadX Win32/Microsoft Version G4.0b.4.0a *";
#else
extern  CHAR                    _tx_version_id[];
#endif


/* Define externals for the Win32 port of ThreadX.  */

extern CRITICAL_SECTION     _tx_win32_critical_section;
extern HANDLE               _tx_win32_semaphore;
extern ULONG                _tx_win32_global_int_disabled_flag;


#define TX_WIN32_MEMORY_SIZE    32000
#define TX_TIMER_PERIODIC       18

#endif

