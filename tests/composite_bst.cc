//
// Created by 方泓睿 on 2020/8/7.
//

#define KERFUFFLE_ENABLE_VERBOSE_DEBUG_LOG
#include "helper.h"

namespace kerfuffle {
namespace test {

using M = kerfuffle::Machine;

using FSM = M::Root<S(Apex),
                    S(S0),
                    S(S1),
                    S(S2),
                    S(S3),
                    S(S4),
                    S(S5),
                    S(S6),
                    S(S7),
                    S(S8),
                    S(S9)>;

STATIC_ASSERT_EQ(FSM::regionId<Apex>(), 0);

STATIC_ASSERT_EQ(FSM::stateId<Apex>(), 0);
STATIC_ASSERT_EQ(FSM::stateId<S0>(), 1);
STATIC_ASSERT_EQ(FSM::stateId<S1>(), 2);
STATIC_ASSERT_EQ(FSM::stateId<S2>(), 3);
STATIC_ASSERT_EQ(FSM::stateId<S3>(), 4);
STATIC_ASSERT_EQ(FSM::stateId<S4>(), 5);
STATIC_ASSERT_EQ(FSM::stateId<S5>(), 6);
STATIC_ASSERT_EQ(FSM::stateId<S6>(), 7);
STATIC_ASSERT_EQ(FSM::stateId<S7>(), 8);
STATIC_ASSERT_EQ(FSM::stateId<S8>(), 9);
STATIC_ASSERT_EQ(FSM::stateId<S9>(), 10);

DEFAULT_STATE_IMPL(Apex);
DEFAULT_STATE_IMPL(S0);
DEFAULT_STATE_IMPL(S1);
DEFAULT_STATE_IMPL(S2);
DEFAULT_STATE_IMPL(S3);
DEFAULT_STATE_IMPL(S4);
DEFAULT_STATE_IMPL(S5);
DEFAULT_STATE_IMPL(S6);
DEFAULT_STATE_IMPL(S7);
DEFAULT_STATE_IMPL(S8);
DEFAULT_STATE_IMPL(S9);

static_assert(FSM::Instance::STATE_COUNT == 11, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS == 1, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS == 0, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS == 0, "ORTHO_UNITS");

const Types all = {
    FSM::stateId<S0>(),
    FSM::stateId<S1>(),
    FSM::stateId<S2>(),
    FSM::stateId<S3>(),
    FSM::stateId<S4>(),
    FSM::stateId<S5>(),
    FSM::stateId<S6>(),
    FSM::stateId<S7>(),
    FSM::stateId<S8>(),
    FSM::stateId<S9>(),
};

class CompositeBSTTest : public testing::Test {};

TEST_F(CompositeBSTTest, GeneralTest) {
  Logger logger;

  {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    FSM::Instance machine{&logger};
    {
      logger.assertSequence({
          {FSM::stateId<Apex>(), Event::ENTRY_GUARD},
          {FSM::stateId<S0>(), Event::ENTRY_GUARD},

          {FSM::stateId<Apex>(), Event::CONSTRUCT},
          {FSM::stateId<S0>(), Event::CONSTRUCT},

          {FSM::stateId<Apex>(), Event::ENTER},
          {FSM::stateId<S0>(), Event::ENTER},
      });

      assertActive(machine,
                   all,
                   {
                       FSM::stateId<S0>(),
                   });

      assertResumable(machine, all, {});
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    machine.changeTo<S1>();
    machine.update();
    {
      logger.assertSequence({
          {Event::CHANGE, FSM::stateId<S1>()},

          {kerfuffle::StateID{0}, Event::UPDATE},
          {FSM::stateId<S0>(), Event::UPDATE},

          {FSM::stateId<S0>(), Event::EXIT_GUARD},
          {FSM::stateId<S1>(), Event::ENTRY_GUARD},

          {FSM::stateId<S0>(), Event::EXIT},
          {FSM::stateId<S0>(), Event::DESTRUCT},
          {FSM::stateId<S1>(), Event::CONSTRUCT},
          {FSM::stateId<S1>(), Event::ENTER},
      });

      assertActive(machine,
                   all,
                   {
                       FSM::stateId<S1>(),
                   });

      assertResumable(machine,
                      all,
                      {
                          FSM::stateId<S0>(),
                      });
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  }

  logger.assertSequence({
      {FSM::stateId<S1>(), Event::EXIT},
      {kerfuffle::StateID{0}, Event::EXIT},

      {FSM::stateId<S1>(), Event::DESTRUCT},
      {kerfuffle::StateID{0}, Event::DESTRUCT},
  });
}
}  // namespace test
}  // namespace kerfuffle