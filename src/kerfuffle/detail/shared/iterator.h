namespace kerfuffle {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
class Iterator {
 public:
  using Container = TContainer;
  using Item      = typename Container::Item;

  template <typename T, LongIndex NCapacity>
  friend class Array;

 private:
  KERFUFFLE_INLINE Iterator(Container& container, const LongIndex cursor)
      : _container{container}, _cursor{cursor} {}

 public:
  KERFUFFLE_INLINE bool operator!=(const Iterator<Container>& dummy) const;

  KERFUFFLE_INLINE Iterator& operator++();

  KERFUFFLE_INLINE Item& operator*() { return _container[_cursor]; }
  KERFUFFLE_INLINE const Item& operator*() const { return _container[_cursor]; }

  KERFUFFLE_INLINE Item* operator->() { return &_container[_cursor]; }
  KERFUFFLE_INLINE const Item* operator->() const {
    return &_container[_cursor];
  }

 private:
  Container& _container;

  LongIndex _cursor;
};

//------------------------------------------------------------------------------

template <typename TContainer>
class Iterator<const TContainer> {
 public:
  using Container = TContainer;
  using Item      = typename Container::Item;

  template <typename T, LongIndex NCapacity>
  friend class Array;

 private:
  KERFUFFLE_INLINE Iterator(const Container& container, const LongIndex cursor)
      : _container{container}, _cursor{cursor} {}

 public:
  KERFUFFLE_INLINE bool operator!=(
      const Iterator<const Container>& dummy) const;

  KERFUFFLE_INLINE Iterator& operator++();

  KERFUFFLE_INLINE const Item& operator*() const { return _container[_cursor]; }

  KERFUFFLE_INLINE const Item* operator->() const { return &operator*(); }

 private:
  const Container& _container;

  LongIndex _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace detail
}  // namespace kerfuffle

#include "iterator.inl"
