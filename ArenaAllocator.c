#include <stdio.h>
#include <stdlib.h>

#define POOL_SIZE 1024

typedef struct allocator {
	void* basePtr;
	size_t offset;
}Arena;

typedef enum {
	MEM_ALLOC,
	MEM_DEALLOC
}Operation;

Arena* arenaIntial() {
	Arena* arena = (Arena*)malloc(sizeof(arena) + POOL_SIZE);
	if (!arena) {
		return NULL;
	}

	arena->basePtr = arena + 1;
	arena->offset = 0;

	return arena;
}

void arenaAlign(size_t* allocSize, Operation op) {
	int rem = *allocSize % 8;

	switch (op) {

	case MEM_ALLOC:
		if (rem != 0) {
			*allocSize += 8 - rem;
			printf("Padding: %d\n", 8 - rem);
		}
		break;

	case MEM_DEALLOC:
		break;

	default:
		break;
	}

	return;
}

void arenaAlloc(Arena* arena, size_t allocSize) {
	arenaAlign(&allocSize, MEM_ALLOC);

	if (allocSize > POOL_SIZE || POOL_SIZE - arena->offset < allocSize) {
		printf("Invalid size");
		return;
	}

	arena->offset += allocSize;
	printf("Allocated memory size: 0x%lu\n", allocSize);
	printf("Free ptr: 0x%lu\n", (uintptr_t)arena->basePtr + arena->offset);

	return;
}

void arenaDealloc(Arena* arena, size_t allocSize) {
	if (allocSize > arena->offset) {
		printf("Invalid size");
		return;
	}

	arena->offset -= allocSize;
	printf("Deallocated memory size: 0x%lu\n", allocSize);
	printf("Free ptr: 0x%lu\n", (uintptr_t)arena->basePtr + arena->offset);

	return;
}

int main() {
	Arena* arena = arenaIntial();
	if (!arena) return 1;

	printf("Base ptr: 0x%lu\n", arena->basePtr);

	arenaAlloc(arena, 7);
	arenaDealloc(arena, 6);

	return 0;
}
