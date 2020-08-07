namespace kerfuffle {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool Iterator<T>::operator!=(
    const Iterator<T>& KERFUFFLE_IF_ASSERT(dummy)) const {
  KERFUFFLE_ASSERT(&_container == &dummy._container);

  return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename T>
Iterator<T>& Iterator<T>::operator++() {
  _cursor = _container.next(_cursor);

  return *this;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool Iterator<const T>::operator!=(
    const Iterator<const T>& KERFUFFLE_IF_ASSERT(dummy)) const {
  KERFUFFLE_ASSERT(&_container == &dummy._container);

  return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename T>
Iterator<const T>& Iterator<const T>::operator++() {
  _cursor = _container.next(_cursor);

  return *this;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace detail
}  // namespace kerfuffle
