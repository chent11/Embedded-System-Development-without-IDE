#include "core_devices/GPIO.hh"
#include "core_devices/core_vendors/core_vendors.hh"

namespace GPIO {

void Hardware::init(Port port, Pin pin, DefaultState state, Mode mode, Pull pull, OutputType outputType, Speed speed,
                    AlternateFunction af) {
    Vendor::HardwareGPIO::Configs configs{mode, pull, outputType, speed, af};
    Vendor::HardwareGPIO::init(port, pin, configs);

    switch (state) {
        case DefaultState::Low:
            setLow(port, pin);
            break;
        case DefaultState::High:
            setHigh(port, pin);
            break;
        case DefaultState::Default:
        default:
            break;
    }
}

void Hardware::setHigh(Port port, Pin pin) { Vendor::HardwareGPIO::setHigh(port, pin); }

void Hardware::setLow(Port port, Pin pin) { Vendor::HardwareGPIO::setLow(port, pin); }

void Hardware::toggle(Port port, Pin pin) { Vendor::HardwareGPIO::toggle(port, pin); }

State Hardware::getState(Port port, Pin pin) { return Vendor::HardwareGPIO::getState(port, pin); }

};  // namespace GPIO
