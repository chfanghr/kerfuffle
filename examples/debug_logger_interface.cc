//
// Created by 方泓睿 on 8/4/20.
//

#include <fmt/format.h>

#define KERFUFFLE_ENABLE_SERIALIZATION
#define KERFUFFLE_ENABLE_VERBOSE_DEBUG_LOG
#include <kerfuffle.h>

using M = kerfuffle::Machine;

#define S(s) struct s

using FSM = M::Root<S(Top), S(From), S(To)>;

// FIXME: use kerfuffle::stateName
const char* stateName(kerfuffle::StateID id) {
  switch (id) {
    case FSM::stateId<Top>():
      return "Top";
    case FSM::stateId<From>():
      return "From";
    case FSM::stateId<To>():
      return "To";
    default:
      return "?";
  }
}

#undef S

struct Logger : kerfuffle::LoggerInterface {
  void recordMethod(Context& /*context*/,
                    const kerfuffle::StateID origin,
                    const Method             method) override {
    fmt::print("{}::{}()\n", stateName(origin), kerfuffle::methodName(method));
  }

  void recordTransition(Context& /*context*/,
                        const kerfuffle::StateID origin,
                        const TransitionType     transitionType,
                        const kerfuffle::StateID target) override {
    fmt::print("{}: {}<{}>()\n",
               stateName(origin),
               kerfuffle::transitionName(transitionType),
               stateName(target));
  }
};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
#pragma ide diagnostic ignored "HidingNonVirtualFunction"

struct EmptyStateBase : FSM::State {
  void entryGuard(GuardControl&) {}
  void enter(Control&) {}
  void update(FullControl&) {}
  template <typename TEvent>
  void react(const TEvent&, FullControl&) {}
  void exit(Control&) {}
};

#pragma clang diagnostic pop

struct Top : EmptyStateBase {};
struct To : EmptyStateBase {};
struct From : EmptyStateBase {};

int main() {
  {
    // logger
    Logger logger;

    fmt::print("\n---------- ctor: ---------\n\n");

    // state machine instance - all initial states are activated
    FSM::Instance machine{&logger};

    // output:
    //	enter()
    //	From::enter()

    fmt::print("\n--------- update: --------\n\n");

    // first update
    machine.update();

    // output:
    //	update()
    //	From::update()

    fmt::print("\n--------- react: ---------\n\n");

    machine.react(1);

    // output:
    //	react()
    //	From::react()
    //	changeTo<To>()
    //	To::entryGuard()
    //	From::exit()
    //	To::enter()

    fmt::print("\n-- external transition: --\n\n");

    machine.changeTo<From>();

    // output:
    //	changeTo<From>()

    fmt::print("\n--------- detach: --------\n\n");

    // detach logger and update again
    machine.attachLogger(nullptr);
    machine.update();

    // no output, since logger is detached

    fmt::print("\n---------- dtor: ---------\n\n");

    // re-attach logger for destruction log
    machine.attachLogger(&logger);

    // state machine instance gets destroyed
  }

  // output:
  //	To::exit()
  //	exit()

  fmt::print("\n---------- done! ---------\n\n");

  return 0;
}