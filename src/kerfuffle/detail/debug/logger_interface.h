namespace kerfuffle {

#ifdef KERFUFFLE_ENABLE_LOG_INTERFACE

////////////////////////////////////////////////////////////////////////////////

template <typename TContext = EmptyContext KERFUFFLE_IF_UTILITY_THEORY(
              , typename TUtilty = float),
          FeatureTag NFeatureTag = FEATURE_TAG>
struct LoggerInterfaceT {
  using Context = TContext;

#  ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  using Utilty = TUtilty;
#  endif

  using Method         = ::kerfuffle::Method;
  using StateID        = ::kerfuffle::StateID;
  using RegionID       = ::kerfuffle::RegionID;
  using TransitionType = ::kerfuffle::TransitionType;

#  ifdef KERFUFFLE_ENABLE_PLANS
  using StatusEvent = ::kerfuffle::StatusEvent;
#  endif

  virtual void recordMethod(Context& /*context*/,
                            const StateID /*origin*/,
                            const Method /*method*/) {}

  virtual void recordTransition(Context& /*context*/,
                                const StateID /*origin*/,
                                const TransitionType /*transitionType*/,
                                const StateID /*target*/) {}

#  ifdef KERFUFFLE_ENABLE_PLANS

  virtual void recordTaskStatus(Context& /*context*/,
                                const RegionID /*region*/,
                                const StateID /*origin*/,
                                const StatusEvent /*event*/) {}

  virtual void recordPlanStatus(Context& /*context*/,
                                const RegionID /*region*/,
                                const StatusEvent /*event*/) {}

#  endif

  virtual void recordCancelledPending(Context& /*context*/,
                                      const StateID /*origin*/) {}

#  ifdef KERFUFFLE_ENABLE_UTILITY_THEORY

  virtual void recordUtilityResolution(Context& /*context*/,
                                       const StateID /*head*/,
                                       const StateID /*prong*/,
                                       const Utilty /*utilty*/) {}

  virtual void recordRandomResolution(Context& /*context*/,
                                      const StateID /*head*/,
                                      const StateID /*prong*/,
                                      const Utilty /*utilty*/) {}

#  endif
};

////////////////////////////////////////////////////////////////////////////////

#else

template <typename TContext = EmptyContext KERFUFFLE_IF_UTILITY_THEORY(
              , typename TUtilty = float),
          FeatureTag NFeatureTag = FEATURE_TAG>
using LoggerInterfaceT = void;

#endif

using LoggerInterface = LoggerInterfaceT<>;

}  // namespace kerfuffle
