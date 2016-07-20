//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 1/19/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  memory.cpp
//! \brief 16 bytes aligned memory allocation on heap.

#include "memory.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

namespace Memory
{

static void **hash_table=0;
static size_t hash_size = 1024;

static
void GrowHash()
{
	void **hash_table_new;
	hash_size *= 2;
	hash_table_new = (void**) calloc(hash_size,8);
	memcpy(hash_table_new, hash_table, hash_size*2);
	free(hash_table);
	hash_table = hash_table_new;
}


void *memAlignedAlloc(size_t size)
{
	void	*pt_align, *pt_unalign;
	size_t		pt_offset, hash_index, hash_probe;

	if(hash_table==NULL){
		hash_table = (void**) (void**) calloc(hash_size,8);
	}

	pt_unalign	= calloc(size+32,1);
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


void memAlignedFree(void *pt_align)
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
		free(pt_unalign);
	}
}
	

}