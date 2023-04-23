#include <tracked-array.h>

void* __tracked_array_alloc(size_t element_size, size_t elements_count) {
	tracked_array_data_t* tracked_array_data = (tracked_array_data_t*)DYNAMIC_ARRAY_FUNCTION_MALLOC(sizeof(tracked_array_data_t) + element_size * elements_count);
	if (!tracked_array_data) return false;

	tracked_array_data->signature = TRACKED_ARRAY_SIGNATURE;
	tracked_array_data->array_length = elements_count;
	tracked_array_data->element_size = element_size;

	return (void*)((size_t)tracked_array_data + sizeof(tracked_array_data_t));
}

void* __tracked_array_realloc(void* tracked_array_memory, size_t elements_count) {
	tracked_array_data_t* old_tracked_array_data = (tracked_array_data_t*)((size_t)tracked_array_memory - sizeof(tracked_array_data_t));
	if (old_tracked_array_data->signature != TRACKED_ARRAY_SIGNATURE) return false;

	void* new_tracked_array_memory = __tracked_array_alloc(old_tracked_array_data->element_size, elements_count);
	if (!new_tracked_array_memory) return false;

	size_t element_offset = 0;
	for (size_t i = 0; i < old_tracked_array_data->array_length; i++) {
		DYNAMIC_MEMCPY((void*)((size_t)new_tracked_array_memory + element_offset), (void*)((size_t)tracked_array_memory + element_offset), old_tracked_array_data->element_size);
		element_offset += old_tracked_array_data->element_size;
	}

	return new_tracked_array_memory;
}

bool __tracked_array_free(void* tracked_array_memory) {
	tracked_array_data_t* tracked_array_data = (tracked_array_data_t*)((size_t)tracked_array_memory - sizeof(tracked_array_data_t));
	if (tracked_array_data->signature != TRACKED_ARRAY_SIGNATURE) return false;

	DYNAMIC_FREE(tracked_array_data);

	return true;
}

size_t __tracked_array_get_length(void* tracked_array_memory) {
	tracked_array_data_t* tracked_array_data = (tracked_array_data_t*)((size_t)tracked_array_memory - sizeof(tracked_array_data_t));
	if (tracked_array_data->signature != TRACKED_ARRAY_SIGNATURE) return INCORRECT_INDEX;

	return tracked_array_data->array_length;
}

size_t __tracked_array_get_element_size(void* tracked_array_memory) {
	tracked_array_data_t* tracked_array_data = (tracked_array_data_t*)((size_t)tracked_array_memory - sizeof(tracked_array_data_t));
	if (tracked_array_data->signature != TRACKED_ARRAY_SIGNATURE) return INCORRECT_INDEX;

	return tracked_array_data->element_size;
}

void* __tracked_array_split(void* tracked_array_memory, size_t index) {
	tracked_array_data_t* tracked_array_data = (tracked_array_data_t*)((size_t)tracked_array_memory - sizeof(tracked_array_data_t));
	if (tracked_array_data->signature != TRACKED_ARRAY_SIGNATURE) return false;

	if (index > tracked_array_data->array_length) {
		void* tmp_array = __tracked_array_realloc(tracked_array_memory, index);
		if(!tmp_array) return false;

		tracked_array_memory = tmp_array;
		tracked_array_data = (tracked_array_data_t*)((size_t)tracked_array_memory - sizeof(tracked_array_data_t));
	}

	void* new_tracked_array_memory = __tracked_array_alloc(tracked_array_data->element_size, tracked_array_data->array_length + 1);
	if (!new_tracked_array_memory) return false;

	size_t i = 0, old_element_address = (size_t)tracked_array_memory, new_element_address = (size_t)new_tracked_array_memory;
	for (; i < index; i++) {
		DYNAMIC_MEMCPY((void*)new_element_address, (void*)old_element_address, tracked_array_data->element_size);
		new_element_address += tracked_array_data->element_size;
		old_element_address += tracked_array_data->element_size;
	}

	++i;
	new_element_address += tracked_array_data->element_size;
	for (; i < tracked_array_data->array_length + 1; i++) {
		DYNAMIC_MEMCPY((void*)new_element_address, (void*)old_element_address, tracked_array_data->element_size);
		new_element_address += tracked_array_data->element_size;
		old_element_address += tracked_array_data->element_size;
	}

	return new_tracked_array_memory;
}

void* __tracked_array_insert(void* tracked_array_memory, void* element, size_t index) {
	size_t element_size = __tracked_array_get_element_size(tracked_array_memory);
	if (element_size == INCORRECT_INDEX) return false;

	tracked_array_memory = __tracked_array_split(tracked_array_memory, index);
	DYNAMIC_MEMCPY((void*)TRACKED_ARRAY_ADDRESS_OF(tracked_array_memory, index), element, element_size);

	return tracked_array_memory;
}

