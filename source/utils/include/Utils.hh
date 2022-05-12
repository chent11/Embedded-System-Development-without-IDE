#ifndef UTILS_HH_
#define UTILS_HH_

#include <array>
#include <cstdio>
#include <string>

#include "core_init.h"

class Utils {
  public:
    template <typename... Args>
    static void ezPrintf(const char* fmt, Args... args) noexcept {
        const int PRINT_MAX_LENGTH = 100;
        std::array<char, PRINT_MAX_LENGTH> buffer{'\0'};
        int cx = snprintf(buffer.data(), PRINT_MAX_LENGTH, fmt, args...);  // NOLINT

        if (cx < 0 || cx > PRINT_MAX_LENGTH) {
            std::string errorMessage;
            if (cx > 0) {
                errorMessage = "Utils::ezPrintf Error: string are too long; Expect < " + std::to_string(PRINT_MAX_LENGTH) + ", got " + std::to_string(cx);
            } else {
                errorMessage = "Utils::ezPrintf Error: unknown error code, " + std::to_string(cx);
            }
            errorMessage += "\r\n";
            for (const auto& ch : errorMessage) {
                send_char_blocking(ch);
            }
            return;
        }

        for (const auto& ch : buffer) {
            if (ch == '\0') {
                return;
            }
            send_char_blocking(ch);
        }
    }
};
#endif
