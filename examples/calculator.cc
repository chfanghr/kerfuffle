//
// Created by 方泓睿 on 2020/8/7.
//

#include <curses.h>
#include <fmt/format.h>
#include <kerfuffle.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

int main();

#define S(x) struct x

using M = kerfuffle::MachineT<kerfuffle::Config::ContextT<S(Context)>>;

// clang-format off

using FSM = M::PeerRoot<
    S(Off),
    M::Composite<
        S(On),
        M::Composite<
            S(Ready),
            S(Begin),
            S(Result)
            >
        >,
    S(Negated1),
    M::Composite<
        S(Operand1),
        S(Zero1),
        S(Int1),
        S(Frac1)
        >,
    S(Error),
    S(OpEntered),
    S(Negated2),
    M::Composite<
        S(Operand2),
        S(Zero2),
        S(Int2),
        S(Frac2)>
    >;

// clang-format on

#undef S

enum MathOperator : uint { IDLE, PLUS, MINUS, MULTIPLY, DIVIDE };
const char* const MathOperationString[] = {"?", "+", "-", "*", "/"};

struct Context {
  double       operand1      = {0.0};
  MathOperator mathOperator  = {MathOperator::IDLE};
  double       operand2      = {0.0};
  double       decimalFactor = {1.0};

  void reset() { *this = Context{}; }

  void error() {
    fmt::print(">>>!!! Error !!!<<<\n");
    display();
  }

  void display() {
    fmt::print("\n");
    fmt::print("[{},{}]\n", MathOperationString[mathOperator], decimalFactor);
    fmt::print("2: {}\n", operand2);
    fmt::print("1: {}\n", operand1);
  }

  void clearOperand1() { operand1 = 0.0; }

  void clearOperand2() { operand2 = 0.0; }

  void beginFraction() { decimalFactor = 10.0; }

  void endFraction() { decimalFactor = 1.0; }

  void negateOperand1() { operand1 = -operand1; }

  void negateOperand2() { operand2 = -operand2; }

  void insertInOperand1(int digit) {
    if (decimalFactor > 1.0) {
      operand1 += std::copysign((double)digit / decimalFactor, operand1);
      decimalFactor *= 10.0;
    } else {
      operand1 *= 10;
      operand1 += std::copysign(digit, operand1);
    }
  }

  void insertInOperand2(int digit) {
    if (decimalFactor > 1.0) {
      operand2 += std::copysign((double)digit / decimalFactor, operand2);
      decimalFactor *= 10.0;
    } else {
      operand2 *= 10;
      operand2 += std::copysign(digit, operand2);
    }
  }

  bool performOp() {
    switch (mathOperator) {
      case PLUS:
        operand1 += operand2;
        return true;
      case MINUS:
        operand1 -= operand2;
        return true;
      case MULTIPLY:
        operand1 *= operand2;
        return true;
      case DIVIDE:
        if (operand2 != 0.0) {
          operand1 /= operand2;
          return true;
        }
      default:
        return false;
    }
  }
};

#define S(x) \
  struct x {}

S(TurnOn);
S(Clear);  // C
S(TurnOff);
S(ClearEntry);  // CE
S(Digit_0);     // 0
struct Digit_1_9 {
  int digit;
};         // 1...9
S(Point);  // .
struct Operator {
  MathOperator mathOperator;
};  // +,-,*,/
S(Equals);

#undef S

struct Off : FSM::State {
  using FSM::State::react;

  void react(const TurnOn&, FullControl& control) { control.changeTo<On>(); }
};

// On
struct On : FSM::State {
  using FSM::State::react;

  void enter(Control& control) { control.context().reset(); }

  void reenter(Control& control) { control.context().reset(); }

  void react(const Clear&, FullControl& control) { control.changeTo<On>(); }

  void react(const TurnOff&, FullControl& control) { control.changeTo<Off>(); }
};

// On::Ready
struct Ready : FSM::State {
  using FSM::State::react;

  void react(const Digit_0&, FullControl& control) {
    control.context().clearOperand1();
    control.context().insertInOperand1(0);
    control.changeTo<Zero1>();
  }

  void react(const Digit_1_9& digit, FullControl& control) {
    control.context().clearOperand1();
    control.context().insertInOperand1(digit.digit);
    control.changeTo<Int1>();
  }

  void react(const Point&, FullControl& control) {
    control.context().clearOperand1();
    control.changeTo<Frac1>();
  }

  void react(const Operator& op, FullControl& control) {
    // This is where bottom-op invocation order is needed
    // In this higher level state I have to ask, if a lower level is active or
    // not
    if (control.isActive<Begin>() && op.mathOperator == MathOperator::MINUS)
      control.changeTo<Negated1>();
    else {
      control.context().mathOperator = op.mathOperator;
      control.changeTo<OpEntered>();
    }
  }
};

// On::Ready::Begin
struct Begin : FSM::State {
  using FSM::State::react;

