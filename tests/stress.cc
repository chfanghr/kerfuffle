//
// Created by 方泓睿 on 2020/8/7.
//

#include "helper.h"

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "cert-err58-cpp"

namespace kerfuffle {
namespace test {
using M = kerfuffle::Machine;

class StressTest : public testing::Test {};

using FSM = M::Root<
    S(Apex),
    S(S1),
    M::Orthogonal<
        S(O2),
        M::Composite<S(O2_C1),
                     S(O2_C1_S1),
                     S(O2_C1_S2),
                     M::Orthogonal<S(O2_C1_O3),
                                   M::Composite<S(O2_C1_O3_C1),
                                                S(O2_C1_O3_C1_S1),
                                                S(O2_C1_O3_C1_S2)>,
                                   M::Composite<S(O2_C1_O3_C2),
                                                S(O2_C1_O3_C2_S1),
                                                S(O2_C1_O3_C2_S2),
                                                S(O2_C1_O3_C2_S3)> > >,
        M::Composite<S(O2_C2),
                     S(O2_C2_S1),
                     M::Composite<S(O2_C2_C2), S(O2_C2_C2_S1), S(O2_C2_C2_S2)>,
                     M::Composite<S(O2_C2_C3), S(O2_C2_C3_S1), S(O2_C2_C3_S2)>,
                     M::Composite<S(O2_C2_C4), S(O2_C2_C4_S1), S(O2_C2_C4_S2)>,
                     S(O2_C2_S5)> >,
    S(S3)>;

STATIC_ASSERT_EQ(FSM::regionId<Apex>(), 0);
STATIC_ASSERT_EQ(FSM::regionId<O2>(), 1);
STATIC_ASSERT_EQ(FSM::regionId<O2_C1>(), 2);
STATIC_ASSERT_EQ(FSM::regionId<O2_C1_O3>(), 3);
STATIC_ASSERT_EQ(FSM::regionId<O2_C1_O3_C1>(), 4);
STATIC_ASSERT_EQ(FSM::regionId<O2_C1_O3_C2>(), 5);
STATIC_ASSERT_EQ(FSM::regionId<O2_C2>(), 6);
STATIC_ASSERT_EQ(FSM::regionId<O2_C2_C2>(), 7);
STATIC_ASSERT_EQ(FSM::regionId<O2_C2_C3>(), 8);
STATIC_ASSERT_EQ(FSM::regionId<O2_C2_C4>(), 9);

STATIC_ASSERT_EQ(FSM::stateId<Apex>(), 0);
STATIC_ASSERT_EQ(FSM::stateId<S1>(), 1);
STATIC_ASSERT_EQ(FSM::stateId<O2>(), 2);
STATIC_ASSERT_EQ(FSM::stateId<O2_C1>(), 3);
STATIC_ASSERT_EQ(FSM::stateId<O2_C1_S1>(), 4);
STATIC_ASSERT_EQ(FSM::stateId<O2_C1_S2>(), 5);
STATIC_ASSERT_EQ(FSM::stateId<O2_C1_O3>(), 6);
STATIC_ASSERT_EQ(FSM::stateId<O2_C1_O3_C1>(), 7);
STATIC_ASSERT_EQ(FSM::stateId<O2_C1_O3_C1_S1>(), 8);
STATIC_ASSERT_EQ(FSM::stateId<O2_C1_O3_C1_S2>(), 9);
STATIC_ASSERT_EQ(FSM::stateId<O2_C1_O3_C2>(), 10);
STATIC_ASSERT_EQ(FSM::stateId<O2_C1_O3_C2_S1>(), 11);
STATIC_ASSERT_EQ(FSM::stateId<O2_C1_O3_C2_S2>(), 12);
STATIC_ASSERT_EQ(FSM::stateId<O2_C1_O3_C2_S3>(), 13);
STATIC_ASSERT_EQ(FSM::stateId<O2_C2>(), 14);
STATIC_ASSERT_EQ(FSM::stateId<O2_C2_S1>(), 15);
STATIC_ASSERT_EQ(FSM::stateId<O2_C2_C2>(), 16);
STATIC_ASSERT_EQ(FSM::stateId<O2_C2_C2_S1>(), 17);
STATIC_ASSERT_EQ(FSM::stateId<O2_C2_C2_S2>(), 18);
STATIC_ASSERT_EQ(FSM::stateId<O2_C2_C3>(), 19);
STATIC_ASSERT_EQ(FSM::stateId<O2_C2_C3_S1>(), 20);
STATIC_ASSERT_EQ(FSM::stateId<O2_C2_C3_S2>(), 21);
STATIC_ASSERT_EQ(FSM::stateId<O2_C2_C4>(), 22);
STATIC_ASSERT_EQ(FSM::stateId<O2_C2_C4_S1>(), 23);
STATIC_ASSERT_EQ(FSM::stateId<O2_C2_C4_S2>(), 24);
STATIC_ASSERT_EQ(FSM::stateId<O2_C2_S5>(), 25);
STATIC_ASSERT_EQ(FSM::stateId<S3>(), 26);

DEFAULT_STATE_IMPL(Apex);
DEFAULT_STATE_IMPL(S1);
DEFAULT_STATE_IMPL(O2);
DEFAULT_STATE_IMPL(O2_C1);
DEFAULT_STATE_IMPL(O2_C1_S1);
DEFAULT_STATE_IMPL(O2_C1_S2);
DEFAULT_STATE_IMPL(O2_C1_O3);
DEFAULT_STATE_IMPL(O2_C1_O3_C1);
DEFAULT_STATE_IMPL(O2_C1_O3_C1_S1);
DEFAULT_STATE_IMPL(O2_C1_O3_C1_S2);
DEFAULT_STATE_IMPL(O2_C1_O3_C2);
DEFAULT_STATE_IMPL(O2_C1_O3_C2_S1);
DEFAULT_STATE_IMPL(O2_C1_O3_C2_S2);
DEFAULT_STATE_IMPL(O2_C1_O3_C2_S3);
DEFAULT_STATE_IMPL(O2_C2);
DEFAULT_STATE_IMPL(O2_C2_S1);
DEFAULT_STATE_IMPL(O2_C2_C2);
DEFAULT_STATE_IMPL(O2_C2_C2_S1);
DEFAULT_STATE_IMPL(O2_C2_C2_S2);
DEFAULT_STATE_IMPL(O2_C2_C3);
DEFAULT_STATE_IMPL(O2_C2_C3_S1);
DEFAULT_STATE_IMPL(O2_C2_C3_S2);
DEFAULT_STATE_IMPL(O2_C2_C4);
DEFAULT_STATE_IMPL(O2_C2_C4_S1);
DEFAULT_STATE_IMPL(O2_C2_C4_S2);
DEFAULT_STATE_IMPL(O2_C2_S5);
DEFAULT_STATE_IMPL(S3);

static_assert(FSM::Instance::STATE_COUNT == 27, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS == 8, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS == 22, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS == 2, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS == 2, "ORTHO_UNITS");

const Types all = {
    FSM::stateId<Apex>(),
    FSM::stateId<S1>(),
    FSM::stateId<O2>(),
    FSM::stateId<O2_C1>(),
    FSM::stateId<O2_C1_S1>(),
    FSM::stateId<O2_C1_S2>(),
    FSM::stateId<O2_C1_O3>(),
    FSM::stateId<O2_C1_O3_C1>(),
    FSM::stateId<O2_C1_O3_C1_S1>(),
    FSM::stateId<O2_C1_O3_C1_S2>(),
    FSM::stateId<O2_C1_O3_C2>(),
    FSM::stateId<O2_C1_O3_C2_S1>(),
    FSM::stateId<O2_C1_O3_C2_S2>(),
    FSM::stateId<O2_C1_O3_C2_S3>(),
    FSM::stateId<O2_C2>(),
    FSM::stateId<O2_C2_S1>(),
    FSM::stateId<O2_C2_C2>(),
    FSM::stateId<O2_C2_C2_S1>(),
    FSM::stateId<O2_C2_C2_S2>(),
    FSM::stateId<O2_C2_C3>(),
    FSM::stateId<O2_C2_C3_S1>(),
    FSM::stateId<O2_C2_C3_S2>(),
    FSM::stateId<O2_C2_C4>(),
    FSM::stateId<O2_C2_C4_S1>(),
    FSM::stateId<O2_C2_C4_S2>(),
    FSM::stateId<O2_C2_S5>(),
    FSM::stateId<S3>(),
};

TEST_F(StressTest, GeneralTest) {
  FSM::Instance machine{};

  // FSM is activated initially
  assertActive(machine,
               all,
               {
                   FSM::stateId<Apex>(),
                   FSM::stateId<S1>(),
               });

  assertResumable(machine, all, {});
}

}  // namespace test
}  // namespace kerfuffle
#pragma clang diagnostic pop