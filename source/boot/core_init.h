#ifndef CORE_INIT_H_
#define CORE_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>  // NOLINT(hicpp-deprecated-headers,modernize-deprecated-headers)

#include "cmsis_compiler.h"

void core_init(void);
void delay_ms(uint32_t time);

void send_char_blocking(char ch);
char get_char_blocking(void);

#ifdef NEED_ASSERT_FUNC
void __assert_func(const char* file,
                   int line,
                   const char* func,
                   const char* failedexpr) {
    while (1) {
    }
}
#define assert_param(x) ((x) ? (void)0 : __assert_func(__FILE__, __LINE__, (char*)0, ""))
#else
#define assert_param(x) ((void)0)  // NOLINT(cppcoreguidelines-macro-usage)
#endif

#ifdef __cplusplus
}
#endif

#endif