void* __tracked_array_remove(void* tracked_array_memory, size_t index) {
	tracked_array_data_t* tracked_array_data = (tracked_array_data_t*)((size_t)tracked_array_memory - sizeof(tracked_array_data_t));
	if (tracked_array_data->signature != TRACKED_ARRAY_SIGNATURE || index >= tracked_array_data->array_length) return false;

	void* new_tracked_array_memory = __tracked_array_alloc(tracked_array_data->element_size, tracked_array_data->array_length - 1);
	if (!new_tracked_array_memory) return false;

	size_t i = 0, old_element_address = (size_t)tracked_array_memory, new_element_address = (size_t)new_tracked_array_memory;
	for (; i < index; i++) {
		DYNAMIC_MEMCPY((void*)new_element_address, (void*)old_element_address, tracked_array_data->element_size);
		new_element_address += tracked_array_data->element_size;
		old_element_address += tracked_array_data->element_size;
	}

	old_element_address += tracked_array_data->element_size;
	for (; i < tracked_array_data->array_length - 1; i++) {
		DYNAMIC_MEMCPY((void*)new_element_address, (void*)old_element_address, tracked_array_data->element_size);
		new_element_address += tracked_array_data->element_size;
		old_element_address += tracked_array_data->element_size;
	}

	return new_tracked_array_memory;
}

size_t __tracked_array_find(void* tracked_array_memory, void* element) {
	tracked_array_data_t* tracked_array_data = (tracked_array_data_t*)((size_t)tracked_array_memory - sizeof(tracked_array_data_t));
	if (tracked_array_data->signature != TRACKED_ARRAY_SIGNATURE) return INCORRECT_INDEX;

	size_t element_offset = 0;
	for (size_t i = 0; i < tracked_array_data->array_length; i++) {
		if (DYNAMIC_MEMCMP((void*)((size_t)tracked_array_memory + element_offset), element,  tracked_array_data->element_size)) return i;
		element_offset += tracked_array_data->element_size;
	}

	return INCORRECT_INDEX;
}

void* __tracked_array_end(void* tracked_array_memory) {
	tracked_array_data_t* tracked_array_data = (tracked_array_data_t*)((size_t)tracked_array_memory - sizeof(tracked_array_data_t));
	if (tracked_array_data->signature != TRACKED_ARRAY_SIGNATURE) return false;

	return (void*)((size_t)tracked_array_memory + tracked_array_data->array_length * tracked_array_data->element_size);
}

size_t __tracked_array_address_of(void* tracked_array_memory, size_t index) {
	tracked_array_data_t* tracked_array_data = (tracked_array_data_t*)((size_t)tracked_array_memory - sizeof(tracked_array_data_t));
	if (tracked_array_data->signature != TRACKED_ARRAY_SIGNATURE || index >= tracked_array_data->array_length) return false;

	return (size_t)tracked_array_memory + index * tracked_array_data->element_size;
}

void* __tracked_array_reverse(void* tracked_array_memory) {
	tracked_array_data_t* tracked_array_data = (tracked_array_data_t*)((size_t)tracked_array_memory - sizeof(tracked_array_data_t));
	if (tracked_array_data->signature != TRACKED_ARRAY_SIGNATURE) return false;

	void* new_tracked_array_memory = __tracked_array_alloc(tracked_array_data->element_size, tracked_array_data->array_length);
	if (!new_tracked_array_memory) return false;

	size_t destination_address = (size_t)new_tracked_array_memory, source_address = (size_t)__tracked_array_end(tracked_array_memory) - tracked_array_data->element_size;
	for (size_t i = 0; i < tracked_array_data->array_length; i++) {
		DYNAMIC_MEMCPY((void*)destination_address, (void*)source_address, tracked_array_data->element_size);
		destination_address += tracked_array_data->element_size;
		source_address -= tracked_array_data->element_size;
	}

	return new_tracked_array_memory;
}

void* __tracked_array_concat(void* first_tracked_array_memory, void* second_tracked_array_memory) {
	tracked_array_data_t* first_tracked_array_data = (tracked_array_data_t*)((size_t)first_tracked_array_memory - sizeof(tracked_array_data_t));
	tracked_array_data_t* second_tracked_array_data = (tracked_array_data_t*)((size_t)second_tracked_array_memory - sizeof(tracked_array_data_t));
	if (first_tracked_array_data->signature != TRACKED_ARRAY_SIGNATURE || second_tracked_array_data->signature != TRACKED_ARRAY_SIGNATURE) return false;

	void* new_tracked_array_memory = __tracked_array_alloc(first_tracked_array_data->element_size, first_tracked_array_data->array_length + second_tracked_array_data->array_length);
	if (!new_tracked_array_memory) return false;

	size_t old_element_address = (size_t)first_tracked_array_memory;
	size_t new_element_address = (size_t)new_tracked_array_memory;
	for (size_t i = 0; i < first_tracked_array_data->array_length; i++) {
		DYNAMIC_MEMCPY((void*)new_element_address, (void*)old_element_address, first_tracked_array_data->element_size);
		old_element_address += first_tracked_array_data->element_size;
		new_element_address += first_tracked_array_data->element_size;
	}

	old_element_address = (size_t)second_tracked_array_memory;
	for (size_t i = 0; i < first_tracked_array_data->array_length; i++) {
		DYNAMIC_MEMCPY((void*)new_element_address, (void*)old_element_address, first_tracked_array_data->element_size);
		old_element_address += first_tracked_array_data->element_size;
		new_element_address += first_tracked_array_data->element_size;
	}

	return new_tracked_array_memory;
}