#ifndef GPIO_H_
#define GPIO_H_

#include <cstdint>

class GPIO {
  public:
    enum class Pin {
        P0 = 0,
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

    GPIO() = delete;

  private:
    static void initInputMode(Port port, Pin pin, Pull pull);
    static void initOutputMode(Port port, Pin pin, Pull pull, OutputType outputType, Speed speed);
    static void initAlternateMode(Port port, Pin pin, Pull pull, OutputType outputType, Speed speed, AlternateFunction alternateFunction);
    static void setHigh(Port port, Pin pin);
    static void setLow(Port port, Pin pin);
    static void toggle(Port port, Pin pin);
    static State getState(Port port, Pin pin);

    template <Port port, Pin pin, Mode mode>
    class Base {
      public:
        void setHigh() const requires(mode == Mode::Output) {
            GPIO::setHigh(port, pin);
        }

        void setLow() const requires(mode == Mode::Output) {
            GPIO::setLow(port, pin);
        }

        void toggle() const requires(mode == Mode::Output) {
            GPIO::toggle(port, pin);
        }

        [[nodiscard]] State getState() const {
            return GPIO::getState(port, pin);
        }

      protected:
        // output mode
        Base() requires(mode == Mode::Output) {  // default output configuration
            GPIO::initOutputMode(port, pin, Pull::NoPull, OutputType::PushPull, Speed::Low);
        }
        Base(Pull pull, OutputType outputType, Speed speed) requires(mode == Mode::Output) {
            GPIO::initOutputMode(port, pin, pull, outputType, speed);
        }

        // input mode
        Base() requires(mode == Mode::Input) {  // default input configuration
            GPIO::initInputMode(port, pin, Pull::PullUp);
        }
        explicit Base(Pull pull) requires(mode == Mode::Input) {
            GPIO::initInputMode(port, pin, pull);
        }

        // alternate
        Base(Pull pull, OutputType outputType, Speed speed, AlternateFunction alternateFunction) requires(mode == Mode::Alternate) {
            GPIO::initAlternateMode(port, pin, pull, outputType, speed, alternateFunction);
        }
    };

  public:
    template <Port port, Pin pin>
    class OutputBase : public Base<port, pin, Mode::Output> {
      protected:
        OutputBase() : Base<port, pin, Mode::Output>{} {}
        OutputBase(Pull pull, OutputType outputType, Speed speed) : Base<port, pin, Mode::Output>{pull, outputType, speed} {}
    };

    template <Port port, Pin pin>
    class InputBase : public Base<port, pin, Mode::Input> {
      protected:
        InputBase() : Base<port, pin, Mode::Input>{} {}
        explicit InputBase(Pull pull) : Base<port, pin, Mode::Input>{pull} {}
    };

    template <Port port, Pin pin>
    class Output : public OutputBase<port, pin> {
      public:
        static Output& getInstance() {
            static Output instance;
            return instance;
        }
        static Output& getInstance(Pull pull, OutputType outputType, Speed speed) {
            static Output instance{pull, outputType, speed};
            return instance;
        }

        ~Output() = default;
        Output(const Output&) = delete;
        Output(Output&&) = delete;
        Output& operator=(const Output&) = delete;
        Output& operator=(Output&&) = delete;

      protected:
        Output() : OutputBase<port, pin>{} {}
        Output(Pull pull, OutputType outputType, Speed speed) : OutputBase<port, pin>{pull, outputType, speed} {}
    };

};  // namespace GPIO

#endif
