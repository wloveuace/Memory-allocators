#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POOL_SIZE 1024

size_t offset;
int nHeaders = 10;
void* basePtr;

typedef struct {
	void* dataPtr;
	size_t dataSz;
}Header;

void allignHeader(uintptr_t* headerPtr) {
	int rem = *headerPtr % 8;
	if (rem != 0) {
		*headerPtr += 8 - rem;
		printf("Padding: %d\n", 8 - rem);
	}
}

void* stackAlloc(size_t size) {
	if (size + offset > POOL_SIZE || !nHeaders) return NULL;

	uintptr_t headerPtr = (uintptr_t)basePtr + offset;
	allignHeader(&headerPtr);
	printf("Aligned header: 0x%lu\n", headerPtr);

	Header* header = (Header*)headerPtr;
	header->dataPtr = header + 1;
	header->dataSz = size;

	offset += sizeof(Header) + size;

	nHeaders--;
	return header->dataPtr;
}

unsigned int initStack() {
	void* stack = malloc(sizeof(Header) * 10 + POOL_SIZE);
	if (!stack) return 0;

	basePtr = stack;

	return 1;
}

int main() {
	
	initStack();
	printf("Stack ptr; 0x%lu\n", basePtr);
	void* allocated = stackAlloc(7);
	printf("Allocation ptr; 0x%lu\n", allocated);
	printf("==================\n\n");

	void* allocated2 = stackAlloc(7);
	printf("Allocation ptr; 0x%lu\n", allocated2);
	printf("==================\n\n");
}
