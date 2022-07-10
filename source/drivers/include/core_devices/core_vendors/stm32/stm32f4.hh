#pragma once

#include "core_devices/GPIO.hh"

namespace STM32f4 {

namespace HardwareGPIO {
struct Configs {
    GPIO::Mode mode;
    GPIO::Pull pull;
    GPIO::OutputType outputType;
    GPIO::Speed speed;
    GPIO::AlternateFunction alternate;
};
void init(GPIO::Port port, GPIO::Pin pin, const Configs& configs);
void setHigh(GPIO::Port port, GPIO::Pin pin);
void setLow(GPIO::Port port, GPIO::Pin pin);
void toggle(GPIO::Port port, GPIO::Pin pin);
GPIO::State getState(GPIO::Port port, GPIO::Pin pin);
}  // namespace HardwareGPIO

namespace HardwareTimer {}  // namespace HardwareTimer

}  // namespace STM32f4