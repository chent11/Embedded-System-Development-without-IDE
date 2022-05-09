#include "core_init.h"

extern "C" {
void _start(void);
}

extern void program() __attribute__((__noreturn__));

void __attribute__((__noreturn__)) _start() {
    core_init();
    program();
    while (true) {
    }
}
