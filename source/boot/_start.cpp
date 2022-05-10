#include "core_init.h"

extern "C" {
void _start(void);  // NOLINT(readability-redundant-declaration)
}

extern void program() __NO_RETURN;

__NO_RETURN void _start() {
    core_init();
    program();
    while (true) {
    }
}
