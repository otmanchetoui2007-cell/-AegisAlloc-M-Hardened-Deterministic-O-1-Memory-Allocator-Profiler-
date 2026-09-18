#ifndef PROFILER_H
#define PROFILER_H

#include <stdint.h>

/**
 *  Initialise le périphérique de profilage matériel (DWT sur ARM).
 */
void profiler_init(void);

/**
 *  Lit la valeur courante du compteur de cycles d'horloge CPU.
 *  Valeur du compteur (DWT_CYCCNT sur ARM, TSC sur x86).
 */
uint64_t profiler_get_cycles(void);

/**
 *  Sauvegarde l'état des interruptions et les désactive de manière atomique.
 *  État d'origine du masque d'interruption (registre PRIMASK).
 */
uint32_t profiler_irq_disable(void);

/**
 *  Restaure l'état d'origine des interruptions système.
 *  state État sauvegardé précédemment par profiler_irq_disable.
 */
void profiler_irq_restore(uint32_t state);

#endif // PROFILER_H