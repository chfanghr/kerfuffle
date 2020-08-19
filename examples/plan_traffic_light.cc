//
// Created by 方泓睿 on 2020/8/19.
//

#include <fmt/format.h>

#include <cstdlib>

#define KERFUFFLE_ENABLE_PLANS
#define KERFUFFLE_ENABLE_STRUCTURE_REPORT
#include <kerfuffle.h>

struct Context {
  bool powerOn;
};

using Machine = kerfuffle::MachineT<kerfuffle::Config::ContextT<Context>>;

#define S(x) struct x

// clang-format off

using FSM =
Machine::PeerRoot<
    S(Off),
    Machine::Composite<
        S(On),
        S(Red),
        S(Yellow),
        S(Green)>,
    S(Done)
    >;

// clang-format on

#undef S

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
#pragma ide diagnostic ignored "HidingNonVirtualFunction"

struct Event {};

struct On : FSM::State {
  void enter(PlanControl& control) {
    auto plan = control.plan();
    plan.change<Red, Yellow>();
    plan.change<Yellow, Green>();
    plan.change<Green, Yellow>();
    plan.change<Yellow, Red>();

    fmt::print("⚪\n");
  }

  void exit(PlanControl& control) {}

  void planSucceeded(FullControl& control) { control.changeTo<Done>(); }

  void planFailed(FullControl& control) {}
};

struct Red : FSM::State {
  void enter(Control& control) { fmt::print("  🔴\n"); }

  void update(FullControl& control) { control.succeed(); }
};

struct Yellow : FSM::State {
  void enter(Control& control) { fmt::print("  🟡\n"); }

  void update(FullControl& control) { control.succeed(); }
};

struct Green : FSM::State {
  void enter(Control& control) { fmt::print("  🟢\n"); }

  void update(FullControl& control) { control.succeed(); }
};

struct Off : FSM::State {
  void enter(Control& control) { fmt::print("⚫️\n"); }

  void entryGuard(FullControl& control) {
    if (control.context().powerOn) control.changeTo<On>();
  }
};

struct Done : FSM::State {
  void enter(Control& control) { fmt::print("💤️\n"); }
};

#pragma clang diagnostic pop

int main() {
  Context context{true};

  FSM::Instance machine{context};

  while (!machine.isActive<Done>()) machine.update();

  return EXIT_SUCCESS;
}
