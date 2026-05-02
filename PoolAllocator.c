#include <stdio.h>
#include <stdlib.h>

#define POOL_SIZE 1024
#define NO_CHUNKS 64

typedef struct {
	void* next;
	void* dataPtr;
	size_t chunkSz;
	int index;
}Header;

Header* freeNode = NULL;

void initFreeList() {
	int chunkSize = POOL_SIZE / NO_CHUNKS; 

	Header* pool = (Header*)malloc(POOL_SIZE + sizeof(Header) * NO_CHUNKS);

	for (int i = 0, j = 1; i < NO_CHUNKS; i++, j++) {
		Header* header = (Header*)((uintptr_t)pool + (i * sizeof(Header)) + (i * chunkSize));
		header->dataPtr = header + 1;
		header->chunkSz = chunkSize;
		header->index = i;

		if (i == NO_CHUNKS - 1) { 
			header->next = NULL;
			break;
		};

		header->next = (void*)((uintptr_t)pool + (j * sizeof(Header)) + (j * chunkSize) );
	}

	freeNode = (Header*)pool;
}

void* stackAlloc() {
	if (!freeNode) return NULL;
	Header* header = freeNode;

	printf("Allocated memory chunk id: %d\n\t Node ptr: 0x%lu\n\t Data ptr 0x%lu\n\t Chunck Size: %d\n",
		header->index, header, header->dataPtr, header->chunkSz);

	void* dataPtr = header->dataPtr;

	freeNode = (Header*)freeNode->next;

	return dataPtr;
}

void stackFree(void* pData) {

	Header* header = (Header*)((uintptr_t)(pData)-sizeof(Header));

	if (!header) return;

	printf("Header ptr: 0x%lu\n", header);
	freeNode = (Header*)header;
	printf("Free node: 0x%lu\n", freeNode);

	return;
}

int main() {
	initFreeList();

	void* chunk = stackAlloc();
	stackFree(chunk);


}
