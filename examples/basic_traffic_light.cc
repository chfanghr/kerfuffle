//
// Created by 方泓睿 on 8/4/20.
//

#include <cstdlib>
#include <fmt/format.h>

#define KERFUFFLE_ENABLE_STRUCTURE_REPORT
#include <kerfuffle.h>

struct Context {
  unsigned cycleCount;
};

using Machine = kerfuffle::MachineT<kerfuffle::Config::ContextT<Context>>;

#define S(x) struct x

// clang-format off

using FSM =
Machine::PeerRoot<
    Machine::Composite<
        S(On),
        S(Red),
        S(YellowDownwards),
        S(YellowUpwards),
        S(Green)>,
    S(Off)>;

// clang-format on

#undef S

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
#pragma ide diagnostic ignored "HidingNonVirtualFunction"

struct On : FSM::State {
  void enter(Control& control) {
    control.context().cycleCount = 0;
    fmt::print("⚪\n");
  }
};

struct Red : FSM::State {
  void enter(Control& control) {
    control.context().cycleCount++;
    fmt::print("  🔴\n");
  }

  void update(FullControl& control) {
    if (control.context().cycleCount > 3)
      control.changeTo<Off>();
    else
      control.changeTo<YellowDownwards>();
  }
};

struct YellowDownwards : FSM ::State {
  void enter(Control& control) { fmt::print("    🟡🟢\n"); }

  void update(FullControl& control) { control.changeTo<Green>(); }
};

struct YellowUpwards : FSM::State {
  void enter(Control& control) { fmt::print("    🟡🔴\n"); }

  void update(FullControl& control) { control.changeTo<Red>(); }
};

struct Green : FSM::State {
  void enter(Control& control) { fmt::print("    🟢\n"); }

  void update(FullControl& control) { control.changeTo<YellowUpwards>(); }
};

struct Off : FSM::State {
  void enter(Control& control) {
    control.context().cycleCount = 0;
    fmt::print("⚫️️\n");
  }
};

#pragma clang diagnostic pop

int main() {
  Context context{};

  FSM::Instance machine{context};

  while (!machine.isActive<Off>()) machine.update();

  return EXIT_SUCCESS;
}
