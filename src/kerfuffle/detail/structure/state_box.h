namespace kerfuffle {

//------------------------------------------------------------------------------

template <typename>
struct Guard {
  template <typename TArgs>
  static void execute(kerfuffle::detail::GuardControlT<TArgs>&) {}
};

//------------------------------------------------------------------------------

namespace detail {

//------------------------------------------------------------------------------

KERFUFFLE_IF_DEBUG(struct None{});

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct DBox final {
  static constexpr bool isBare() { return false; }

  union {
    T t_;
  };

  KERFUFFLE_INLINE DBox() {}
  KERFUFFLE_INLINE ~DBox() {}

  KERFUFFLE_INLINE void guard(GuardControlT<TArgs>& control) {
    Guard<T>::execute(control);
  }

  KERFUFFLE_INLINE void construct();
  KERFUFFLE_INLINE void destruct();

  KERFUFFLE_INLINE T& get() {
    KERFUFFLE_ASSERT(initialized_);
    return t_;
  }
  KERFUFFLE_INLINE const T& get() const {
    KERFUFFLE_ASSERT(initialized_);
    return t_;
  }

  KERFUFFLE_IF_ASSERT(bool initialized_ = false);

  KERFUFFLE_IF_DEBUG(const std::type_index TYPE = typeid(T));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct SBox final {
  static constexpr bool isBare() {
    return std::is_base_of<T, StaticEmptyT<TArgs>>::value;
  }

  T t_;

  KERFUFFLE_INLINE void guard(GuardControlT<TArgs>& control);

  KERFUFFLE_INLINE void construct() {}
  KERFUFFLE_INLINE void destruct() {}

  KERFUFFLE_INLINE T&    get() { return t_; }
  KERFUFFLE_INLINE const T& get() const { return t_; }

  KERFUFFLE_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None)
                                                           : typeid(T));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct BoxifyT final {
  using Type = typename std::conditional<std::is_base_of<Dynamic_, T>::value,
                                         DBox<T, TArgs>,
                                         SBox<T, TArgs>>::type;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, typename TArgs>
using Boxify = typename BoxifyT<T, TArgs>::Type;

//------------------------------------------------------------------------------

}  // namespace detail
}  // namespace kerfuffle

#include "state_box.inl"
