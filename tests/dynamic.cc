//
// Created by 方泓睿 on 8/12/20.
//

#define KERFUFFLE_ENABLE_VERBOSE_DEBUG_LOG
#include "helper.h"

namespace kerfuffle {
namespace test {
using M = kerfuffle::Machine;

using FSM = M::PeerRoot<
    M::Composite<S(A), S(A_1), M::Composite<S(A_2), S(A_2_1), S(A_2_2)>>,
    M::Orthogonal<S(B),
                  M::Composite<S(B_1), S(B_1_1), S(B_1_2)>,
                  M::Composite<S(B_2), S(B_2_1), S(B_2_2)>>>;

STATIC_ASSERT_EQ(FSM::regionId<A>(), 1);
STATIC_ASSERT_EQ(FSM::regionId<A_2>(), 2);
STATIC_ASSERT_EQ(FSM::regionId<B>(), 3);
STATIC_ASSERT_EQ(FSM::regionId<B_1>(), 4);
STATIC_ASSERT_EQ(FSM::regionId<B_2>(), 5);

STATIC_ASSERT_EQ(FSM::stateId<A>(), 1);
STATIC_ASSERT_EQ(FSM::stateId<A_1>(), 2);
STATIC_ASSERT_EQ(FSM::stateId<A_2>(), 3);
STATIC_ASSERT_EQ(FSM::stateId<A_2_1>(), 4);
STATIC_ASSERT_EQ(FSM::stateId<A_2_2>(), 5);
STATIC_ASSERT_EQ(FSM::stateId<B>(), 6);
STATIC_ASSERT_EQ(FSM::stateId<B_1>(), 7);
STATIC_ASSERT_EQ(FSM::stateId<B_1_1>(), 8);
STATIC_ASSERT_EQ(FSM::stateId<B_1_2>(), 9);
STATIC_ASSERT_EQ(FSM::stateId<B_2>(), 10);
STATIC_ASSERT_EQ(FSM::stateId<B_2_1>(), 11);
STATIC_ASSERT_EQ(FSM::stateId<B_2_2>(), 12);

#define DEFAULT_DYNAMIC_STATE_IMPL(x) \
  struct x : FSM::DynamicState {      \
    static int c;                     \
    x() { ++c; }                      \
    ~x() { --c; }                     \
  };                                  \
  int x::c = 0

DEFAULT_DYNAMIC_STATE_IMPL(A);
DEFAULT_DYNAMIC_STATE_IMPL(A_1);
DEFAULT_DYNAMIC_STATE_IMPL(A_2);
DEFAULT_DYNAMIC_STATE_IMPL(A_2_1);
DEFAULT_DYNAMIC_STATE_IMPL(A_2_2);
DEFAULT_DYNAMIC_STATE_IMPL(B);
DEFAULT_DYNAMIC_STATE_IMPL(B_1);
DEFAULT_DYNAMIC_STATE_IMPL(B_1_1);
DEFAULT_DYNAMIC_STATE_IMPL(B_1_2);
DEFAULT_DYNAMIC_STATE_IMPL(B_2);
DEFAULT_DYNAMIC_STATE_IMPL(B_2_1);
DEFAULT_DYNAMIC_STATE_IMPL(B_2_2);

}  // namespace test

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <>
struct Guard<test::A_2> {
  static void execute(test::FSM::GuardControl& control) {
    control.cancelPendingTransitions();
  }
};

namespace test {
STATIC_ASSERT_EQ(FSM::Instance::STATE_COUNT, 13);
STATIC_ASSERT_EQ(FSM::Instance::COMPO_REGIONS, 5);
STATIC_ASSERT_EQ(FSM::Instance::COMPO_PRONGS, 10);
STATIC_ASSERT_EQ(FSM::Instance::ORTHO_REGIONS, 1);
STATIC_ASSERT_EQ(FSM::Instance::ORTHO_UNITS, 1);

const Types all = {
    FSM::stateId<A>(),
    FSM::stateId<A_1>(),
    FSM::stateId<A_2>(),
    FSM::stateId<A_2_1>(),
    FSM::stateId<A_2_2>(),
    FSM::stateId<B>(),
    FSM::stateId<B_1>(),
    FSM::stateId<B_1_1>(),
    FSM::stateId<B_1_2>(),
    FSM::stateId<B_2>(),
    FSM::stateId<B_2_1>(),
    FSM::stateId<B_2_2>(),
};

template <typename... Ts>
using TypeList = kerfuffle::detail::ITL_<Ts...>;

template <typename TState,
          typename TAactiveList,
          bool = TAactiveList::template contains<TState>()>
struct CheckActive {
  CheckActive() { ConstructorAssert(); }
  void ConstructorAssert() { ASSERT_TRUE(TState::c == 0); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TState, typename TAactiveList>
struct CheckActive<TState, TAactiveList, true> {
  CheckActive() { ConstructorAssert(); }
  void ConstructorAssert() { ASSERT_TRUE(TState::c == 1); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TAactiveList, bool BContains>
struct CheckActive<void, TAactiveList, BContains> {
  CheckActive() = default;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TAactiveList>
struct CheckActive<void, TAactiveList, true> {
  CheckActive() = default;
};

//------------------------------------------------------------------------------

template <typename, typename>
struct CheckActives;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... TAllStates, typename... TActiveStates>
struct CheckActives<TypeList<TAllStates...>, TypeList<TActiveStates...>>
    : CheckActive<TAllStates, TypeList<TActiveStates...>>... {
  CheckActives() : CheckActive<TAllStates, TypeList<TActiveStates...>>()... {}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... TActiveStates>
void checkActive() {
  CheckActives<FSM::StateList, TypeList<TActiveStates...>>{};
}

class DynamicTest : public testing::Test {};

TEST_F(DynamicTest, GeneralTest) {
  Logger logger;

  {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    FSM::Instance machine{&logger};
    {
      logger.assertSequence({
          {kerfuffle::StateID{0}, Event::ENTRY_GUARD},
          {FSM::stateId<A>(), Event::ENTRY_GUARD},
          {FSM::stateId<A_1>(), Event::ENTRY_GUARD},

          {kerfuffle::StateID{0}, Event::CONSTRUCT},
          {FSM::stateId<A>(), Event::CONSTRUCT},
          {FSM::stateId<A_1>(), Event::CONSTRUCT},

          {kerfuffle::StateID{0}, Event::ENTER},
          {FSM::stateId<A>(), Event::ENTER},
          {FSM::stateId<A_1>(), Event::ENTER},
      });

      assertActive(machine,
                   all,
                   {
                       FSM::stateId<A>(),
                       FSM::stateId<A_1>(),
                   });

      checkActive<A, A_1>();

      assertResumable(machine, all, {});
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    machine.changeTo<A_2>();
    machine.update();
    {
      logger.assertSequence({
          {Event::CHANGE, FSM::stateId<A_2>()},

          {kerfuffle::StateID{0}, Event::UPDATE},
          {FSM::stateId<A>(), Event::UPDATE},
          {FSM::stateId<A_1>(), Event::UPDATE},

          {FSM::stateId<A_1>(), Event::EXIT_GUARD},
          {FSM::stateId<A_2>(), Event::ENTRY_GUARD},
          {FSM::stateId<A_2>(), Event::CANCEL_PENDING},
      });

      checkActive<A, A_1>();

      assertResumable(machine, all, {});
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  }

  logger.assertSequence({
      {FSM::stateId<A_1>(), Event::EXIT},
      {FSM::stateId<A>(), Event::EXIT},
      {kerfuffle::StateID{0}, Event::EXIT},

      {FSM::stateId<A_1>(), Event::DESTRUCT},
      {FSM::stateId<A>(), Event::DESTRUCT},
      {kerfuffle::StateID{0}, Event::DESTRUCT},
  });
}
}  // namespace test
}  // namespace kerfuffle