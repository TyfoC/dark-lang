#include <stdio.h>
#include <tracked-array.h>

int main() {
	int* ints = TRACKED_ARRAY_ALLOC(int, 1);

	for (size_t i = 1; i < 5; i++) {
		printf("Ints array(%lu):\n", TRACKED_ARRAY_GET_LENGTH(ints));
		for (size_t j = 0; j < i; j++) {
			printf("Element #%lu = %d\n", j, ints[j]);
		}

		TRACKED_ARRAY_APPEND(ints, int, i)
	}

	int value = 9999;
	
	TRACKED_ARRAY_INSERT(ints, int, value, 3);
	for (size_t j = 0; j < TRACKED_ARRAY_GET_LENGTH(ints); j++) printf("Element #%lu = %d\n", j, ints[j]);

	TRACKED_ARRAY_FREE(ints);
}