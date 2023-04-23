#pragma once
#ifndef __TYPEDEFS__
#define __TYPEDEFS__

#define INCORRECT_INDEX				((size_t)-1)

#if defined(__clang__) || defined(__GNUC__)
typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __SIZE_TYPE__ size_t;
#elif defined(_MSC_VER)
typedef __EDG_PTRDIFF_TYPE__ ptrdiff_t;
typedef __EDG_SIZE_TYPE__ size_t;
#else
typedef long signed int ptrdiff_t;
typedef long unsigned int size_t;
#endif

#ifndef __cplusplus
#ifndef bool
#define bool	uint8_t
#endif
#ifndef false
#define false	0
#endif
#ifndef true
#define true	1
#endif
#endif

#if !defined(INT8_MAX) && !defined(__int8_t_defined)
typedef signed char int8_t;
//	#define __int8_t_defined
#define INT8_MIN	(-128)
#define INT8_MAX	(127)
#endif
#if !defined(INT16_MAX) && !defined(__int16_t_defined)
typedef signed short int16_t;
//	#define __int16_t_defined
#define INT16_MIN	(-32767-1)
#define INT16_MAX	(32767)
#endif
#if !defined(INT32_MAX) && !defined(__int32_t_defined)
typedef signed int int32_t;
//	#define __int32_t_defined
#define INT32_MIN	(-2147483647-1)
#define INT32_MAX	(2147483647)
#endif

#if !defined(UINT8_MAX) && !defined(__uint8_t_defined)
typedef unsigned char uint8_t;
//	#define __uint8_t_defined
#define UINT8_MIN	(0)
#define UINT8_MAX	(255)
#endif
#if !defined(UINT16_MAX) && !defined(__uint16_t_defined)
typedef unsigned short uint16_t;
//	#define __uint16_t_defined
#define UINT16_MIN	(0)
#define UINT16_MAX	(65535)
#endif
#if !defined(UINT32_MAX) && !defined(__uint32_t_defined)
typedef unsigned int uint32_t;
//	#define __uint32_t_defined
#define UINT32_MIN	(0)
#define UINT32_MAX	(4294967295)
#endif

#if !defined(INT64_MAX) && !defined(__int64_t_defined)
#ifdef __GNUC__
#if __WORDSIZE == 64
typedef signed long int64_t;
#define INT64_MIN	(-9223372036854775807-1)
#define INT64_MAX	(9223372036854775807)
#else
__extension__ typedef signed long int __int64_t;
#define INT64_MIN	(-9223372036854775807-1)
#define INT64_MAX	(9223372036854775807)
#endif
#else
typedef signed long long int64_t;
#define INT64_MIN	(-9223372036854775807-1)
#define INT64_MAX	(9223372036854775807)
#endif
#endif

#if !defined(UINT64_MAX) && !defined(__uint64_t_defined)
#ifdef __GNUC__
#if __WORDSIZE == 64
typedef unsigned long uint64_t;
#define UINT64_MIN	(0)
#define UINT64_MAX	(18446744073709551615)
#else
__extension__ typedef unsigned long int __uint64_t;
#define UINT64_MIN	(0)
#define UINT64_MAX	(18446744073709551615)
#endif
#else
typedef unsigned long long uint64_t;
#define UINT64_MIN	(0)
#define UINT64_MAX	(18446744073709551615)
#endif
#endif


#endif