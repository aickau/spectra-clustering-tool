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
//! \file  memory.h
//! \brief 16 bytes aligned memory allocation on heap.

#ifndef H_MEMORY
#define H_MEMORY

#include "defines.h"

namespace Memory
{

	void *memAlignedAlloc(size_t size);
	void memAlignedFree(void *pt_align);

};

#endif