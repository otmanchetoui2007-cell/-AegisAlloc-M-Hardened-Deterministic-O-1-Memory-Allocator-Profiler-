#include "profiler.h"

#if defined(__arm__) || defined(__thumb__)
// ============================================================================
// ARCHITECTURE ARM CORTEX-M (Matériel réel / Émulateur QEMU)
// ============================================================================

// Registre de contrôle de débogage (bit 24 = TRCENA pour activer le DWT)
#define COREDEBUG_DEMCR   (*((volatile uint32_t *)0xE000EDFCU))

// Registre de contrôle du bloc DWT (bit 0 = CYCCNTENA pour lancer le compteur)
#define DWT_CTRL          (*((volatile uint32_t *)0xE0001000U))

// Registre de compteur de cycles CPU (s'incrémente à chaque impulsion d'horloge)
#define DWT_CYCCNT        (*((volatile uint32_t *)0xE0001004U))

void profiler_init(void) {
    COREDEBUG_DEMCR |= (1U << 24); // Activer l'accès aux périphériques de traçage
    DWT_CYCCNT = 0U;               // Réinitialiser le compteur à zéro
    DWT_CTRL |= 1U;                // Démarrer le comptage des cycles
}

uint64_t profiler_get_cycles(void) {
    // Cast direct du registre matériel 32 bits
    return (uint64_t)DWT_CYCCNT;
}

uint32_t profiler_irq_disable(void) {
    uint32_t primask;
    // Assembleur Inline ARM :
    // 1. "mrs" : lit le registre d'état PRIMASK dans la variable primask
    // 2. "cpsid i" : désactive immédiatement toutes les interruptions masquables
    __asm volatile (
        "mrs %0, primask \n"
        "cpsid i         \n"
        : "=r" (primask)
        :: "memory"
    );
    return primask;
}

void profiler_irq_restore(uint32_t state) {
    // Assembleur Inline ARM :
    // "msr" : réinjecte la valeur sauvegardée dans le registre PRIMASK
    __asm volatile (
        "msr primask, %0 \n"
        :: "r" (state)
        : "memory"
    );
}

#else
// ============================================================================
// ARCHITECTURE x86 / SIMULATION PC (Compilateur GCC / Clang)
// ============================================================================

#include <x86intrin.h> // Fournit l'instruction __rdtsc() pour le processeur x86

void profiler_init(void) {
    // Pas d'initialisation requise pour le registre TSC sur PC
}

uint64_t profiler_get_cycles(void) {
    // Lit le registre Time Stamp Counter (TSC) du processeur x86
    return __rdtsc();
}

uint32_t profiler_irq_disable(void) {
    // Simulation : les interruptions ne sont pas coupables depuis l'espace utilisateur sur PC
    return 0; 
}

void profiler_irq_restore(uint32_t state) {
    // Empêche le warning du compilateur concernant la variable non utilisée
    (void)state; 
}
#endif