#ifndef CORE_INIT_H_
#define CORE_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h> // NOLINT(hicpp-deprecated-headers,modernize-deprecated-headers)

void core_init(void);
void delay_ms(uint32_t time);

#ifdef __cplusplus
}
#endif

#endif
