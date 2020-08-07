namespace kerfuffle {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
          typename TArgs,
          Strategy TStrategy,
          typename THead,
          typename... TSubStates>
struct C_ final {
  using Indices                           = TIndices;
  static constexpr StateID    HEAD_ID     = Indices::STATE_ID;
  static constexpr ShortIndex COMPO_INDEX = Indices::COMPO_INDEX;
  static constexpr ShortIndex ORTHO_INDEX = Indices::ORTHO_INDEX;
  static constexpr ShortIndex ORTHO_UNIT  = Indices::ORTHO_UNIT;

  static constexpr Strategy STRATEGY = TStrategy;

  static constexpr ShortIndex REGION_ID = COMPO_INDEX + ORTHO_INDEX;
  static constexpr ForkID     COMPO_ID  = COMPO_INDEX + 1;

  using Args = TArgs;

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  using Rank    = typename Args::Rank;
  using Utility = typename Args::Utility;
  using UP      = typename Args::UP;
#endif

  using StateList  = typename Args::StateList;
  using RegionList = typename Args::RegionList;

  using Registry     = RegistryT<Args>;
  using StateParents = typename Registry::StateParents;

  using Control = ControlT<Args>;

  using PlanControl  = PlanControlT<Args>;
  using ScopedOrigin = typename PlanControl::Origin;
  using ScopedRegion = typename PlanControl::Region;

#ifdef KERFUFFLE_ENABLE_PLANS
  using Plan = PlanT<Args>;
#endif

  using FullControl = FullControlT<Args>;
  using ControlLock = typename FullControl::Lock;

  using GuardControl = GuardControlT<Args>;

  using Head = THead;

  using HeadState = S_<Indices, Args, Head>;

  using SubStates =
      CS_<I_<HEAD_ID + 1, COMPO_INDEX + 1, ORTHO_INDEX, ORTHO_UNIT>,
          Args,
          STRATEGY,
          0,
          TSubStates...>;

  using Info                              = CI_<STRATEGY, Head, TSubStates...>;
  static constexpr ShortIndex WIDTH       = Info::WIDTH;
  static constexpr ShortIndex WIDTH_BITS  = Info::WIDTH_BITS;
  static constexpr ShortIndex STATE_COUNT = Info::STATE_COUNT;

  //----------------------------------------------------------------------

#ifdef KERFUFFLE_EXPLICIT_MEMBER_SPECIALIZATION
  template <typename T>
  struct Accessor {
    KERFUFFLE_INLINE static T& get(C_& c) {
      return c._subStates.template access<T>();
    }
    KERFUFFLE_INLINE static const T& get(const C_& c) {
      return c._subStates.template access<T>();
    }
  };

  template <>
  struct Accessor<Head> {
    KERFUFFLE_INLINE static Head& get(C_& c) {
      return c._headState._headBox.get();
    }
    KERFUFFLE_INLINE static const Head& get(const C_& c) {
      return c._headState._headBox.get();
    }
  };

  template <typename T>
  KERFUFFLE_INLINE T& access() {
    return Accessor<T>::get(*this);
  }

  template <typename T>
  KERFUFFLE_INLINE const T& access() const {
    return Accessor<T>::get(*this);
  }
#endif

  //----------------------------------------------------------------------

  KERFUFFLE_INLINE ShortIndex& compoRequested(Registry& registry) const {
    return registry.compoRequested[COMPO_INDEX];
  }
  KERFUFFLE_INLINE const ShortIndex& compoRequested(
      const Registry& registry) const {
    return registry.compoRequested[COMPO_INDEX];
  }

  KERFUFFLE_INLINE ShortIndex& compoActive(Registry& registry) const {
    return registry.compoActive[COMPO_INDEX];
  }
  KERFUFFLE_INLINE const ShortIndex& compoActive(
      const Registry& registry) const {
    return registry.compoActive[COMPO_INDEX];
  }

  KERFUFFLE_INLINE ShortIndex& compoResumable(Registry& registry) const {
    return registry.compoResumable[COMPO_INDEX];
  }
  KERFUFFLE_INLINE const ShortIndex& compoResumable(
      const Registry& registry) const {
    return registry.compoResumable[COMPO_INDEX];
  }

  KERFUFFLE_INLINE ShortIndex& compoRequested(Control& control) const {
    return compoRequested(control._registry);
  }
  KERFUFFLE_INLINE const ShortIndex& compoRequested(
      const Control& control) const {
    return compoRequested(control._registry);
  }

  KERFUFFLE_INLINE ShortIndex& compoActive(Control& control) const {
    return compoActive(control._registry);
  }
  KERFUFFLE_INLINE const ShortIndex& compoActive(const Control& control) const {
    return compoActive(control._registry);
  }

  KERFUFFLE_INLINE ShortIndex& compoResumable(Control& control) const {
    return compoResumable(control._registry);
  }
  KERFUFFLE_INLINE const ShortIndex& compoResumable(
      const Control& control) const {
    return compoResumable(control._registry);
  }

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  KERFUFFLE_INLINE ShortIndex
  resolveRandom(Control& control,
                const Utility (&options)[Info::WIDTH],
                const Utility sum,
                const Rank (&ranks)[Info::WIDTH],
                const Rank top) const;
#endif

  KERFUFFLE_INLINE bool compoRemain(Control& control) {
    return control._registry.compoRemains.template get<COMPO_INDEX>();
  }

  KERFUFFLE_INLINE void deepRegister(Registry& registry, const Parent parent);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE bool deepForwardEntryGuard(GuardControl& control);
  KERFUFFLE_INLINE bool deepEntryGuard(GuardControl& control);

  KERFUFFLE_INLINE void deepConstruct(PlanControl& control);

  KERFUFFLE_INLINE void deepEnter(PlanControl& control);
  KERFUFFLE_INLINE void deepReenter(PlanControl& control);

  KERFUFFLE_INLINE Status deepUpdate(FullControl& control);

  template <typename TEvent>
  KERFUFFLE_INLINE Status deepReact(FullControl& control, const TEvent& event);

  KERFUFFLE_INLINE bool deepForwardExitGuard(GuardControl& control);
  KERFUFFLE_INLINE bool deepExitGuard(GuardControl& control);

  KERFUFFLE_INLINE void deepExit(PlanControl& control);

  KERFUFFLE_INLINE void deepDestruct(PlanControl& control);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE void deepForwardActive(Control&            control,
                                          const Request::Type request);
  KERFUFFLE_INLINE void deepForwardRequest(Control&            control,
                                           const Request::Type request);

  KERFUFFLE_INLINE void deepRequest(Control&            control,
                                    const Request::Type request);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_EXPLICIT_MEMBER_SPECIALIZATION

  template <Strategy = STRATEGY>
  KERFUFFLE_INLINE void deepRequestChange(
      Control& control, const ShortIndex = INVALID_SHORT_INDEX);

  template <>
  KERFUFFLE_INLINE void deepRequestChange<Composite>(Control& control,
                                                     const ShortIndex) {
    deepRequestChangeComposite(control);
  }

  template <>
  KERFUFFLE_INLINE void deepRequestChange<Resumable>(Control& control,
                                                     const ShortIndex) {
    deepRequestChangeResumable(control);
  }

#  ifdef KERFUFFLE_ENABLE_UTILITY_THEORY

  template <>
  KERFUFFLE_INLINE void deepRequestChange<Utilitarian>(Control& control,
                                                       const ShortIndex) {
    deepRequestChangeUtilitarian(control);
  }

  template <>
  KERFUFFLE_INLINE void deepRequestChange<RandomUtil>(Control& control,
                                                      const ShortIndex) {
    deepRequestChangeRandom(control);
  }

#  endif

#else

  KERFUFFLE_INLINE void deepRequestChange(Control& control);

#endif

  KERFUFFLE_INLINE void deepRequestChangeComposite(Control& control);
  KERFUFFLE_INLINE void deepRequestChangeResumable(Control& control);

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  KERFUFFLE_INLINE void deepRequestChangeUtilitarian(Control& control);
  KERFUFFLE_INLINE void deepRequestChangeRandom(Control& control);
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE void deepRequestRemain(Registry& registry);
  KERFUFFLE_INLINE void deepRequestRestart(Registry& registry);
  KERFUFFLE_INLINE void deepRequestResume(Registry& registry);

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  KERFUFFLE_INLINE void deepRequestUtilize(Control& control);
  KERFUFFLE_INLINE void deepRequestRandomize(Control& control);
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
#  ifdef KERFUFFLE_EXPLICIT_MEMBER_SPECIALIZATION

  template <Strategy = STRATEGY>
  KERFUFFLE_INLINE UP deepReportChange(Control& control);

  template <>
  KERFUFFLE_INLINE UP deepReportChange<Composite>(Control& control) {
    return deepReportChangeComposite(control);
  }

  template <>
  KERFUFFLE_INLINE UP deepReportChange<Resumable>(Control& control) {
    return deepReportChangeResumable(control);
  }

  template <>
  KERFUFFLE_INLINE UP deepReportChange<Utilitarian>(Control& control) {
    return deepReportChangeUtilitarian(control);
  }

  template <>
  KERFUFFLE_INLINE UP deepReportChange<RandomUtil>(Control& control) {
    return deepReportChangeRandom(control);
  }

#  else

  KERFUFFLE_INLINE UP deepReportChange(Control& control);

#  endif

  KERFUFFLE_INLINE UP deepReportChangeComposite(Control& control);
  KERFUFFLE_INLINE UP deepReportChangeResumable(Control& control);
  KERFUFFLE_INLINE UP deepReportChangeUtilitarian(Control& control);
  KERFUFFLE_INLINE UP deepReportChangeRandom(Control& control);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE UP      deepReportUtilize(Control& control);
  KERFUFFLE_INLINE Rank    deepReportRank(Control& control);
  KERFUFFLE_INLINE Utility deepReportRandomize(Control& control);

#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE void deepChangeToRequested(PlanControl& control);

  //----------------------------------------------------------------------

#ifdef KERFUFFLE_ENABLE_SERIALIZATION
  using WriteStream = typename Args::WriteStream;
  using ReadStream  = typename Args::ReadStream;

  KERFUFFLE_INLINE void deepSaveActive(const Registry& registry,
                                       WriteStream&    stream) const;
  KERFUFFLE_INLINE void deepSaveResumable(const Registry& registry,
                                          WriteStream&    stream) const;

  KERFUFFLE_INLINE void deepLoadRequested(Registry&   registry,
                                          ReadStream& stream) const;
  KERFUFFLE_INLINE void deepLoadResumable(Registry&   registry,
                                          ReadStream& stream) const;
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_STRUCTURE_REPORT
  using StructureStateInfos = typename Args::StructureStateInfos;
  using RegionType          = typename StructureStateInfo::RegionType;

  static constexpr LongIndex NAME_COUNT =
      HeadState::NAME_COUNT + SubStates::NAME_COUNT;

  void deepGetNames(const LongIndex      parent,
                    const RegionType     region,
                    const ShortIndex     depth,
                    StructureStateInfos& stateInfos) const;
#endif

  //----------------------------------------------------------------------

  HeadState _headState;
  SubStates _subStates;

  KERFUFFLE_IF_DEBUG(static constexpr Info _info = Info{});
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace detail
}  // namespace kerfuffle

#include "composite.inl"
