#include <dynamic-array.h>

void* __dynamic_realloc(void* memory, size_t bytes_count) {
	void* new_memory = DYNAMIC_ARRAY_FUNCTION_MALLOC(bytes_count);
	if (!new_memory) return memory;
	for (size_t i = 0; i < bytes_count; i++) ((uint8_t*)new_memory)[i] = ((uint8_t*)memory)[i];
	return new_memory;
}

void* __dynamic_copy(void* destinaion, const void* source, size_t bytes_count) {
	const uint8_t* source8 = (const uint8_t*)source;
	uint8_t* destinaion8 = (uint8_t*)destinaion;
	for (size_t i = 0; i < bytes_count; i++) destinaion8[i] = source8[i];
	return destinaion;
}

bool __dynamic_compare(const void* first, const void* second, size_t bytes_count) {
	const uint8_t* first8 = (const uint8_t*)first;
	const uint8_t* second8 = (const uint8_t*)second;
	for (size_t i = 0; i < bytes_count; i++) if (first8[i] != second8[i]) return false;
	return true;
}