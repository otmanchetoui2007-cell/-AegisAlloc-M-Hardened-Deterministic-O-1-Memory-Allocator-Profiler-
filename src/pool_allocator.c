#include "pool_allocator.h"

void pool_init(MemoryPool *pool) {
    if (pool == NULL) {
        return;
    }

    // Initialisation du bitmap : 0xFFFFFFFF marque les 32 blocs comme LIBRES
    pool->free_bitmap = 0xFFFFFFFFU;

    // Remise à zéro du buffer RAM
    for (size_t i = 0; i < (BLOCK_COUNT * BLOCK_SIZE); i++) {
        pool->buffer[i] = 0U;
    }
}

void *pool_alloc(MemoryPool *pool) {
    // 1. Vérification de saturation
    if (pool == NULL || pool->free_bitmap == 0U) {
        return NULL;
    }

    // 2. Recherche O(1) du premier bit à 1 via l'instruction CLZ
    uint32_t clz = (uint32_t)__builtin_clz(pool->free_bitmap);
    uint32_t block_index = clz; 

    // 3. Basculement du bit de 1 (LIBRE) à 0 (OCCUPÉ)
    uint32_t bit_pos = 31U - block_index;
    pool->free_bitmap &= ~(1U << bit_pos);

    // 4. Calcul de l'adresse RAM du bloc alloué
    uint8_t *block_ptr = &pool->buffer[block_index * BLOCK_SIZE];

    return (void *)block_ptr;
}
void pool_free(MemoryPool *pool, void *ptr) {
    // Garde-fou : vérification des pointeurs nuls
    if (pool == NULL || ptr == NULL) {
        return;
    }

    // Transtypage en pointeur d'octets pour permettre l'arithmétique sur les adresses
    uint8_t *byte_ptr = (uint8_t *)ptr;
    uint8_t *start_ptr = pool->buffer;
    uint8_t *end_ptr = pool->buffer + (BLOCK_COUNT * BLOCK_SIZE);

    // 1. Contrôle des limites (Bounds Check) : vérifie si l'adresse appartient bien au tampon
    if (byte_ptr < start_ptr || byte_ptr >= end_ptr) {
        return; // Adresse hors du pool -> opération ignorée
    }

    // 2. Calcul du décalage relatif en octets par rapport à la base
    ptrdiff_t offset = byte_ptr - start_ptr;

    // 3. Contrôle d'alignement : s'assure que l'adresse pointe exactement au DÉBUT d'un bloc
    if (offset % BLOCK_SIZE != 0) {
        return; // Adresse mal alignée (au milieu d'un bloc) -> opération ignorée
    }

    // 4. Conversion de l'offset en octets vers l'index numérique du bloc (0 à 31)
    uint32_t block_index = (uint32_t)(offset / BLOCK_SIZE);

    // 5. Basculement du bit de 0 (OCCUPÉ) vers 1 (LIBRE)
    uint32_t bit_pos = 31U - block_index;
    pool->free_bitmap |= (1U << bit_pos); // Application d'un masque OR pour forcer le bit à 1
}