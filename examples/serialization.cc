//
// Created by 方泓睿 on 8/4/20.
//

#include <cstdlib>
#include <cassert>

#define KERFUFFLE_ENABLE_SERIALIZATION
#include <kerfuffle.h>

using M = kerfuffle::Machine;

#define S(s) struct s

using FSM = M::PeerRoot<S(StateOne), S(StateTwo)>;

#undef S

struct StateOne : FSM::State {};
struct StateTwo : FSM::State {};

int main() {
  FSM::Instance::SerialBuffer buffer;

  {
    FSM::Instance fsm{};

    fsm.changeTo<StateTwo>();  // request a transition to 'StateTwo'
    fsm.update();              // process transition

    assert(fsm.isActive<StateTwo>());

    fsm.save(buffer);  // serialize fsm state into 'buffer'
  }

  {
    FSM::Instance fsm{};

    assert(fsm.isActive<StateOne>());
    assert(!fsm.isActive<StateTwo>());

    fsm.load(buffer);  // restore fsm state from 'buffer'

    assert(!fsm.isActive<StateOne>());
    assert(fsm.isActive<StateTwo>());
  }

  return EXIT_SUCCESS;
}