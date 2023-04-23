#include <stdio.h>
#include <tracked-array.h>

int main() {
	uint8_t* bytes = TRACKED_ARRAY_ALLOC(uint8_t, 10);
	uint8_t* nbytes = TRACKED_ARRAY_ALLOC(uint8_t, 10);
	for (uint8_t i = 0; i < 10; i++) bytes[i] = '0' + i;
	for (uint8_t i = 0; i < 10; i++) nbytes[i] = 'A' + i;

	uint8_t* concatenated = TRACKED_ARRAY_CONCAT(nbytes, bytes, uint8_t);
	for (size_t i = 0; i < TRACKED_ARRAY_GET_LENGTH(concatenated); i++) putchar(concatenated[i]);

	printf("\n");

	uint8_t* reversed = TRACKED_ARRAY_REVERSE(concatenated, uint8_t);
	for (size_t i = 0; i < TRACKED_ARRAY_GET_LENGTH(reversed); i++) putchar(reversed[i]);

	printf("\n");

	TRACKED_ARRAY_FREE(reversed);
	TRACKED_ARRAY_FREE(concatenated);
	TRACKED_ARRAY_FREE(nbytes);
	TRACKED_ARRAY_FREE(bytes);
}