#pragma once
#ifndef __TRACKED_ARRAY__
#define __TRACKED_ARRAY__

#include "dynamic-array.h"

/*
	$>DESCRIPTION
	The tracked array in memory looks like this:
		[tracked_array_data_t][tracked_array_element[0]|tracked_array_element[1]|...|tracked_array_element[n]]
	DESCRIPTION<$
*/

#define TRACKED_ARRAY_SIGNATURE															0x52415254		//	'TR'acked 'AR'ray -> 'TRAR'
#define TRACKED_ARRAY_ALLOC($type_name, $elements_count)								(($type_name*)__tracked_array_alloc(sizeof($type_name), $elements_count))
#define TRACKED_ARRAY_REALLOC($tracked_array_memory, $type_name, $elements_count)		(($type_name*)__tracked_array_realloc($tracked_array_memory, $elements_count))
#define TRACKED_ARRAY_FREE($tracked_array_memory)										__tracked_array_free($tracked_array_memory)
#define TRACKED_ARRAY_GET_LENGTH($tracked_array_memory)									__tracked_array_get_length($tracked_array_memory)
#define TRACKED_ARRAY_GET_ELEMENT_SIZE($tracked_array_memory)							__tracked_array_get_element_size($tracked_array_memory)
#define TRACKED_ARRAY_SPLIT($tracked_array_memory, $type_name, $index)					(($type_name*)__tracked_array_split($tracked_array_memory, $index))
#define TRACKED_ARRAY_INSERT($tracked_array_memory, $type_name, $element, $index)\
{\
	$type_name __ta__$tmp_value = $element;\
	$tracked_array_memory = ($type_name*)__tracked_array_insert($tracked_array_memory, &__ta__$tmp_value, $index);\
}
#define TRACKED_ARRAY_APPEND($tracked_array_memory, $type_name, $element)				TRACKED_ARRAY_INSERT($tracked_array_memory, $type_name, $element, TRACKED_ARRAY_GET_LENGTH($tracked_array_memory))
#define TRACKED_ARRAY_REMOVE($tracked_array_memory, $type_name, $index)					(($type_name*)__tracked_array_remove($tracked_array_memory, $index))
#define TRACKED_ARRAY_FIND($tracked_array_memory, $element)								(($type_name*)__tracked_array_find($tracked_array_memory, &$element))
#define TRACKED_ARRAY_BEGIN($tracked_array_memory, $type_name)							(($type_name)$tracked_array_memory)
#define TRACKED_ARRAY_END($tracked_array_memory, $type_name)							(($type_name)__tracked_array_end($tracked_array_memory))
#define TRACKED_ARRAY_ADDRESS_OF($tracked_array_memory, $index)							((size_t)__tracked_array_address_of($tracked_array_memory, $index))
#define TRACKED_ARRAY_REVERSE($tracked_array_memory, $type_name)						(($type_name)__tracked_array_reverse($tracked_array_memory))

typedef struct tracked_array_data_t {
	uint32_t	signature;
	size_t		array_length;
	size_t		element_size;
} tracked_array_data_t;

void* __tracked_array_alloc(size_t element_size, size_t elements_count);
void* __tracked_array_realloc(void* tracked_array_memory, size_t elements_count);
bool __tracked_array_free(void* tracked_array_memory);
size_t __tracked_array_get_length(void* tracked_array_memory);
size_t __tracked_array_get_element_size(void* tracked_array_memory);
void* __tracked_array_split(void* tracked_array_memory, size_t index);
void* __tracked_array_insert(void* tracked_array_memory, void* element, size_t index);
void* __tracked_array_remove(void* tracked_array_memory, size_t index);
size_t __tracked_array_find(void* tracked_array_memory, void* element);
void* __tracked_array_end(void* tracked_array_memory);
size_t __tracked_array_address_of(void* tracked_array_memory, size_t index);
void* __tracked_array_reverse(void* tracked_array_memory);

#endif