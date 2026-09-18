#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H
 #include <stdint.h>
#include <stddef.h>
// Dimensions du pool mémoire
#define BLOCK_SIZE   64U  // Taille fixe de chaque bloc en octets
#define BLOCK_COUNT  32U  // Nombre total de blocs (32 bits = 32 blocs)

// Structure de données du pool
typedef struct {
    uint32_t free_bitmap; // Register bitmap (1 = Libre, 0 = Occupé)
    uint8_t buffer[BLOCK_COUNT * BLOCK_SIZE] __attribute__((aligned(8)));//Elle demande au compilateur de placer buffer sur une adresse alignée sur 8 octets.//
} MemoryPool;

// Prototype de la fonction d'initialisation
void pool_init(MemoryPool *pool);
// Prototype de la fonction d'allocation O(1)
void *pool_alloc(MemoryPool *pool);
/**
 *Libère un bloc mémoire et le réintroduit dans le pool en O(1).
 *pool Pointeur vers l'instance du pool.
 * ptr Pointeur vers le bloc à restituer.
 */
void pool_free(MemoryPool *pool, void *ptr);

#endif // POOL_ALLOCATOR_H