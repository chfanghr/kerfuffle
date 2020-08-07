//
// Created by 方泓睿 on 2020/8/7.
//

#include "helper.h"

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "cert-err58-cpp"

namespace kerfuffle {
namespace test {

using M = kerfuffle::Machine;

using FSM = M::PeerRoot<
    M::Composite<S(A), S(A_1), M::Composite<S(A_2), S(A_2_1), S(A_2_2)> >,
    M::Orthogonal<S(B),
                  M::Composite<S(B_1), S(B_1_1), S(B_1_2)>,
                  M::Composite<S(B_2), S(B_2_1), S(B_2_2)> > >;

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

struct A : FSM::State {
  const kerfuffle::StateID id = FSM::stateId<A>();
};
struct A_1 : FSM::State {
  const kerfuffle::StateID id = FSM::stateId<A_1>();
};
struct A_2 : FSM::State {
  const kerfuffle::StateID id = FSM::stateId<A_2>();
};
struct A_2_1 : FSM::State {
  const kerfuffle::StateID id = FSM::stateId<A_2_1>();
};
struct A_2_2 : FSM::State {
  const kerfuffle::StateID id = FSM::stateId<A_2_2>();
};
struct B : FSM::State {
  const kerfuffle::StateID id = FSM::stateId<B>();
};
struct B_1 : FSM::State {
  const kerfuffle::StateID id = FSM::stateId<B_1>();
};
struct B_1_1 : FSM::State {
  const kerfuffle::StateID id = FSM::stateId<B_1_1>();
};
struct B_1_2 : FSM::State {
  const kerfuffle::StateID id = FSM::stateId<B_1_2>();
};
struct B_2 : FSM::State {
  const kerfuffle::StateID id = FSM::stateId<B_2>();
};
struct B_2_1 : FSM::State {
  const kerfuffle::StateID id = FSM::stateId<B_2_1>();
};
struct B_2_2 : FSM::State {
  const kerfuffle::StateID id = FSM::stateId<B_2_2>();
};

static_assert(FSM::Instance::STATE_COUNT == 13, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS == 5, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS == 1, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS == 1, "ORTHO_UNITS");

class AccessTest : public testing::Test {};

TEST_F(AccessTest, GeneralTest) {
  FSM::Instance machine;

  ASSERT_EQ(machine.access<A>().id, FSM::stateId<A>());
  ASSERT_EQ(machine.access<A_1>().id, FSM::stateId<A_1>());
  ASSERT_EQ(machine.access<A_2>().id, FSM::stateId<A_2>());
  ASSERT_EQ(machine.access<A_2_1>().id, FSM::stateId<A_2_1>());
  ASSERT_EQ(machine.access<A_2_2>().id, FSM::stateId<A_2_2>());
  ASSERT_EQ(machine.access<B>().id, FSM::stateId<B>());
  ASSERT_EQ(machine.access<B_1>().id, FSM::stateId<B_1>());
  ASSERT_EQ(machine.access<B_1_1>().id, FSM::stateId<B_1_1>());
  ASSERT_EQ(machine.access<B_1_2>().id, FSM::stateId<B_1_2>());
  ASSERT_EQ(machine.access<B_2>().id, FSM::stateId<B_2>());
  ASSERT_EQ(machine.access<B_2_1>().id, FSM::stateId<B_2_1>());
  ASSERT_EQ(machine.access<B_2_2>().id, FSM::stateId<B_2_2>());
}
}  // namespace test
}  // namespace kerfuffle
#pragma clang diagnostic pop