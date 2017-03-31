/* 
    2014 - 2015

    \file stypes.h
    \brief Contains generic type definition of types for different processors
    
    stypes.h contains specific type definitions Contains generic type definition of
    types for different processors. Following shall be defined per platform (in configuration file) 
    in order to use the correct type:
    - dsPIC30F6014:     DSPIC30F6014
    
    When a platform is not supported in this file, the default definition is used.
 */

#ifndef STYPES_H
#define STYPES_H


/* Integer type definitions */
#if DSPIC30F6014
typedef char            int8_t;
typedef int             int16_t;
typedef long            int32_t;
typedef unsigned char   uint8_t;
typedef unsigned int    uint16_t;
typedef unsigned long   uint32_t;

/* Default types for standard GCC compiler */
#else
typedef signed char     int8_t;
typedef short           int16_t;
typedef int             int32_t;
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
#endif

/* Boolean type (needs conditional defines, because of possible conflict with other sources) */
typedef uint8_t         bool_t;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif
/* Limit defines per type */
#ifndef INT8_MAX
#define INT8_MAX        (127)
#endif 

#ifndef INT8_MIN
#define INT8_MIN        (-128)  
#endif 

#ifndef INT16_MAX
#define INT16_MAX       (32767)
#endif 

#ifndef INT16_MIN
#define INT16_MIN       (-32768)
#endif 

#ifndef INT32_MAX
#define INT32_MAX       (2147483647)
#endif 

#ifndef INT32_MIN
#define INT32_MIN       (-2147483648)  
#endif 

#ifndef UINT8_MAX
#define UINT8_MAX       (255)
#endif 

#ifndef UINT16_MAX
#define UINT16_MAX      (65535)  
#endif 

#ifndef UINT32_MAX
#define UINT32_MAX      (4294967295)
#endif 

/* define NULL pointer (ANSI C standard) */
#ifndef NULL
#define NULL            ((void *)0)
#endif


#endif
