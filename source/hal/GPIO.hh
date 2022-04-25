#ifndef GPIO_H_
#define GPIO_H_

#include <cstdint>

class GPIO {
  public:
    enum class Pin {
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
    enum class Port {
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

        OutputPushPull,
        OutputOpenDrain,

        AlternatePushPull,
        AlternateOpenDrain,

        Analog,

        ExternalInterruptRisingEdge,
        ExternalInterruptFallingEdge,
        ExternalInterruptRsingAndFallingEdge,

        ExternalEventRisingEdge,
        ExternalEventFallingEdge,
        ExternalEventRisingAndFallingEdge
    };
    enum class Speed {
        Low,
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

  private:
    const Port _port;
    const Pin _pin;

  protected:
    GPIO(Port port, Pin pin, Mode mode, Pull pull, Speed speed);
    ~GPIO();

  public:
    GPIO(const GPIO&) = delete;
    GPIO(GPIO&&) = delete;
    GPIO& operator=(const GPIO& rhs) = delete;
    GPIO& operator=(GPIO&& rhs) = delete;

    void setHigh() const;
    void setLow() const;
    void toggle() const;
    State getState() const;
};

#endif
