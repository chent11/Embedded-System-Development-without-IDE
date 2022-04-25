#ifndef LED_H_
#define LED_H_

#include "hal/GPIO.hh"

class Led : protected GPIO {
  public:
    Led(Port port, Pin pin) : GPIO(port, pin, Mode::OutputPushPull, Pull::NoPull, Speed::Low) { off(); };
    ~Led() { off(); };

    Led(const Led&) = delete;
    Led(Led&&) = delete;
    Led& operator=(const Led& rhs) = delete;
    Led& operator=(Led&& rhs) = delete;

    using GPIO::toggle;
    void on() const { setHigh(); };
    void off() const { setLow(); };
};

class LedRed : public Led {
  public:
    LedRed() : Led(Port::B, Pin::P11) {}
};

class LedBlue : public Led {
  public:
    LedBlue() : Led(Port::B, Pin::P3) {}
};

class LedGreen : public Led {
  public:
    LedGreen() : Led(Port::B, Pin::P1) {}
};
#endif
