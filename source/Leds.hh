#ifndef LED_H_
#define LED_H_

#include "hal/GPIO.hh"

class Led : protected GPIODef<GPIO::Mode::Output> {
  protected:
    Led(Port port, Pin pin)
        : GPIODef(port, pin, Pull::NoPull, OutputType::PushPull, Speed::Low) { off(); };

  public:
    using GPIO::toggle;
    void on() const { setLow(); };
    void off() const { setHigh(); };
};

// static instance will increase code size
class LedRed : public Led {
  private:
    LedRed() : Led(Port::B, Pin::P11) {}

  public:
    static LedRed& getInstance() {
        static LedRed instance;
        return instance;
    }

    ~LedRed() = default;

    LedRed(const LedRed&) = delete;
    LedRed(LedRed&&)      = delete;
    LedRed& operator=(const LedRed&) = delete;
    LedRed& operator=(LedRed&&) = delete;
};

class LedBlue : public Led {
  private:
    LedBlue() : Led(Port::B, Pin::P3) {}

  public:
    static LedBlue& getInstance() {
        static LedBlue instance;
        return instance;
    }

    ~LedBlue() = default;

    LedBlue(const LedBlue&) = delete;
    LedBlue(LedBlue&&)      = delete;
    LedBlue& operator=(const LedBlue&) = delete;
    LedBlue& operator=(LedBlue&&) = delete;
};

class LedGreen : public Led {
  private:
    LedGreen() : Led(Port::B, Pin::P1) {}

  public:
    static LedGreen& getInstance() {
        static LedGreen instance;
        return instance;
    }

    ~LedGreen() = default;

    LedGreen(const LedGreen&) = delete;
    LedGreen(LedGreen&&)      = delete;
    LedGreen& operator=(const LedGreen&) = delete;
    LedGreen& operator=(LedGreen&&) = delete;
};

#endif
