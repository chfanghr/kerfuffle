namespace kerfuffle {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices, typename TArgs, typename THead>
struct S_ final {
  static constexpr auto STATE_ID = TIndices::STATE_ID;

  using Context = typename TArgs::Context;

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  using Rank    = typename TArgs::Rank;
  using Utility = typename TArgs::Utility;
  using UP      = typename TArgs::UP;
#endif

#ifdef KERFUFFLE_ENABLE_LOG_INTERFACE
  using Logger = typename TArgs::Logger;
#endif

  using Control      = ControlT<TArgs>;
  using Registry     = RegistryT<TArgs>;
  using StateParents = typename Registry::StateParents;

  using PlanControl  = PlanControlT<TArgs>;
  using ScopedOrigin = typename PlanControl::Origin;

  using FullControl  = FullControlT<TArgs>;
  using GuardControl = GuardControlT<TArgs>;

  using Head    = THead;
  using HeadBox = Boxify<Head, TArgs>;

  //----------------------------------------------------------------------

#ifdef KERFUFFLE_EXPLICIT_MEMBER_SPECIALIZATION
#  ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wnull-dereference"
#  endif

  template <typename T>
  struct Accessor {
    KERFUFFLE_INLINE static T& get(S_&) {
      KERFUFFLE_BREAK();
      return *reinterpret_cast<T*>(0);
    }
    KERFUFFLE_INLINE static const T& get(const S_&) {
      KERFUFFLE_BREAK();
      return *reinterpret_cast<T*>(0);
    }
  };

#  ifdef __clang__
#    pragma clang diagnostic pop
#  endif

  template <>
  struct Accessor<Head> {
    KERFUFFLE_INLINE static Head&       get(S_& s) { return s._headBox.get(); }
    KERFUFFLE_INLINE static const Head& get(const S_& s) {
      return s._headBox.get();
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

  KERFUFFLE_INLINE Parent stateParent(Control& control) {
    return control._registry.stateParents[STATE_ID];
  }

  KERFUFFLE_INLINE void deepRegister(Registry& registry, const Parent parent);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  KERFUFFLE_INLINE Rank    wrapRank(Control& control);
  KERFUFFLE_INLINE Utility wrapUtility(Control& control);
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE bool deepForwardEntryGuard(GuardControl&) { return false; }
  KERFUFFLE_INLINE bool deepEntryGuard(GuardControl& control);

  KERFUFFLE_INLINE void deepConstruct(PlanControl& control);

  KERFUFFLE_INLINE void deepEnter(PlanControl& control);
  KERFUFFLE_INLINE void deepReenter(PlanControl& control);

  KERFUFFLE_INLINE Status deepUpdate(FullControl& control);

  template <typename TEvent>
  KERFUFFLE_INLINE Status deepReact(FullControl& control, const TEvent& event);

  KERFUFFLE_INLINE bool deepForwardExitGuard(GuardControl&) { return false; }
  KERFUFFLE_INLINE bool deepExitGuard(GuardControl& control);

  KERFUFFLE_INLINE void deepExit(PlanControl& control);

  KERFUFFLE_INLINE void deepDestruct(PlanControl& control);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_PLANS
  KERFUFFLE_INLINE void wrapPlanSucceeded(FullControl& control);
  KERFUFFLE_INLINE void wrapPlanFailed(FullControl& control);
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE void deepForwardActive(Control&, const Request::Type) {}
  KERFUFFLE_INLINE void deepForwardRequest(Control&, const Request::Type) {}

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE void deepRequestChange(Control&) {}
  KERFUFFLE_INLINE void deepRequestRemain(Registry&) {}
  KERFUFFLE_INLINE void deepRequestRestart(Registry&) {}
  KERFUFFLE_INLINE void deepRequestResume(Registry&) {}

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  KERFUFFLE_INLINE void deepRequestUtilize(Control&) {}
  KERFUFFLE_INLINE void deepRequestRandomize(Control&) {}

  KERFUFFLE_INLINE UP      deepReportChange(Control& control);
  KERFUFFLE_INLINE UP      deepReportUtilize(Control& control);
  KERFUFFLE_INLINE Rank    deepReportRank(Control& control);
  KERFUFFLE_INLINE Utility deepReportRandomize(Control& control);
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  KERFUFFLE_INLINE void deepChangeToRequested(Control&) {}

  //----------------------------------------------------------------------

#ifdef KERFUFFLE_ENABLE_SERIALIZATION
  using WriteStream = typename TArgs::WriteStream;
  using ReadStream  = typename TArgs::ReadStream;

  KERFUFFLE_INLINE void deepSaveActive(const Registry&, WriteStream&) const {}
  KERFUFFLE_INLINE void deepSaveResumable(const Registry&, WriteStream&) const {
  }

  KERFUFFLE_INLINE void deepLoadRequested(Registry&, ReadStream&) const {}
  KERFUFFLE_INLINE void deepLoadResumable(Registry&, ReadStream&) const {}
#endif

  //------------------------------------------------------------------------------

#ifdef KERFUFFLE_ENABLE_STRUCTURE_REPORT
  using StructureStateInfos = typename TArgs::StructureStateInfos;
  using RegionType          = typename StructureStateInfo::RegionType;

  static const char* name();

  void deepGetNames(const LongIndex      parent,
                    const RegionType     region,
                    const ShortIndex     depth,
                    StructureStateInfos& stateInfos) const;
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _DEBUG || defined KERFUFFLE_ENABLE_STRUCTURE_REPORT || \
    defined                   KERFUFFLE_ENABLE_LOG_INTERFACE

  static constexpr LongIndex NAME_COUNT = HeadBox::isBare() ? 0 : 1;

#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_LOG_INTERFACE

  template <typename>
  struct Traits_;

  template <typename TR_, typename TH_, typename... TAs_>
  struct Traits_<TR_ (TH_::*)(TAs_...)> {
    using Host = TH_;
  };

  template <typename TM_>
  using Host_ = typename Traits_<TM_>::Host;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  using Empty = EmptyT<TArgs>;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  template <typename TMethodType>
  typename std::enable_if<std::is_same<Host_<TMethodType>, Empty>::value>::type
  log(Logger&, Context&, const Method) const {}

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  template <typename TMethodType>
  typename std::enable_if<!std::is_same<Host_<TMethodType>, Empty>::value>::type
  log(Logger& logger, Context& context, const Method method) const {
    logger.recordMethod(context, STATE_ID, method);
  }

#endif

  //----------------------------------------------------------------------

  // TODO: account for boxing
  //
  // if you see..
  // Clang - error : field has incomplete type
  // 'kerfuffle::detail::S_<...>::Head' (aka 'Blah')
  //
  // .. add definition for the state 'Blah'
  HeadBox _headBox;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace detail
}  // namespace kerfuffle

#include "state.inl"
