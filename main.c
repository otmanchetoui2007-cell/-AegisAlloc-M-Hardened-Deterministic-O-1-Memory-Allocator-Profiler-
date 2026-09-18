#define __USE_MINGW_ANSI_STDIO 1 // Active la conformité C99 pour printf sous MinGW/Windows
#include <stdio.h>
#include <inttypes.h>
#include "pool_allocator.h"
#include "profiler.h"

int main(void) {
    MemoryPool pool;
    
    pool_init(&pool);
    profiler_init();

    printf("=== BENCHMARK DE L'ALLOCATEUR MEMOIRE O(1) ===\n\n");

    // 1. Test pool_alloc()
    uint32_t irq = profiler_irq_disable();
    uint64_t start = profiler_get_cycles();

    void *ptr = pool_alloc(&pool);

    uint64_t end = profiler_get_cycles();
    profiler_irq_restore(irq);

    uint64_t cycles_alloc = end - start;
    printf("[ALLOC] Adresse obtenue : %p | Coût : %" PRIu64 " cycles CPU\n", 
           ptr, cycles_alloc);

    // 2. Test pool_free()
    if (ptr != NULL) {
        irq = profiler_irq_disable();
        start = profiler_get_cycles();

        pool_free(&pool, ptr);

        end = profiler_get_cycles();
        profiler_irq_restore(irq);

        uint64_t cycles_free = end - start;
        printf("[FREE]  Statut : Nettoyé & Libéré | Coût : %" PRIu64 " cycles CPU\n", 
               cycles_free);
    }

    return 0;
}