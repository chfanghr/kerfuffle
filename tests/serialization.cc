//
// Created by 方泓睿 on 8/4/20.
//

#define KERFUFFLE_ENABLE_SERIALIZATION
#define KERFUFFLE_ENABLE_VERBOSE_DEBUG_LOG
#include "helper.h"

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "cert-err58-cpp"

namespace kerfuffle {
namespace test {
class SerializationTest : public ::testing::Test {};

using M = kerfuffle::Machine;

namespace server {

using FSM = M::PeerRoot<
    M::Composite<S(C1),
                 S(C1_S1),
                 M::Composite<S(C1_C2), S(C1_C2_S1), S(C1_C2_S2)> >,
    M::Orthogonal<S(O2),
                  M::Composite<S(O2_C1), S(O2_C1_S1), S(O2_C1_S2)>,
                  M::Composite<S(O2_C2), S(O2_C2_S1), S(O2_C2_S2)> > >;

TEST_F(SerializationTest, CheckServerStateID) {
#define ERROR_MSG "should equal"
  static_assert(FSM::regionId<C1>() == 1, ERROR_MSG);
  static_assert(FSM::regionId<C1_C2>() == 2, ERROR_MSG);
  static_assert(FSM::regionId<O2>() == 3, ERROR_MSG);
  static_assert(FSM::regionId<O2_C1>() == 4, ERROR_MSG);
  static_assert(FSM::regionId<O2_C2>() == 5, ERROR_MSG);

  static_assert(FSM::stateId<C1>() == 1, ERROR_MSG);
  static_assert(FSM::stateId<C1_S1>() == 2, ERROR_MSG);
  static_assert(FSM::stateId<C1_C2>() == 3, ERROR_MSG);
  static_assert(FSM::stateId<C1_C2_S1>() == 4, ERROR_MSG);
  static_assert(FSM::stateId<C1_C2_S2>() == 5, ERROR_MSG);
  static_assert(FSM::stateId<O2>() == 6, ERROR_MSG);
  static_assert(FSM::stateId<O2_C1>() == 7, ERROR_MSG);
  static_assert(FSM::stateId<O2_C1_S1>() == 8, ERROR_MSG);
  static_assert(FSM::stateId<O2_C1_S2>() == 9, ERROR_MSG);
  static_assert(FSM::stateId<O2_C2>() == 10, ERROR_MSG);
  static_assert(FSM::stateId<O2_C2_S1>() == 11, ERROR_MSG);
  static_assert(FSM::stateId<O2_C2_S2>() == 12, ERROR_MSG);

#undef ERROR_MSG
}

DEFAULT_STATE_IMPL(C1);
DEFAULT_STATE_IMPL(C1_S1);
DEFAULT_STATE_IMPL(C1_C2);
DEFAULT_STATE_IMPL(C1_C2_S1);
DEFAULT_STATE_IMPL(C1_C2_S2);
DEFAULT_STATE_IMPL(O2);
DEFAULT_STATE_IMPL(O2_C1);
DEFAULT_STATE_IMPL(O2_C1_S1);
DEFAULT_STATE_IMPL(O2_C1_S2);
DEFAULT_STATE_IMPL(O2_C2);
DEFAULT_STATE_IMPL(O2_C2_S1);
DEFAULT_STATE_IMPL(O2_C2_S2);

TEST_F(SerializationTest, CheckServerConfig) {
  ASSERT_EQ_CONSTEXPR(FSM::Instance::STATE_COUNT, 13);
  ASSERT_EQ_CONSTEXPR(FSM::Instance::REGION_COUNT, 6);
  ASSERT_EQ_CONSTEXPR(FSM::Instance::COMPO_REGIONS, 5);
  ASSERT_EQ_CONSTEXPR(FSM::Instance::COMPO_PRONGS, 10);
  ASSERT_EQ_CONSTEXPR(FSM::Instance::ORTHO_REGIONS, 1);
  ASSERT_EQ_CONSTEXPR(FSM::Instance::ORTHO_UNITS, 1);
  ASSERT_EQ_CONSTEXPR(FSM::Instance::ACTIVE_BITS, 3);
  ASSERT_EQ_CONSTEXPR(FSM::Instance::RESUMABLE_BITS, 10);
}

const Types all = {
    FSM::stateId<C1>(),
    FSM::stateId<C1_S1>(),
    FSM::stateId<C1_C2>(),
    FSM::stateId<C1_C2_S1>(),
    FSM::stateId<C1_C2_S2>(),
    FSM::stateId<O2>(),
    FSM::stateId<O2_C1>(),
    FSM::stateId<O2_C1_S1>(),
    FSM::stateId<O2_C1_S2>(),
    FSM::stateId<O2_C2>(),
    FSM::stateId<O2_C2_S1>(),
    FSM::stateId<O2_C2_S2>(),
};

}  // namespace server

namespace client {
using FSM = M::PeerRoot<
    M::Composite<S(C1),
                 S(C1_S1),
                 M::Composite<S(C1_C2), S(C1_C2_S1), S(C1_C2_S2)> >,
    M::Orthogonal<S(O2),
                  M::Composite<S(O2_C1), S(O2_C1_S1), S(O2_C1_S2)>,
                  M::Composite<S(O2_C2), S(O2_C2_S1), S(O2_C2_S2)> > >;

DEFAULT_STATE_IMPL(C1);
DEFAULT_STATE_IMPL(C1_S1);
DEFAULT_STATE_IMPL(C1_C2);
DEFAULT_STATE_IMPL(C1_C2_S1);
DEFAULT_STATE_IMPL(C1_C2_S2);
DEFAULT_STATE_IMPL(O2);
DEFAULT_STATE_IMPL(O2_C1);
DEFAULT_STATE_IMPL(O2_C1_S1);
DEFAULT_STATE_IMPL(O2_C1_S2);
DEFAULT_STATE_IMPL(O2_C2);
DEFAULT_STATE_IMPL(O2_C2_S1);
DEFAULT_STATE_IMPL(O2_C2_S2);

TEST_F(SerializationTest, CheckClientStateID) {
#define ERROR_MSG "should equal"
  static_assert(FSM::regionId<C1>() == 1, ERROR_MSG);
  static_assert(FSM::regionId<C1_C2>() == 2, ERROR_MSG);
  static_assert(FSM::regionId<O2>() == 3, ERROR_MSG);
  static_assert(FSM::regionId<O2_C1>() == 4, ERROR_MSG);
  static_assert(FSM::regionId<O2_C2>() == 5, ERROR_MSG);

  static_assert(FSM::stateId<C1>() == 1, ERROR_MSG);
  static_assert(FSM::stateId<C1_S1>() == 2, ERROR_MSG);
  static_assert(FSM::stateId<C1_C2>() == 3, ERROR_MSG);
  static_assert(FSM::stateId<C1_C2_S1>() == 4, ERROR_MSG);
  static_assert(FSM::stateId<C1_C2_S2>() == 5, ERROR_MSG);
  static_assert(FSM::stateId<O2>() == 6, ERROR_MSG);
  static_assert(FSM::stateId<O2_C1>() == 7, ERROR_MSG);
  static_assert(FSM::stateId<O2_C1_S1>() == 8, ERROR_MSG);
  static_assert(FSM::stateId<O2_C1_S2>() == 9, ERROR_MSG);
  static_assert(FSM::stateId<O2_C2>() == 10, ERROR_MSG);
  static_assert(FSM::stateId<O2_C2_S1>() == 11, ERROR_MSG);
  static_assert(FSM::stateId<O2_C2_S2>() == 12, ERROR_MSG);
#undef ERROR_MSG
}

TEST_F(SerializationTest, CheckClientConfig) {
  ASSERT_EQ_CONSTEXPR(FSM::Instance::STATE_COUNT, 13);
  ASSERT_EQ_CONSTEXPR(FSM::Instance::REGION_COUNT, 6);
  ASSERT_EQ_CONSTEXPR(FSM::Instance::COMPO_REGIONS, 5);
  ASSERT_EQ_CONSTEXPR(FSM::Instance::COMPO_PRONGS, 10);
  ASSERT_EQ_CONSTEXPR(FSM::Instance::ORTHO_REGIONS, 1);
  ASSERT_EQ_CONSTEXPR(FSM::Instance::ORTHO_UNITS, 1);
  ASSERT_EQ_CONSTEXPR(FSM::Instance::ACTIVE_BITS, 3);
  ASSERT_EQ_CONSTEXPR(FSM::Instance::RESUMABLE_BITS, 10);
}

const Types all = {
    FSM::stateId<C1>(),
    FSM::stateId<C1_S1>(),
    FSM::stateId<C1_C2>(),
    FSM::stateId<C1_C2_S1>(),
    FSM::stateId<C1_C2_S2>(),
    FSM::stateId<O2>(),
    FSM::stateId<O2_C1>(),
    FSM::stateId<O2_C1_S1>(),
    FSM::stateId<O2_C1_S2>(),
    FSM::stateId<O2_C2>(),
    FSM::stateId<O2_C2_S1>(),
    FSM::stateId<O2_C2_S2>(),
};

}  // namespace client

TEST_F(SerializationTest, TestStoreLoad) {
  Logger logger;

  server::FSM::Instance::SerialBuffer buffer;

  {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    server::FSM::Instance authority;
    client::FSM::Instance replicated{&logger};
    {
      logger.assertSequence({
          {kerfuffle::StateID{0}, Event::ENTRY_GUARD},
          {client::FSM::stateId<client::C1>(), Event::ENTRY_GUARD},
          {client::FSM::stateId<client::C1_S1>(), Event::ENTRY_GUARD},

          {kerfuffle::StateID{0}, Event::CONSTRUCT},
          {client::FSM::stateId<client::C1>(), Event::CONSTRUCT},
          {client::FSM::stateId<client::C1_S1>(), Event::CONSTRUCT},

          {kerfuffle::StateID{0}, Event::ENTER},
          {client::FSM::stateId<client::C1>(), Event::ENTER},
          {client::FSM::stateId<client::C1_S1>(), Event::ENTER},
      });

      assertActive(authority,
                   server::all,
                   {
                       server::FSM::stateId<server::C1>(),
                       server::FSM::stateId<server::C1_S1>(),
                   });

      assertActive(replicated,
                   client::all,
                   {
                       client::FSM::stateId<client::C1>(),
                       client::FSM::stateId<client::C1_S1>(),
                   });

      assertResumable(authority, server::all, {});

      assertResumable(replicated, client::all, {});
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    authority.changeTo<server::O2_C1_S2>();
    authority.update();
    {
      assertActive(authority,
                   server::all,
                   {
                       server::FSM::stateId<server::O2>(),
                       server::FSM::stateId<server::O2_C1>(),
                       server::FSM::stateId<server::O2_C1_S2>(),
                       server::FSM::stateId<server::O2_C2>(),
                       server::FSM::stateId<server::O2_C2_S1>(),
                   });

      assertResumable(authority,
                      server::all,
                      {
                          server::FSM::stateId<server::C1>(),
                          server::FSM::stateId<server::C1_S1>(),
                      });
    }
    authority.save(buffer);

    replicated.load(buffer);
    {
      logger.assertSequence({
          {client::FSM::stateId<client::C1_S1>(), Event::EXIT},
          {client::FSM::stateId<client::C1>(), Event::EXIT},
          {kerfuffle::StateID{0}, Event::EXIT},

          {client::FSM::stateId<client::C1_S1>(), Event::DESTRUCT},
          {client::FSM::stateId<client::C1>(), Event::DESTRUCT},
          {kerfuffle::StateID{0}, Event::DESTRUCT},

          {kerfuffle::StateID{0}, Event::CONSTRUCT},
          {client::FSM::stateId<client::O2>(), Event::CONSTRUCT},
          {client::FSM::stateId<client::O2_C1>(), Event::CONSTRUCT},
          {client::FSM::stateId<client::O2_C1_S2>(), Event::CONSTRUCT},
          {client::FSM::stateId<client::O2_C2>(), Event::CONSTRUCT},
          {client::FSM::stateId<client::O2_C2_S1>(), Event::CONSTRUCT},

          {kerfuffle::StateID{0}, Event::ENTER},
          {client::FSM::stateId<client::O2>(), Event::ENTER},
          {client::FSM::stateId<client::O2_C1>(), Event::ENTER},
          {client::FSM::stateId<client::O2_C1_S2>(), Event::ENTER},
          {client::FSM::stateId<client::O2_C2>(), Event::ENTER},
          {client::FSM::stateId<client::O2_C2_S1>(), Event::ENTER},
      });

      assertActive(replicated,
                   client::all,
                   {
                       client::FSM::stateId<client::O2>(),
                       client::FSM::stateId<client::O2_C1>(),
                       client::FSM::stateId<client::O2_C1_S2>(),
                       client::FSM::stateId<client::O2_C2>(),
                       client::FSM::stateId<client::O2_C2_S1>(),
                   });

      assertResumable(replicated,
                      client::all,
                      {
                          client::FSM::stateId<client::C1>(),
                          client::FSM::stateId<client::C1_S1>(),
                      });
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  }
}
}  // namespace test
}  // namespace kerfuffle
#pragma clang diagnostic pop