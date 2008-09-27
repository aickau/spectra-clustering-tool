#include "memory.h"

#define WIN32_LEAN_AND_MEAN		
#include <windows.h>

#include <assert.h>

namespace Memory
{

void **hash_table=0;
size_t hash_size = 1024;

static
void GrowHash()
{
	void **hash_table_new;
	hash_size *= 2;
	hash_table_new = (void**) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, hash_size*4);
	memcpy(hash_table_new, hash_table, hash_size*2);
	HeapFree(GetProcessHeap(), 0, hash_table);
	hash_table = hash_table_new;
}


void *MemAlignedAlloc(size_t size)
{
	void	*pt_align, *pt_unalign;
	size_t		pt_offset, hash_index, hash_probe;

	if(hash_table==NULL){
		hash_table = (void**) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, hash_size*4);
	}

	pt_unalign	= (void*) HeapAlloc(GetProcessHeap(), 0, size+32);
	pt_offset	= (size_t) pt_unalign % 32;
	pt_align	= (char *)pt_unalign + pt_offset;
	hash_probe	= 0;

	do	{
		hash_index	= ((((size_t) pt_align) >> 5) + hash_probe) % hash_size;
		hash_probe++;
	
		if (hash_probe > hash_size) {
			GrowHash();
		}

	} while (hash_table[hash_index] != 0);

	hash_table[hash_index]	= pt_unalign;

	return(pt_align);
}


void MemAlignedFree(void *pt_align)
{
	if (pt_align == NULL)
		return;
	
	void	*pt_unalign;
	size_t		hash_index, hash_probe, pt_delta;

	hash_probe = 0;

	do{
		hash_index	= ((((size_t) pt_align) >> 5) + hash_probe) % hash_size;
		pt_unalign	= hash_table[hash_index];

		hash_probe++;

		if (hash_probe > hash_size) {
			assert(0);
		}

		pt_delta = (size_t)pt_align - (size_t)pt_unalign;
	} while ((pt_delta >= 32) || (pt_delta<0));


	hash_table[hash_index]	= 0;

	if(pt_unalign != 0)	{
		HeapFree(GetProcessHeap(), 0, pt_unalign);
	}
}
	

}