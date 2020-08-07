//
// Created by 方泓睿 on 8/4/20.
//

#pragma once

#define KERFUFFLE_ENABLE_LOG_INTERFACE
#define KERFUFFLE_ENABLE_ASSERT
#include <gtest/gtest.h>
#include <kerfuffle.h>

#include <vector>

#define S(x) struct x

#define DEFAULT_STATE_IMPL(x) \
  struct x : FSM::State {}

template <typename T>
constexpr T access(const T val) {
  return val;
}

#define ASSERT_EQ_CONSTEXPR(x, y) \
  static_assert(access((x)) == access((y)), #x "and" #y "should be equal")

#if defined(KERFUFFLE_ENABLE_UTILITY_THEORY)
#  define IF_UTILITY_THEORY(...) __VA_ARGS__
#else
#  define IF_UTILITY_THEORY(...)
#endif

using Types = std::vector<kerfuffle::StateID>;

struct Event {
  enum Type {
    RANK,
    UTILITY,

    ENTRY_GUARD,
    CONSTRUCT,
    ENTER,
    REENTER,
    UPDATE,
    REACT,
    EXIT_GUARD,
    EXIT,
    DESTRUCT,

    UTILITY_RESOLUTION,
    RANDOM_RESOLUTION,

    TASK_SUCCEED,
    TASK_FAILURE,
    PLAN_SUCCEED,
    PLAN_FAILURE,

    CHANGE,
    RESTART,
    RESUME,

    UTILIZE,
    RANDOMIZE,

    SCHEDULE,
    CANCEL_PENDING,
    COUNT
  };

  kerfuffle::StateID origin, target;
  float              utility;
  Type               type;

  Event(kerfuffle::StateID origin_,
        Type               type_,
        kerfuffle::StateID target_  = kerfuffle::INVALID_STATE_ID,
        const float        utility_ = 0.0f)
      : origin(origin_), type(type_), target(target_), utility(utility_) {}

  Event(Type type_, kerfuffle::StateID target_, const float utility_)
      : origin(kerfuffle::INVALID_STATE_ID),
        type(type_),
        target(target_),
        utility(utility_) {}

  Event() = default;

  friend bool operator==(const Event& lhs, const Event& rhs) {
    return lhs.origin == rhs.origin && lhs.target == rhs.target &&
           lhs.type == rhs.type;
  }
};

using Events = std::vector<Event>;

template <typename TMachine>
void assertActive(TMachine& machine, const Types& all, const Types& toCheck);

template <typename TMachine>
void assertResumable(TMachine& machine, const Types& all, const Types& toCheck);

template <kerfuffle::FeatureTag NFeatureTag = kerfuffle::FEATURE_TAG
                                IF_UTILITY_THEORY(, typename TUtilty = float),
          typename TContext = kerfuffle::EmptyContext>
struct LoggerT
    : kerfuffle::LoggerInterfaceT<TContext IF_UTILITY_THEORY(, TUtilty),
                                  NFeatureTag> {
  static constexpr kerfuffle::FeatureTag FEATURE_TAG = NFeatureTag;

  using Context = TContext;

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  using Utilty = TUtilty;
#endif

  using Interface =
      kerfuffle::LoggerInterfaceT<Context IF_UTILITY_THEORY(, Utilty),
                                  FEATURE_TAG>;

  using Method         = typename Interface::Method;
  using StateID        = typename Interface::StateID;
  using RegionID       = typename Interface::RegionID;
  using TransitionType = typename Interface::TransitionType;

#ifdef KERFUFFLE_ENABLE_PLANS
  using StatusEvent = typename Interface::StatusEvent;
#endif

  void recordMethod(Context&      context,
                    const StateID origin,
                    const Method  method) override;

  void recordTransition(Context&             context,
                        const StateID        origin,
                        const TransitionType transitionType,
                        const StateID        target) override;

#ifdef KERFUFFLE_ENABLE_PLANS

  void recordTaskStatus(Context&          context,
                        const RegionID    region,
                        const StateID     origin,
                        const StatusEvent event) override;

  void recordPlanStatus(Context&          context,
                        const RegionID    region,
                        const StatusEvent event) override;

#endif

  void recordCancelledPending(Context& context, const StateID origin) override;

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY

  void recordUtilityResolution(Context&      context,
                               const StateID head,
                               const StateID prong,
                               const Utilty  utilty) override;

  void recordRandomResolution(Context&      context,
                              const StateID head,
                              const StateID prong,
                              const Utilty  utilty) override;

#endif

  void assertSequence(const Events& reference);

  Events history;
};

using Logger = LoggerT<>;

#include "helper.inl"