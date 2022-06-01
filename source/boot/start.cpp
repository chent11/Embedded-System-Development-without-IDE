#include "core.h"

extern "C" {
void _start(void);

extern void (*__preinit_array_start[])(void);
extern void (*__preinit_array_end[])(void);
extern void (*__init_array_start[])(void);
extern void (*__init_array_end[])(void);
__STATIC_FORCEINLINE void call_init_array(void) {
    // Iterate over all the preinit/init routines (mainly static or global constructors).
    uint32_t count = __preinit_array_end - __preinit_array_start;
    for (uint32_t i = 0; i < count; i++) {
        __preinit_array_start[i]();
    }

    count = __init_array_end - __init_array_start;
    for (uint32_t i = 0; i < count; i++) {
        __init_array_start[i]();
    }
}

extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
__STATIC_FORCEINLINE void clear_bss() {
    uint32_t* p = &__bss_start__;
    while (p < &__bss_end__) {
        *p++ = 0;
    }
}
}

extern void program() __NO_RETURN;
__NO_RETURN void _start() {
    clear_bss();
    core_init();
    // Calling static or global constructors after the core is initialized in c code could prevent some unexpected
    // problems from happening in hardware because some of the global constructors would initialize peripherals that
    // depend on the clock or other core configuration.
    call_init_array();
    program();
    while (true) {
    }
}
