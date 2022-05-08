#ifndef LED_H_
#define LED_H_

#include "hal/GPIO.hh"

class Led : protected GPIODef<GPIO::Mode::Output> {
  protected:
    Led(Port port, Pin pin)
        : GPIODef(port, pin, Pull::NoPull, OutputType::PushPull, Speed::Low) { off(); };
    ~Led() = default;

  public:
    Led(const Led&) = delete;
    Led(Led&&)      = delete;
    Led& operator=(const Led&) = delete;
    Led& operator=(Led&&) = delete;

    using GPIO::toggle;
    void on() const { setLow(); };
    void off() const { setHigh(); };
};

// static instance will increase code size
class LedRed : public Led {
  public:
    static LedRed& getInstance() {
        static LedRed instance;
        return instance;
    }

  private:
    LedRed() : Led(Port::B, Pin::P11) {}
};

class LedBlue : public Led {
  public:
    static LedBlue& getInstance() {
        static LedBlue instance;
        return instance;
    }

  private:
    LedBlue() : Led(Port::B, Pin::P3) {}
};

class LedGreen : public Led {
  public:
    static LedGreen& getInstance() {
        static LedGreen instance;
        return instance;
    }

  private:
    LedGreen() : Led(Port::B, Pin::P1) {}
};

#endif
