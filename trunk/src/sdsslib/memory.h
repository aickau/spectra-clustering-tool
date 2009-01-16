#ifndef H_MEMORY
#define H_MEMORY

namespace Memory
{

	void *memAlignedAlloc(size_t size);
	void memAlignedFree(void *pt_align);

};

#endif