  void enter(Control& control) {
    control.context().clearOperand1();
    control.context().display();
  }
};

// On::Ready::Result
struct Result : FSM::State {
  using FSM::State::react;

  void enter(Control& control) { control.context().display(); }

  void reenter(Control& control) { control.context().display(); }

  void react(const Equals&, FullControl& control) {
    ;
    control.context().performOp();
    control.changeTo<Result>();
  }
};

// On::Negated1
struct Negated1 : FSM::State {
  using FSM::State::react;

  void enter(Control& control) {
    control.context().negateOperand1();
    control.context().display();
  }

  void reenter(Control& control) {
    control.context().negateOperand1();
    control.context().display();
  }

  void react(const ClearEntry&, FullControl& control) {
    ;
    control.changeTo<Begin>();
  }

  void react(const Digit_0&, FullControl& control) {
    control.changeTo<Zero1>();
  }

  void react(const Digit_1_9& digit, FullControl& control) {
    control.context().insertInOperand1(digit.digit);
    control.changeTo<Int1>();
  }

  void react(const Point&, FullControl& control) { control.changeTo<Frac1>(); }

  void react(const Operator& op, FullControl& control) {
    if (op.mathOperator == MathOperator::MINUS) control.changeTo<Negated1>();
  }
};

// On::Operand1
struct Operand1 : FSM::State {
  using FSM::State::react;

  void react(const ClearEntry&, FullControl& control) {
    control.context().clearOperand1();
    control.changeTo<Ready>();
  }

  void react(const Operator& op, FullControl& control) {
    control.context().mathOperator = op.mathOperator;
    control.changeTo<OpEntered>();
  }
};

// On::Operand1::Zero1
struct Zero1 : FSM::State {
  using FSM::State::react;

  void enter(Control& control) {
    control.context().insertInOperand1(0);
    control.context().display();
  }

  void reenter(Control& control) {
    control.context().insertInOperand1(0);
    control.context().display();
  }

  void react(const Digit_0&, FullControl& control) {
    control.changeTo<Zero1>();
  }

  void react(const Digit_1_9& digit, FullControl& control) {
    control.context().insertInOperand1(digit.digit);
    control.changeTo<Int1>();
  }

  void react(const Point&, FullControl& control) { control.changeTo<Frac1>(); }
};

// On::Operand1::Int1
struct Int1 : FSM::State {
  using FSM::State::react;

  void enter(Control& control) { control.context().display(); }

  void reenter(Control& control) { control.context().display(); }

  void react(const Digit_0&, FullControl& control) {
    control.context().insertInOperand1(0);
    control.changeTo<Int1>();
  }

  void react(const Digit_1_9& digit, FullControl& control) {
    control.context().insertInOperand1(digit.digit);
    control.changeTo<Int1>();
  }

  void react(const Point&, FullControl& control) { control.changeTo<Frac1>(); }
};

// On::Operand1::Frac1
struct Frac1 : FSM::State {
  using FSM::State::react;

  void enter(Control& control) { control.context().beginFraction(); }

  void reenter(Control& control) { control.context().display(); }

  void exit(Control& control) { control.context().endFraction(); }

  void react(const Digit_0&, FullControl& control) {
    control.context().insertInOperand1(0);
    control.changeTo<Frac1>();
  }

  void react(const Digit_1_9& digit, FullControl& control) {
    control.context().insertInOperand1(digit.digit);
    control.changeTo<Frac1>();
  }
};

// On::Error
struct Error : FSM::State {
  using FSM::State::react;

  void enter(Control& control) { control.context().error(); }

  void reenter(Control& control) { control.context().error(); }
};

// On::opEntered
struct OpEntered : FSM::State {
  using FSM::State::react;

  void enter(Control& control) {
    control.context().clearOperand2();
    control.context().display();
  }

  void reenter(Control& control) {
    control.context().clearOperand2();
    control.context().display();
  }

  void react(const Digit_0&, FullControl& control) {
    control.context().insertInOperand2(0);
    control.changeTo<Zero2>();
  }

  void react(const Digit_1_9& digit, FullControl& control) {
    control.context().insertInOperand2(digit.digit);
    control.changeTo<Int2>();
  }

  void react(const Point&, FullControl& control) { control.changeTo<Frac2>(); }

  void react(const Operator& op, FullControl& control) {
    ;
    if (op.mathOperator == MathOperator::MINUS)
      control.changeTo<Negated2>();
    else {
      control.context().mathOperator = op.mathOperator;
      control.changeTo<OpEntered>();
    }
  }
};

// On::Negated2
struct Negated2 : FSM::State {
  using FSM::State::react;

  void enter(Control& control) {
    control.context().negateOperand2();
    control.context().display();
  }

  void reenter(Control& control) {
    control.context().negateOperand2();
    control.context().display();
  }

  void react(const ClearEntry&, FullControl& control) {
    control.changeTo<OpEntered>();
  }

