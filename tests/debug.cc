//
// Created by 方泓睿 on 8/12/20.
//

#define KERFUFFLE_ENABLE_STRUCTURE_REPORT
#define KERFUFFLE_ENABLE_UTILITY_THEORY
#define KERFUFFLE_ENABLE_VERBOSE_DEBUG_LOG
#include "helper.h"

namespace kerfuffle {
namespace test {
struct Empty {};

using M = kerfuffle::MachineT<
    kerfuffle::Config::ContextT<Empty>::RandomT<kerfuffle::XoShiRo128Plus>>;

using FSM = M::Root<S(Apex),
                    S(I),
                    M::Orthogonal<S(O),
                                  M::Resumable<S(R), S(R_1), S(R_2)>,
                                  M::Composite<S(C), S(C_1), S(C_2)>,
                                  M::Utilitarian<S(U), S(U_1), S(U_2)>,
                                  M::Random<S(N), S(N_1), S(N_2)>>>;

STATIC_ASSERT_EQ(FSM::regionId<Apex>(), 0);
STATIC_ASSERT_EQ(FSM::regionId<O>(), 1);
STATIC_ASSERT_EQ(FSM::regionId<R>(), 2);
STATIC_ASSERT_EQ(FSM::regionId<C>(), 3);
STATIC_ASSERT_EQ(FSM::regionId<U>(), 4);
STATIC_ASSERT_EQ(FSM::regionId<N>(), 5);

STATIC_ASSERT_EQ(FSM::stateId<Apex>(), 0);
STATIC_ASSERT_EQ(FSM::stateId<I>(), 1);
STATIC_ASSERT_EQ(FSM::stateId<O>(), 2);
STATIC_ASSERT_EQ(FSM::stateId<R>(), 3);
STATIC_ASSERT_EQ(FSM::stateId<R_1>(), 4);
STATIC_ASSERT_EQ(FSM::stateId<R_2>(), 5);
STATIC_ASSERT_EQ(FSM::stateId<C>(), 6);
STATIC_ASSERT_EQ(FSM::stateId<C_1>(), 7);
STATIC_ASSERT_EQ(FSM::stateId<C_2>(), 8);
STATIC_ASSERT_EQ(FSM::stateId<U>(), 9);
STATIC_ASSERT_EQ(FSM::stateId<U_1>(), 10);
STATIC_ASSERT_EQ(FSM::stateId<U_2>(), 11);
STATIC_ASSERT_EQ(FSM::stateId<N>(), 12);
STATIC_ASSERT_EQ(FSM::stateId<N_1>(), 13);
STATIC_ASSERT_EQ(FSM::stateId<N_2>(), 14);

DEFAULT_STATE_IMPL(Apex);
DEFAULT_STATE_IMPL(I);
DEFAULT_STATE_IMPL(O);
DEFAULT_STATE_IMPL(R);
DEFAULT_STATE_IMPL(R_1);
DEFAULT_STATE_IMPL(R_2);
DEFAULT_STATE_IMPL(C);
DEFAULT_STATE_IMPL(C_1);
DEFAULT_STATE_IMPL(C_2);
DEFAULT_STATE_IMPL(U);
DEFAULT_STATE_IMPL(U_1);
DEFAULT_STATE_IMPL(U_2);
DEFAULT_STATE_IMPL(N);
DEFAULT_STATE_IMPL(N_1);
DEFAULT_STATE_IMPL(N_2);

STATIC_ASSERT_EQ(FSM::Instance::STATE_COUNT, 15);
STATIC_ASSERT_EQ(FSM::Instance::COMPO_REGIONS, 5);
STATIC_ASSERT_EQ(FSM::Instance::COMPO_PRONGS, 10);
STATIC_ASSERT_EQ(FSM::Instance::ORTHO_REGIONS, 1);
STATIC_ASSERT_EQ(FSM::Instance::ORTHO_UNITS, 1);

using StructureReference = std::vector<kerfuffle::StructureEntry>;

void assertStructure(const FSM::Instance::Structure& structure,
                     const StructureReference&       reference) {
  const auto count = std::max((std::size_t)structure.count(), reference.size());

  for (uint16_t i = 0; i < count; ++i) {
    ASSERT_TRUE(i < structure.count());
    ASSERT_TRUE(i < reference.size());

    if (i < structure.count() && i < reference.size()) {
      ASSERT_TRUE(structure[i].isActive == reference[i].isActive);
      ASSERT_TRUE(wcscmp(structure[i].prefix, reference[i].prefix) == 0);

#ifdef _MSC_VER
      ASSERT_TRUE(strcmp(structure[i].name, reference[i].name) == 0);
#endif
    }
  }
}

using ActivityReference = std::vector<char>;

void assertActivity(const FSM::Instance::ActivityHistory& activity,
                    const ActivityReference&              reference) {
  const auto count = std::max((std::size_t)activity.count(), reference.size());

  for (uint16_t i = 0; i < count; ++i) {
    ASSERT_TRUE(i < activity.count());
    ASSERT_TRUE(i < reference.size());

    if (i < activity.count() && i < reference.size()) {
      ASSERT_TRUE(activity[i] == reference[i]);
    }
  }
}

const Types all = {
    FSM::stateId<I>(),
    FSM::stateId<O>(),
    FSM::stateId<R>(),
    FSM::stateId<R_1>(),
    FSM::stateId<R_2>(),
    FSM::stateId<C>(),
    FSM::stateId<C_1>(),
    FSM::stateId<C_2>(),
    FSM::stateId<U>(),
    FSM::stateId<U_1>(),
    FSM::stateId<U_2>(),
};

class DebugTest : public testing::Test {};

TEST_F(DebugTest, GeneralTest) {
  Empty                                         context;
  kerfuffle::XoShiRo128Plus                     generator{0};
  LoggerT<kerfuffle::FEATURE_TAG, float, Empty> logger;

  {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    FSM::Instance machine{context, generator, &logger};
    {
      logger.assertSequence({
          {FSM::stateId<Apex>(), Event::ENTRY_GUARD},
          {FSM::stateId<I>(), Event::ENTRY_GUARD},

          {FSM::stateId<Apex>(), Event::CONSTRUCT},
          {FSM::stateId<I>(), Event::CONSTRUCT},

          {FSM::stateId<Apex>(), Event::ENTER},
          {FSM::stateId<I>(), Event::ENTER},
      });

      assertActive(machine,
                   all,
                   {
                       FSM::stateId<Apex>(),
                       FSM::stateId<I>(),
                   });

      assertResumable(machine, all, {});

      // clang-format off
      assertStructure(machine.structure(), {
          kerfuffle::StructureEntry{ true,  L"", "Apex"},
          kerfuffle::StructureEntry{ true,  L" ├ ", "I"},
          kerfuffle::StructureEntry{ false, L" └ ", "O"},
          kerfuffle::StructureEntry{ false, L"   ╟ ", "R"},
          kerfuffle::StructureEntry{ false, L"   ║ ├ ", "R_1"},
          kerfuffle::StructureEntry{ false, L"   ║ └ ", "R_2"},
          kerfuffle::StructureEntry{ false, L"   ╟ ", "C"},
          kerfuffle::StructureEntry{ false, L"   ║ ├ ", "C_1"},
          kerfuffle::StructureEntry{ false, L"   ║ └ ", "C_2"},
          kerfuffle::StructureEntry{ false, L"   ╟ ", "U"},
          kerfuffle::StructureEntry{ false, L"   ║ ├ ", "U_1"},
          kerfuffle::StructureEntry{ false, L"   ║ └ ", "U_2"},
          kerfuffle::StructureEntry{ false, L"   ╙ ", "N"},
          kerfuffle::StructureEntry{ false, L"     ├ ", "N_1"},
          kerfuffle::StructureEntry{ false, L"     └ ", "N_2"},
      });
      // clang-format on

      assertActivity(machine.activityHistory(),
                     {
                         (char)+1,
                         (char)+1,
                         (char)-1,
                         (char)-1,
                         (char)-1,
                         (char)-1,
                         (char)-1,
                         (char)-1,
                         (char)-1,
                         (char)-1,
                         (char)-1,
                         (char)-1,
                         (char)-1,
                         (char)-1,
                         (char)-1,
                     });
    }

    machine.changeTo<O>();
    machine.update();
    {
      logger.assertSequence({
          {Event::CHANGE, FSM::stateId<O>()},

          {FSM::stateId<Apex>(), Event::UPDATE},
          {FSM::stateId<I>(), Event::UPDATE},

          {FSM::stateId<U_1>(), Event::UTILITY},
          {FSM::stateId<U_2>(), Event::UTILITY},
          {FSM::stateId<U>(), Event::UTILITY_RESOLUTION, 0},

          {FSM::stateId<N_1>(), Event::RANK},
          {FSM::stateId<N_2>(), Event::RANK},
          {FSM::stateId<N_1>(), Event::UTILITY},
          {FSM::stateId<N_2>(), Event::UTILITY},
          {FSM::stateId<N>(), Event::RANDOM_RESOLUTION, 1},

          {FSM::stateId<I>(), Event::EXIT_GUARD},

          {FSM::stateId<O>(), Event::ENTRY_GUARD},
          {FSM::stateId<R>(), Event::ENTRY_GUARD},
          {FSM::stateId<R_1>(), Event::ENTRY_GUARD},
          {FSM::stateId<C>(), Event::ENTRY_GUARD},
          {FSM::stateId<C_1>(), Event::ENTRY_GUARD},
          {FSM::stateId<U>(), Event::ENTRY_GUARD},
          {FSM::stateId<U_1>(), Event::ENTRY_GUARD},
          {FSM::stateId<N>(), Event::ENTRY_GUARD},
          {FSM::stateId<N_2>(), Event::ENTRY_GUARD},

          {FSM::stateId<I>(), Event::EXIT},

          {FSM::stateId<I>(), Event::DESTRUCT},
          {FSM::stateId<O>(), Event::CONSTRUCT},
          {FSM::stateId<R>(), Event::CONSTRUCT},
          {FSM::stateId<R_1>(), Event::CONSTRUCT},
          {FSM::stateId<C>(), Event::CONSTRUCT},
          {FSM::stateId<C_1>(), Event::CONSTRUCT},
          {FSM::stateId<U>(), Event::CONSTRUCT},
          {FSM::stateId<U_1>(), Event::CONSTRUCT},
          {FSM::stateId<N>(), Event::CONSTRUCT},
          {FSM::stateId<N_2>(), Event::CONSTRUCT},

          {FSM::stateId<O>(), Event::ENTER},
          {FSM::stateId<R>(), Event::ENTER},
          {FSM::stateId<R_1>(), Event::ENTER},
          {FSM::stateId<C>(), Event::ENTER},
          {FSM::stateId<C_1>(), Event::ENTER},
          {FSM::stateId<U>(), Event::ENTER},
          {FSM::stateId<U_1>(), Event::ENTER},
          {FSM::stateId<N>(), Event::ENTER},
          {FSM::stateId<N_2>(), Event::ENTER},
      });

      assertActive(machine,
                   all,
                   {
                       FSM::stateId<O>(),
                       FSM::stateId<R>(),
                       FSM::stateId<R_1>(),
                       FSM::stateId<C>(),
                       FSM::stateId<C_1>(),
                       FSM::stateId<U>(),
                       FSM::stateId<U_1>(),
                       FSM::stateId<N>(),
                       FSM::stateId<N_2>(),
                   });

      assertResumable(machine,
                      all,
                      {
                          FSM::stateId<I>(),
                      });

      // clang-format off
      assertStructure(machine.structure(), {
          kerfuffle::StructureEntry{ true,  L"", "Apex"},
          kerfuffle::StructureEntry{ false, L" ├ ", "I"},
          kerfuffle::StructureEntry{ true,  L" └ ", "O"},
          kerfuffle::StructureEntry{ true,  L"   ╟ ", "R"},
          kerfuffle::StructureEntry{ true,  L"   ║ ├ ", "R_1"},
          kerfuffle::StructureEntry{ false, L"   ║ └ ", "R_2"},
          kerfuffle::StructureEntry{ true,  L"   ╟ ", "C"},
          kerfuffle::StructureEntry{ true,  L"   ║ ├ ", "C_1"},
          kerfuffle::StructureEntry{ false, L"   ║ └ ", "C_2"},
          kerfuffle::StructureEntry{ true,  L"   ╟ ", "U"},
          kerfuffle::StructureEntry{ true,  L"   ║ ├ ", "U_1"},
          kerfuffle::StructureEntry{ false, L"   ║ └ ", "U_2"},
          kerfuffle::StructureEntry{ true,  L"   ╙ ", "N"},
          kerfuffle::StructureEntry{ false, L"     ├ ", "N_1"},
          kerfuffle::StructureEntry{ true,  L"     └ ", "N_2"},
      });
      // clang-format on

      assertActivity(machine.activityHistory(),
                     {
                         (char)+2,
                         (char)-1,
                         (char)+1,
                         (char)+1,
                         (char)+1,
                         (char)-2,
                         (char)+1,
                         (char)+1,
                         (char)-2,
                         (char)+1,
                         (char)+1,
                         (char)-2,
                         (char)+1,
                         (char)-2,
                         (char)+1,
                     });
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  }

  logger.assertSequence({
      {FSM::stateId<R_1>(), Event::EXIT},
      {FSM::stateId<R>(), Event::EXIT},
      {FSM::stateId<C_1>(), Event::EXIT},
      {FSM::stateId<C>(), Event::EXIT},
      {FSM::stateId<U_1>(), Event::EXIT},
      {FSM::stateId<U>(), Event::EXIT},
      {FSM::stateId<N_2>(), Event::EXIT},
      {FSM::stateId<N>(), Event::EXIT},
      {FSM::stateId<O>(), Event::EXIT},
      {kerfuffle::StateID{0}, Event::EXIT},

      {FSM::stateId<R_1>(), Event::DESTRUCT},
      {FSM::stateId<R>(), Event::DESTRUCT},
      {FSM::stateId<C_1>(), Event::DESTRUCT},
      {FSM::stateId<C>(), Event::DESTRUCT},
      {FSM::stateId<U_1>(), Event::DESTRUCT},
      {FSM::stateId<U>(), Event::DESTRUCT},
      {FSM::stateId<N_2>(), Event::DESTRUCT},
      {FSM::stateId<N>(), Event::DESTRUCT},
      {FSM::stateId<O>(), Event::DESTRUCT},
      {kerfuffle::StateID{0}, Event::DESTRUCT},
  });
}
}  // namespace test
}  // namespace kerfuffle
