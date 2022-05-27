#include "core.h"

extern "C" {
void _start(void);

// These magic symbols are provided by the linker.
extern void (*__preinit_array_start[])(void) __WEAK;
extern void (*__preinit_array_end[])(void) __WEAK;
extern void (*__init_array_start[])(void) __WEAK;
extern void (*__init_array_end[])(void) __WEAK;
extern unsigned int __bss_start__;
extern unsigned int __bss_end__;

// Iterate over all the preinit/init routines (mainly static or global constructors).
__STATIC_FORCEINLINE void call_init_array(void) {
    uint32_t count = __preinit_array_end - __preinit_array_start;
    for (uint32_t i = 0; i < count; i++) {
        __preinit_array_start[i]();
    }

    count = __init_array_end - __init_array_start;
    for (uint32_t i = 0; i < count; i++) {
        __init_array_start[i]();
    }
}

__STATIC_FORCEINLINE void clear_bss() {
    unsigned int* p = &__bss_start__;
    while (p < &__bss_end__) {
        *p++ = 0;
    }
}
}

extern void program() __NO_RETURN;
__NO_RETURN void _start() {
    clear_bss();
    core_init();
    call_init_array();  // call static or global constructors after core initialized
    program();
    while (true) {
    }
}
