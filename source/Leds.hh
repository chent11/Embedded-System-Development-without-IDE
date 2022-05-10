#ifndef LED_H_
#define LED_H_

#include "drivers/core_devices/GPIO.hh"

template <GPIO::Port port, GPIO::Pin pin>
class Led : public GPIO::OutputMode<port, pin> {
  public:
    Led() : GPIO::OutputMode<port, pin>{} { off(); };
    void on() const { GPIO::OutputMode<port, pin>::setLow(); };
    void off() const { GPIO::OutputMode<port, pin>::setHigh(); };
};

// static instance will increase code size
class LedRed : public Led<GPIO::Port::B, GPIO::Pin::P11> {
  private:
    LedRed() = default;

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

class LedBlue : public Led<GPIO::Port::B, GPIO::Pin::P3> {
  private:
    LedBlue() = default;

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

class LedGreen : public Led<GPIO::Port::B, GPIO::Pin::P1> {
  private:
    LedGreen() = default;

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
