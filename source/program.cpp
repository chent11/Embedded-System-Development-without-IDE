#include "Leds.hh"
#include "Utils.hh"
#include "boot/cmsis_startup/system_ARMCM4.h"
#include "core_init.h"
#include "fmt/format.h"

const uint32_t BOOT_LED_BLINK_TIMES = 10;
const uint32_t BOOT_LED_BLINK_DELAY = 100;
const uint32_t TOGGLE_DELAY = 1000;

extern "C" {
int _read(int file, char* ptr, int len) {
    file = file;
    len = len;
    *ptr = get_char_blocking();
    return (1);
}

int _write(int file, char* ptr, int len) {
    file = file;
    for (int i = 0; i < len; i++) {
        send_char_blocking(*ptr++);
    }
    return len;
}
}

__NO_RETURN void program() noexcept {
    const auto& ledGreen = LedGreen::getInstance();
    const auto& ledRed = LedRed::getInstance();

    for (uint32_t i = 0; i < BOOT_LED_BLINK_TIMES; i++) {
        ledRed.toggle();
        delay_ms(BOOT_LED_BLINK_DELAY);
    }
    ledRed.off();

    std::string str = fmt::format("Start {}\r\n", 133);
    for (const auto& ch : str) {
        send_char_blocking(ch);
    }

    // Utils::ezPrintf("Start %d\n", 133);
    // printf("Start %d\r\n", 133);

    uint64_t i = 0;
    uint32_t* sysCount = (uint32_t*)0xe000e018;

    while (true) {
        // ez_print("%llu\r", i++);
        *sysCount = 0;
        str = fmt::format("test string {}, {} ----  ", 9481231, -132003);
        for (const auto& ch : str) {
            send_char_blocking(ch);
        }
        str = fmt::format("fmt: {}\r\n", *sysCount);
        for (const auto& ch : str) {
            send_char_blocking(ch);
        }

        *sysCount = 0;
        printf("test string %d, %d  ----  ", 9481231, -132003);
        printf("printf: %d\r\n", *sysCount);

        *sysCount = 0;
        Utils::ezPrintf("test string %d, %d  ----  ", 9481231, -132003);
        Utils::ezPrintf("ezPrintf: %d\r\n", *sysCount);
        // ledGreen.toggle();
        delay_ms(TOGGLE_DELAY);
    }
}
