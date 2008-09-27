#ifndef H_MEMORY
#define H_MEMORY

namespace Memory
{

void *MemAlignedAlloc(size_t size);
void MemAlignedFree(void *pt_align);

};

#endif