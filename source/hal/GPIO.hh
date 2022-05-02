#ifndef GPIO_H_
#define GPIO_H_

#include <cstdint>
#include <type_traits>

class GPIO {
  public:
    enum class Pin : uint16_t {
        P0,
        P1,
        P2,
        P3,
        P4,
        P5,
        P6,
        P7,
        P8,
        P9,
        P10,
        P11,
        P12,
        P13,
        P14,
        P15,
    };
    enum class Port : uint16_t {
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K
    };
    enum class Mode {
        Input,
        Output,
        Analog,
        Alternate
    };
    enum class OutputType {
        PushPull,
        OpenDrain
    };
    enum class Speed {
        Low = 0,
        Medium,
        High,
        VeryHigh
    };
    enum class Pull {
        NoPull,
        PullUp,
        PullDown
    };
    enum class State {
        Low,
        High
    };
    enum class AlternateFunction {
        AF0,
        AF1,
        AF2,
        AF3,
        AF4,
        AF5,
        AF6,
        AF7,
        AF8,
        AF9,
        AF10,
        AF11,
        AF12,
        AF13,
        AF14,
        AF15
    };

  private:
    const Port _port;
    const Pin _pin;
    GPIO(Port port, Pin pin);  // delegate

  protected:
    GPIO(Port port, Pin pin, Pull pull);
    GPIO(Port port, Pin pin, Pull pull, OutputType outputType, Speed speed);
    GPIO(Port port, Pin pin, Pull pull, OutputType outputType, Speed speed, AlternateFunction alternateFunction);
    void setHigh() const;
    void setLow() const;
    void toggle() const;
    State getState() const;
};

template <GPIO::Mode mode>
class GPIODef : protected GPIO {
  public:
    template <Mode _mode = mode, typename std::enable_if_t<_mode == Mode::Output, int> = 0>  // Require(mode == Mode::Output)
    GPIODef(Port port, Pin pin, Pull pull, OutputType outputType, Speed speed)
        : GPIO(port, pin, pull, outputType, speed) {}

    template <Mode _mode = mode, typename std::enable_if_t<_mode == Mode::Input, int> = 0>  // Require(mode == Mode::Input)
    GPIODef(Port port, Pin pin, Pull pull)
        : GPIO(port, pin, pull) {}

    template <Mode _mode = mode, typename std::enable_if_t<_mode == Mode::Alternate, int> = 0>  // Require(mode == Mode::Alternate)
    GPIODef(Port port, Pin pin, Pull pull, OutputType outputType, Speed speed, AlternateFunction alternateFunction)
        : GPIO(port, pin, pull, outputType, speed, alternateFunction) {}
};

#endif