  void react(const Digit_0&, FullControl& control) {
    control.changeTo<Zero2>();
  }

  void react(const Digit_1_9& digit, FullControl& control) {
    control.context().insertInOperand2(digit.digit);
    control.changeTo<Int2>();
  }

  void react(const Point&, FullControl& control) { control.changeTo<Frac2>(); }

  void react(const Operator& op, FullControl& control) {
    if (op.mathOperator == MathOperator::MINUS) control.changeTo<Negated2>();
  }
};

// On::Operand2
struct Operand2 : FSM::State {
  using FSM::State::react;

  void enter(Control& control) { control.context().display(); }

  void reenter(Control& control) { control.context().display(); }

  void react(const Operator& op, FullControl& control) {
    if (control.context().performOp()) {
      control.context().mathOperator = op.mathOperator;
      control.changeTo<OpEntered>();
    } else
      control.changeTo<Error>();
  }

  void react(const Equals&, FullControl& control) {
    if (control.context().performOp())
      control.changeTo<Result>();
    else
      control.changeTo<Error>();
  }

  void react(const ClearEntry&, FullControl& control) {
    control.context().clearOperand2();
    control.changeTo<OpEntered>();
  }
};

// On::Operand2::Zero2
struct Zero2 : FSM::State {
  using FSM::State::react;

  void enter(Control& control) {
    control.context().insertInOperand2(0);
    control.context().display();
  }

  void reenter(Control& control) {
    control.context().insertInOperand2(0);
    control.context().display();
  }

  void react(const Digit_0&, FullControl& control) {
    control.changeTo<Zero2>();
  }

  void react(const Digit_1_9& digit, FullControl& control) {
    control.context().insertInOperand2(digit.digit);
    control.changeTo<Int2>();
  }

  void react(const Point&, FullControl& control) { control.changeTo<Frac2>(); }
};

// On::Operand2::Int2
struct Int2 : FSM::State {
  using FSM::State::react;

  void enter(Control& control) { control.context().display(); }

  void reenter(Control& control) { control.context().display(); }

  void react(const Digit_0&, FullControl& control) {
    control.context().insertInOperand2(0);
    control.changeTo<Int2>();
  }

  void react(const Digit_1_9& digit, FullControl& control) {
    control.context().insertInOperand2(digit.digit);
    control.changeTo<Int2>();
  }

  void react(const Point&, FullControl& control) { control.changeTo<Frac2>(); }
};

// On::Operand2::Frac2
struct Frac2 : FSM::State {
  using FSM::State::react;

  void enter(Control& control) {
    control.context().display();
    control.context().beginFraction();
  }

  void reenter(Control& control) { control.context().display(); }

  void exit(Control& control) { control.context().endFraction(); }

  void react(const Digit_0&, FullControl& control) {
    control.context().insertInOperand2(0);
    control.changeTo<Frac2>();
  }

  void react(const Digit_1_9& digit, FullControl& control) {
    control.context().insertInOperand2(digit.digit);
    control.changeTo<Frac2>();
  }
};

int main() {
  initscr();
  timeout(-1);
  refresh();

  Context       context{};
  FSM::Instance machine{context};
  machine.react(TurnOn{});

  int ch{};

  do {
    refresh();
    ch = getchar();
    switch (ch) {
      case '0': {
        machine.react(Digit_0{});
        break;
      }
      case '1': {
        machine.react(Digit_1_9{1});
        break;
      }
      case '2': {
        machine.react(Digit_1_9{2});
        break;
      }
      case '3': {
        machine.react(Digit_1_9{3});
        break;
      }
      case '4': {
        machine.react(Digit_1_9{4});
        break;
      }
      case '5': {
        machine.react(Digit_1_9{5});
        break;
      }
      case '6': {
        machine.react(Digit_1_9{6});
        break;
      }
      case '7': {
        machine.react(Digit_1_9{7});
        break;
      }
      case '8': {
        machine.react(Digit_1_9{8});
        break;
      }
      case '9': {
        machine.react(Digit_1_9{9});
        break;
      }
      case 'c': {
        machine.react(Clear{});
        break;
      }
      case 'e': {
        machine.react(ClearEntry{});
        break;
      }
      case '+': {
        machine.react(Operator{MathOperator::PLUS});
        break;
      }
      case '-': {
        machine.react(Operator{MathOperator::MINUS});
        break;
      }
      case '*': {
        machine.react(Operator{MathOperator::MULTIPLY});
        break;
      }
      case '/': {
        machine.react(Operator{MathOperator::DIVIDE});
        break;
      }
      case ',':
      case '.': {
        machine.react(Point{});
        break;
      }
      case '=': {
        machine.react(Equals{});
        break;
      }
      case 'x':
        break;
      default: {
        ch = ' ';
        break;
      }
    }
  } while (ch != 'x');

  endwin();

  machine.react(TurnOff{});

  return EXIT_SUCCESS;
}

#pragma clang diagnostic pop