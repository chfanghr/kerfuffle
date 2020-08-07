namespace kerfuffle {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class InjectionT {
  template <typename...>
  friend struct B_;

 protected:
  using Context = typename TArgs::Context;

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  using Rank    = typename TArgs::Rank;
  using Utility = typename TArgs::Utility;
#endif

  using StateList  = typename TArgs::StateList;
  using RegionList = typename TArgs::RegionList;

  using Control = ControlT<TArgs>;

  using PlanControl = PlanControlT<TArgs>;

#ifdef KERFUFFLE_ENABLE_PLANS
  using Plan = PlanT<TArgs>;
#endif

  using FullControl  = FullControlT<TArgs>;
  using GuardControl = GuardControlT<TArgs>;

 public:
  KERFUFFLE_INLINE void preEntryGuard(Context&) {}

  KERFUFFLE_INLINE void preEnter(Context&) {}
  KERFUFFLE_INLINE void preReenter(Context&) {}

  KERFUFFLE_INLINE void preUpdate(Context&) {}

  template <typename TEvent>
  KERFUFFLE_INLINE void preReact(const TEvent&, Context&) {}

  KERFUFFLE_INLINE void preExitGuard(Context&) {}

  KERFUFFLE_INLINE void postExit(Context&) {}

  template <typename T>
  static constexpr StateID stateId() {
    return StateList ::template index<T>();
  }

  template <typename T>
  static constexpr RegionID regionId() {
    return (RegionID)RegionList::template index<T>();
  }
};

//------------------------------------------------------------------------------

template <typename...>
struct B_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst, typename... TRest>
struct B_<TFirst, TRest...> : TFirst, B_<TRest...> {
  using typename TFirst::Context;

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  using typename TFirst::Rank;
  using typename TFirst::Utility;
#endif

  using typename TFirst::RegionList;
  using typename TFirst::StateList;

  using typename TFirst::Control;
  using typename TFirst::PlanControl;

#ifdef KERFUFFLE_ENABLE_PLANS
  using typename TFirst::Plan;
#endif

  using typename TFirst::FullControl;
  using typename TFirst::GuardControl;

  using TFirst::regionId;
  using TFirst::stateId;

  KERFUFFLE_INLINE void widePreEntryGuard(Context& context);

  KERFUFFLE_INLINE void widePreEnter(Context& context);
  KERFUFFLE_INLINE void widePreReenter(Context& context);

  KERFUFFLE_INLINE void widePreUpdate(Context& context);

  template <typename TEvent>
  KERFUFFLE_INLINE void widePreReact(const TEvent& event, Context& context);

  KERFUFFLE_INLINE void widePreExitGuard(Context& context);

  KERFUFFLE_INLINE void widePostExit(Context& context);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst>
struct B_<TFirst> : TFirst {
  using typename TFirst::Context;

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  using typename TFirst::Rank;
  using typename TFirst::Utility;
#endif

  using typename TFirst::RegionList;
  using typename TFirst::StateList;

  using typename TFirst::Control;
  using typename TFirst::PlanControl;

#ifdef KERFUFFLE_ENABLE_PLANS
  using typename TFirst::Plan;
#endif

  using typename TFirst::FullControl;
  using typename TFirst::GuardControl;

  using TFirst::regionId;
  using TFirst::stateId;

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  KERFUFFLE_INLINE Rank    rank(const Control&) { return Rank{0}; }
  KERFUFFLE_INLINE Utility utility(const Control&) { return Utility{1.0f}; }
#endif

  KERFUFFLE_INLINE void entryGuard(GuardControl&) {}

  KERFUFFLE_INLINE void enter(PlanControl&) {}
  KERFUFFLE_INLINE void reenter(PlanControl&) {}

  KERFUFFLE_INLINE void update(FullControl&) {}

  template <typename TEvent>
  KERFUFFLE_INLINE void react(const TEvent&, FullControl&) {}

  KERFUFFLE_INLINE void exitGuard(GuardControl&) {}

  KERFUFFLE_INLINE void exit(PlanControl&) {}

#ifdef KERFUFFLE_ENABLE_PLANS
  KERFUFFLE_INLINE void planSucceeded(FullControl& control) {
    control.succeed();
  }
  KERFUFFLE_INLINE void planFailed(FullControl& control) { control.fail(); }
#endif

  KERFUFFLE_INLINE void widePreEntryGuard(Context& context);

  KERFUFFLE_INLINE void widePreEnter(Context& context);
  KERFUFFLE_INLINE void widePreReenter(Context& context);

  KERFUFFLE_INLINE void widePreUpdate(Context& context);

  template <typename TEvent>
  KERFUFFLE_INLINE void widePreReact(const TEvent& event, Context& context);

  KERFUFFLE_INLINE void widePreExitGuard(Context& context);

  KERFUFFLE_INLINE void widePostExit(Context& context);
};

//------------------------------------------------------------------------------

template <typename TArgs>
using EmptyT = B_<InjectionT<TArgs>>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Dynamic_ {};

template <typename... TI>
struct DB_ : Dynamic_, B_<TI...> {};

template <typename TArgs>
using DynamicEmptyT = DB_<InjectionT<TArgs>>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Static_ {};

template <typename... TI>
struct SB_ : Static_, B_<TI...> {};

template <typename TArgs>
using StaticEmptyT = SB_<InjectionT<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}  // namespace detail
}  // namespace kerfuffle

#include "injections.inl"
