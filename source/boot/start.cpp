#include "core_init.h"

extern "C" {
void _start(void);

// These magic symbols are provided by the linker.
extern void (*__preinit_array_start[])(void) __WEAK;
extern void (*__preinit_array_end[])(void) __WEAK;
extern void (*__init_array_start[])(void) __WEAK;
extern void (*__init_array_end[])(void) __WEAK;
extern unsigned int __bss_start__;
extern unsigned int __bss_end__;

// Iterate over all the preinit/init routines (mainly static constructors).
static inline void init_array(void) {
    long count = __preinit_array_end - __preinit_array_start;
    for (long i = 0; i < count; i++) {
        __preinit_array_start[i]();
    }

    count = __init_array_end - __init_array_start;
    for (long i = 0; i < count; i++) {
        __init_array_start[i]();
    }
}

__STATIC_FORCEINLINE void init_bss() {
    unsigned int* p = &__bss_start__;
    while (p < &__bss_end__) {
        *p++ = 0;
    }
}
}

extern void program() __NO_RETURN;

__NO_RETURN void _start() {
    init_bss();
    init_array();
    core_init();
    program();
    while (true) {
    }
}
