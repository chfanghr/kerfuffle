namespace kerfuffle {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, ShortIndex, typename...>
struct OS_;

//------------------------------------------------------------------------------

template <typename TIndices,
          typename TArgs,
          ShortIndex NIndex,
          typename TInitial,
          typename... TRemaining>
struct OS_<TIndices, TArgs, NIndex, TInitial, TRemaining...> final {
  using Indices                           = TIndices;
  static constexpr StateID    INITIAL_ID  = Indices::STATE_ID;
  static constexpr ShortIndex COMPO_INDEX = Indices::COMPO_INDEX;
  static constexpr ShortIndex ORTHO_INDEX = Indices::ORTHO_INDEX;
  static constexpr ShortIndex ORTHO_UNIT  = Indices::ORTHO_UNIT;

  static constexpr ShortIndex PRONG_INDEX = NIndex;

  using Args = TArgs;

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  using Rank    = typename Args::Rank;
  using Utility = typename Args::Utility;
  using UP      = typename Args::UP;
#endif

  using Registry       = RegistryT<Args>;
  using StateParents   = typename Registry::StateParents;
  using OrthoForks     = typename Registry::OrthoForks;
  using ProngBits      = typename OrthoForks::Bits;
  using ProngConstBits = typename OrthoForks::ConstBits;

  using Control      = ControlT<Args>;
  using PlanControl  = PlanControlT<Args>;
  using FullControl  = FullControlT<Args>;
  using GuardControl = GuardControlT<Args>;

  using Initial = Material<I_<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, ORTHO_UNIT>,
                           Args,
                           TInitial>;

  using InitialInfo   = WrapInfo<TInitial>;
  using InitialStates = typename InitialInfo::StateList;

  using Remaining = OS_<I_<INITIAL_ID + InitialInfo::STATE_COUNT,
                           COMPO_INDEX + InitialInfo::COMPO_REGIONS,
                           ORTHO_INDEX + InitialInfo::ORTHO_REGIONS,
                           ORTHO_UNIT + InitialInfo::ORTHO_UNITS>,
                        Args,
                        PRONG_INDEX + 1,
                        TRemaining...>;

  using Info = OSI_<TInitial, TRemaining...>;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_EXPLICIT_MEMBER_SPECIALIZATION
  template <typename T>
  KERFUFFLE_INLINE T& access();

  template <typename T>
  KERFUFFLE_INLINE const T& access() const;
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE void wideRegister(Registry& registry, const ForkID forkId);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE bool wideForwardEntryGuard(GuardControl&        control,
                                              const ProngConstBits prongs);
  KERFUFFLE_INLINE bool wideForwardEntryGuard(GuardControl& control);
  KERFUFFLE_INLINE bool wideEntryGuard(GuardControl& control);

  KERFUFFLE_INLINE void wideConstruct(PlanControl& control);

  KERFUFFLE_INLINE void wideEnter(PlanControl& control);
  KERFUFFLE_INLINE void wideReenter(PlanControl& control);

  KERFUFFLE_INLINE Status wideUpdate(FullControl& control);

  template <typename TEvent>
  KERFUFFLE_INLINE Status wideReact(FullControl& control, const TEvent& event);

  KERFUFFLE_INLINE bool wideForwardExitGuard(GuardControl&        control,
                                             const ProngConstBits prongs);
  KERFUFFLE_INLINE bool wideForwardExitGuard(GuardControl& control);
  KERFUFFLE_INLINE bool wideExitGuard(GuardControl& control);

  KERFUFFLE_INLINE void wideExit(PlanControl& control);

  KERFUFFLE_INLINE void wideDestruct(PlanControl& control);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE void wideForwardActive(Control&             control,
                                          const Request::Type  request,
                                          const ProngConstBits prongs);
  KERFUFFLE_INLINE void wideForwardRequest(Control&             control,
                                           const Request::Type  request,
                                           const ProngConstBits prongs);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE void wideRequestChange(Control& control);
  KERFUFFLE_INLINE void wideRequestRemain(Registry& registry);
  KERFUFFLE_INLINE void wideRequestRestart(Registry& registry);
  KERFUFFLE_INLINE void wideRequestResume(Registry& registry);

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  KERFUFFLE_INLINE void wideRequestUtilize(Control& control);
  KERFUFFLE_INLINE void wideRequestRandomize(Control& control);

  KERFUFFLE_INLINE Utility wideReportChange(Control& control);
  KERFUFFLE_INLINE Utility wideReportUtilize(Control& control);
  KERFUFFLE_INLINE Utility wideReportRandomize(Control& control);
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE void wideChangeToRequested(PlanControl& control);

  //----------------------------------------------------------------------

#ifdef KERFUFFLE_ENABLE_SERIALIZATION
  using WriteStream = typename Args::WriteStream;
  using ReadStream  = typename Args::ReadStream;

  KERFUFFLE_INLINE void wideSaveActive(const Registry& registry,
                                       WriteStream&    stream) const;
  KERFUFFLE_INLINE void wideSaveResumable(const Registry& registry,
                                          WriteStream&    stream) const;

  KERFUFFLE_INLINE void wideLoadRequested(Registry&   registry,
                                          ReadStream& stream) const;
  KERFUFFLE_INLINE void wideLoadResumable(Registry&   registry,
                                          ReadStream& stream) const;
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_STRUCTURE_REPORT
  using StructureStateInfos = typename Args::StructureStateInfos;

  static constexpr LongIndex NAME_COUNT =
      Initial::NAME_COUNT + Remaining::NAME_COUNT;

  void wideGetNames(const LongIndex      parent,
                    const ShortIndex     depth,
                    StructureStateInfos& stateInfos) const;
#endif

  //----------------------------------------------------------------------

  Initial   initial;
  Remaining remaining;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
          typename TArgs,
          ShortIndex NIndex,
          typename TInitial>
struct OS_<TIndices, TArgs, NIndex, TInitial> final {
  using Indices                           = TIndices;
  static constexpr StateID    INITIAL_ID  = Indices::STATE_ID;
  static constexpr ShortIndex COMPO_INDEX = Indices::COMPO_INDEX;
  static constexpr ShortIndex ORTHO_INDEX = Indices::ORTHO_INDEX;
  static constexpr ShortIndex ORTHO_UNIT  = Indices::ORTHO_UNIT;

  static constexpr ShortIndex PRONG_INDEX = NIndex;

  using Args = TArgs;

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  using Rank    = typename Args::Rank;
  using Utility = typename Args::Utility;
  using UP      = typename Args::UP;
#endif

  using Registry       = RegistryT<Args>;
  using StateParents   = typename Registry::StateParents;
  using OrthoForks     = typename Registry::OrthoForks;
  using ProngBits      = typename OrthoForks::Bits;
  using ProngConstBits = typename OrthoForks::ConstBits;

  using Control      = ControlT<Args>;
  using PlanControl  = PlanControlT<Args>;
  using FullControl  = FullControlT<Args>;
  using GuardControl = GuardControlT<Args>;

  using Initial = Material<I_<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, ORTHO_UNIT>,
                           Args,
                           TInitial>;

  using Info = OSI_<TInitial>;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_EXPLICIT_MEMBER_SPECIALIZATION
  template <typename T>
  KERFUFFLE_INLINE T& access() {
    return initial.template access<T>();
  }

  template <typename T>
  KERFUFFLE_INLINE const T& access() const {
    return initial.template access<T>();
  }
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE void wideRegister(Registry& registry, const ForkID forkId);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE bool wideForwardEntryGuard(GuardControl&        control,
                                              const ProngConstBits prongs);
  KERFUFFLE_INLINE bool wideForwardEntryGuard(GuardControl& control);
  KERFUFFLE_INLINE bool wideEntryGuard(GuardControl& control);

  KERFUFFLE_INLINE void wideConstruct(PlanControl& control);

  KERFUFFLE_INLINE void wideEnter(PlanControl& control);
  KERFUFFLE_INLINE void wideReenter(PlanControl& control);

  KERFUFFLE_INLINE Status wideUpdate(FullControl& control);

  template <typename TEvent>
  KERFUFFLE_INLINE Status wideReact(FullControl& control, const TEvent& event);

  KERFUFFLE_INLINE bool wideForwardExitGuard(GuardControl&        control,
                                             const ProngConstBits prongs);
  KERFUFFLE_INLINE bool wideForwardExitGuard(GuardControl& control);
  KERFUFFLE_INLINE bool wideExitGuard(GuardControl& control);

  KERFUFFLE_INLINE void wideExit(PlanControl& control);

  KERFUFFLE_INLINE void wideDestruct(PlanControl& control);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE void wideForwardActive(Control&             control,
                                          const Request::Type  request,
                                          const ProngConstBits prongs);
  KERFUFFLE_INLINE void wideForwardRequest(Control&             control,
                                           const Request::Type  request,
                                           const ProngConstBits prongs);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE void wideRequestChange(Control& control);
  KERFUFFLE_INLINE void wideRequestRemain(Registry& registry);
  KERFUFFLE_INLINE void wideRequestRestart(Registry& registry);
  KERFUFFLE_INLINE void wideRequestResume(Registry& registry);

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  KERFUFFLE_INLINE void wideRequestUtilize(Control& control);
  KERFUFFLE_INLINE void wideRequestRandomize(Control& control);

  KERFUFFLE_INLINE Utility wideReportChange(Control& control);
  KERFUFFLE_INLINE Utility wideReportUtilize(Control& control);
  KERFUFFLE_INLINE Utility wideReportRandomize(Control& control);
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE void wideChangeToRequested(PlanControl& control);

  //----------------------------------------------------------------------

#ifdef KERFUFFLE_ENABLE_SERIALIZATION
  using WriteStream = typename Args::WriteStream;
  using ReadStream  = typename Args::ReadStream;

  KERFUFFLE_INLINE void wideSaveActive(const Registry& registry,
                                       WriteStream&    stream) const;
  KERFUFFLE_INLINE void wideSaveResumable(const Registry& registry,
                                          WriteStream&    stream) const;

  KERFUFFLE_INLINE void wideLoadRequested(Registry&   registry,
                                          ReadStream& stream) const;
  KERFUFFLE_INLINE void wideLoadResumable(Registry&   registry,
                                          ReadStream& stream) const;
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_STRUCTURE_REPORT
  using StructureStateInfos = typename Args::StructureStateInfos;

  static constexpr LongIndex NAME_COUNT = Initial::NAME_COUNT;

  void wideGetNames(const LongIndex      parent,
                    const ShortIndex     depth,
                    StructureStateInfos& stateInfos) const;
#endif

  //----------------------------------------------------------------------

  Initial initial;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace detail
}  // namespace kerfuffle

#include "orthogonal_sub_1.inl"
#include "orthogonal_sub_2.inl"
