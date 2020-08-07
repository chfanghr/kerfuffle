namespace kerfuffle {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TA>
void DBox<T, TA>::construct() {
  KERFUFFLE_ASSERT(!initialized_);

  new (&t_) T{};

  KERFUFFLE_IF_ASSERT(initialized_ = true);
}

//------------------------------------------------------------------------------

template <typename T, typename TA>
void DBox<T, TA>::destruct() {
  KERFUFFLE_ASSERT(initialized_);

  t_.~T();

  KERFUFFLE_IF_ASSERT(initialized_ = false);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TA>
void SBox<T, TA>::guard(GuardControlT<TA>& control) {
  t_.widePreEntryGuard(control.context());
  t_.entryGuard(control);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace detail
}  // namespace kerfuffle
