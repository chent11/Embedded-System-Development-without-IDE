#ifndef LED_H_
#define LED_H_

#include "hal/GPIO.hh"

class Led : protected GPIO {
  protected:
    Led(Port port, Pin pin) : GPIO(port, pin, Mode::OutputPushPull, Pull::NoPull, Speed::Low) { off(); };
    ~Led() { off(); };

  public:
    Led(const Led&) = delete;
    Led(Led&&) = delete;
    Led& operator=(const Led& rhs) = delete;
    Led& operator=(Led&& rhs) = delete;

    using GPIO::toggle;
    void on() const { setHigh(); };
    void off() const { setLow(); };
};

// singleton will increase code size because the compiler would link the atexit for destructing static method
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
