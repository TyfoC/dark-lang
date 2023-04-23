#pragma once
#ifndef __DYNAMIC_ARRAY__
#define __DYNAMIC_ARRAY__

#include "typedefs.h"

/*
	$>IMPORTANT
	This file require functions to allocate and free memory.
	Before including this file, you can define them using macros. Function prototypes must match these patterns:
		malloc:
			#define DYNAMIC_ARRAY_FUNCTION_MALLOC NAME_MALLOC
			void* NAME_MALLOC(size_t bytes_count);
		free:
			#define DYNAMIC_ARRAY_FUNCTION_FREE NAME_FREE
			void NAME_FREE(void* memory);
		memcpy:
			#define DYNAMIC_ARRAY_FUNCTION_MEMCPY NAME_MEMCPY
			void* NAME_MEMCPY(void* destination, const void* source, size_t bytes_count);
		memcmp:
			#define DYNAMIC_ARRAY_FUNCTION_MEMCMP NAME_MEMCMP
			bool NAME_MEMCMP(const void* first, const void* second, size_t bytes_count);
	Otherwise, functions from stdlib.h will be used (except DYNAMIC_ARRAY_FUNCTION_MEMCPY, DYNAMIC_ARRAY_FUNCTION_MEMCMP).
	IMPORTANT<$
*/

#ifndef DYNAMIC_ARRAY_FUNCTION_MALLOC
#include <stdlib.h>
#define DYNAMIC_ARRAY_FUNCTION_MALLOC	malloc
#endif
#ifndef DYNAMIC_ARRAY_FUNCTION_FREE
#include <stdlib.h>
#define DYNAMIC_ARRAY_FUNCTION_FREE		free
#endif
#ifndef DYNAMIC_ARRAY_FUNCTION_MEMCPY
#define DYNAMIC_ARRAY_FUNCTION_MEMCPY	__dynamic_copy
#endif
#ifndef DYNAMIC_ARRAY_FUNCTION_MEMCMP
#define DYNAMIC_ARRAY_FUNCTION_MEMCMP	__dynamic_compare
#endif

#define DYNAMIC_MALLOC($type_name, $elements_count)				(($type_name*)DYNAMIC_ARRAY_FUNCTION_MALLOC(sizeof($type_name) * $elements_count))
#define DYNAMIC_REALLOC($memory, $type_name, $elements_count)	(($type_name)__dynamic_realloc($memory, sizeof($type_name) * $elements_count))
#define DYNAMIC_FREE($memory)									DYNAMIC_ARRAY_FUNCTION_FREE($memory)
#define DYNAMIC_MEMCPY($destination, $source, $bytes_count)		DYNAMIC_ARRAY_FUNCTION_MEMCPY($destination, $source, $bytes_count)
#define DYNAMIC_MEMCMP($first, $second, $bytes_count)			DYNAMIC_ARRAY_FUNCTION_MEMCMP($first, $second, $bytes_count)

void* __dynamic_realloc(void* memory, size_t bytes_count);
void* __dynamic_copy(void* destinaion, const void* source, size_t bytes_count);
bool __dynamic_compare(const void* first, const void* second, size_t bytes_count);

#endif