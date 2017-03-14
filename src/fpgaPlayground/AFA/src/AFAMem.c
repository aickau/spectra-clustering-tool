#include "AFAConfig.h"
#include "AFATypesCommon.h"


void *
AFAmalloc( uint64_t size )
{
#ifdef AFA_RUN_ON_VISUAL_STUDIO
    return malloc( size );
#else
    // creates memory areas on foreign board memory
    // ============================================

    static uint64_t offset = 0;
    uint64_t offsetNew = offset + size;
    if ( offsetNew < AFA_HW_MEMORY_SIZE )
    {
        uint64_t offsetReturn = offset;
        offset = offsetNew;
        return offsetReturn;
    }
    return NULL;
#endif
}

void
AFAmemset( void *addr, int value, uint64_t size )
{
#ifdef AFA_RUN_ON_VISUAL_STUDIO
    return memset(addr, value, size);
#else
    // clear area on foreign/remote hardware

    // TODO: JSC
#endif
}