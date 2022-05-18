#pragma once

namespace GPIO {

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
enum class Port { A, B, C, D, E, F, G, H, I, J, K };
enum class Mode { Input, Output, Analog, Alternate };
enum class OutputType { None, PushPull, OpenDrain };
enum class Speed { None = 0, Low, Medium, High, VeryHigh };
enum class Pull { NoPull, PullUp, PullDown };
enum class State { Low, High };
using DefaultState = State;  // alias for readability
enum class AlternateFunction {
    None,
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

class Hardware {
  public:
    Hardware() = delete;
    static void setHigh(Port port, Pin pin);
    static void setLow(Port port, Pin pin);
    static void toggle(Port port, Pin pin);
    [[nodiscard]] static State getState(Port port, Pin pin);
    static void init(Port port, Pin pin, DefaultState state, Mode mode, Pull pull, OutputType outputType, Speed speed,
                     AlternateFunction alternateFunction);
};

template <Port port, Pin pin, Mode mode>
class Base {
  public:
    void setHigh() const requires(mode == Mode::Output) { Hardware::setHigh(port, pin); }
    void setLow() const requires(mode == Mode::Output) { Hardware::setLow(port, pin); }
    void toggle() const requires(mode == Mode::Output) { Hardware::toggle(port, pin); }
    [[nodiscard]] State getState() const { return Hardware::getState(port, pin); }

    // output mode
    Base(DefaultState state, Pull pull, OutputType outputType, Speed speed) requires(mode == Mode::Output) {
        Hardware::init(port, pin, state, mode, pull, outputType, speed, AlternateFunction::None);
    }

    // input mode
    explicit Base(DefaultState state, Pull pull) requires(mode == Mode::Input) {
        Hardware::init(port, pin, state, mode, pull, OutputType::None, Speed::None, AlternateFunction::None);
    }

    // alternate mode
    Base(DefaultState state, Pull pull, OutputType outputType, Speed speed,
         AlternateFunction alternateFunction) requires(mode == Mode::Alternate) {
        Hardware::init(port, pin, state, mode, pull, outputType, speed, alternateFunction);
    }
};

template <Port port, Pin pin>
class Output : public Base<port, pin, Mode::Output> {
  public:
    explicit Output(DefaultState state)
        : Base<port, pin, Mode::Output>{state, Pull::NoPull, OutputType::PushPull, Speed::Low} {}
    Output(DefaultState state, Pull pull, OutputType outputType)
        : Base<port, pin, Mode::Output>{state, pull, outputType, Speed::Low} {}
    Output(DefaultState state, Pull pull, OutputType outputType, Speed speed)
        : Base<port, pin, Mode::Output>{state, pull, outputType, speed} {}

    static void lowLevelInit(DefaultState state) { Output{state}; }
};

template <Port port, Pin pin>
class Input : public Base<port, pin, Mode::Input> {
  public:
    Input() : Base<port, pin, Mode::Input>{} {}
    explicit Input(Pull pull) : Base<port, pin, Mode::Input>{pull} {}
};

template <Port port, Pin pin>
class OutputInstance : public Output<port, pin> {
  public:
    static OutputInstance& get() {
        static OutputInstance instance;
        return instance;
    }
    static OutputInstance& get(Pull pull, OutputType outputType, Speed speed) {
        static OutputInstance instance{pull, outputType, speed};
        return instance;
    }

    ~OutputInstance() = default;
    OutputInstance(const OutputInstance&) = delete;
    OutputInstance(OutputInstance&&) = delete;
    OutputInstance& operator=(const OutputInstance&) = delete;
    OutputInstance& operator=(OutputInstance&&) = delete;

  private:
    OutputInstance() : Output<port, pin>{} {}
    OutputInstance(Pull pull, OutputType outputType, Speed speed) : Output<port, pin>{pull, outputType, speed} {}
};

};  // namespace GPIO
