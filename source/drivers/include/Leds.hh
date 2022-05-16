#pragma once

#include "core_devices/GPIO.hh"

template <GPIO::Port port, GPIO::Pin pin>
class Led : public GPIO::Output<port, pin> {
  public:
    Led() : GPIO::Output<port, pin>{GPIO::DefaultState::High, GPIO::Pull::PullUp, GPIO::OutputType::OpenDrain} {};
    void on() const { GPIO::Output<port, pin>::setLow(); };
    void off() const { GPIO::Output<port, pin>::setHigh(); };
};

// static instance will increase code size
class LedRedInstance : public Led<GPIO::Port::B, GPIO::Pin::P11> {
  private:
    LedRedInstance() = default;

  public:
    static LedRedInstance& get() {
        static LedRedInstance instance;
        return instance;
    }

    ~LedRedInstance() = default;
    LedRedInstance(const LedRedInstance&) = delete;
    LedRedInstance(LedRedInstance&&) = delete;
    LedRedInstance& operator=(const LedRedInstance&) = delete;
    LedRedInstance& operator=(LedRedInstance&&) = delete;
};

class LedBlueInstance : public Led<GPIO::Port::B, GPIO::Pin::P3> {
  private:
    LedBlueInstance() = default;

  public:
    static LedBlueInstance& get() {
        static LedBlueInstance instance;
        return instance;
    }

    ~LedBlueInstance() = default;
    LedBlueInstance(const LedBlueInstance&) = delete;
    LedBlueInstance(LedBlueInstance&&) = delete;
    LedBlueInstance& operator=(const LedBlueInstance&) = delete;
    LedBlueInstance& operator=(LedBlueInstance&&) = delete;
};

class LedGreenInstance : public Led<GPIO::Port::B, GPIO::Pin::P1> {
  private:
    LedGreenInstance() = default;

  public:
    static LedGreenInstance& get() {
        static LedGreenInstance instance;
        return instance;
    }

    ~LedGreenInstance() = default;
    LedGreenInstance(const LedGreenInstance&) = delete;
    LedGreenInstance(LedGreenInstance&&) = delete;
    LedGreenInstance& operator=(const LedGreenInstance&) = delete;
    LedGreenInstance& operator=(LedGreenInstance&&) = delete;
};
