namespace kerfuffle {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NCapacity>
class StaticArray {
 public:
  static constexpr LongIndex CAPACITY = NCapacity;
  static constexpr LongIndex DUMMY    = INVALID_LONG_INDEX;

  using Item  = T;
  using Index = UnsignedCapacity<CAPACITY>;

 public:
  KERFUFFLE_INLINE StaticArray() = default;
  KERFUFFLE_INLINE StaticArray(const Item filler);

  template <typename N>
  KERFUFFLE_INLINE Item& operator[](const N i);

  template <typename N>
  KERFUFFLE_INLINE const Item& operator[](const N i) const;

  KERFUFFLE_INLINE LongIndex count() const { return CAPACITY; }

  KERFUFFLE_INLINE void fill(const Item filler);
  KERFUFFLE_INLINE void clear() { fill(INVALID_SHORT_INDEX); }

  KERFUFFLE_INLINE Iterator<StaticArray> begin() {
    return Iterator<StaticArray>(*this, 0);
  }
  KERFUFFLE_INLINE Iterator<const StaticArray> begin() const {
    return Iterator<const StaticArray>(*this, 0);
  }
  KERFUFFLE_INLINE Iterator<const StaticArray> cbegin() const {
    return Iterator<const StaticArray>(*this, 0);
  }

  KERFUFFLE_INLINE Iterator<StaticArray> end() {
    return Iterator<StaticArray>(*this, DUMMY);
  }
  KERFUFFLE_INLINE Iterator<const StaticArray> end() const {
    return Iterator<const StaticArray>(*this, DUMMY);
  }
  KERFUFFLE_INLINE Iterator<const StaticArray> cend() const {
    return Iterator<const StaticArray>(*this, DUMMY);
  }

 private:
  Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArray<T, 0> {
  using Item = T;

  KERFUFFLE_INLINE StaticArray() = default;
  KERFUFFLE_INLINE StaticArray(const Item) {}
};

//------------------------------------------------------------------------------

template <typename T, LongIndex NCapacity>
class Array {
  template <typename>
  friend class Iterator;

 public:
  static constexpr LongIndex CAPACITY = NCapacity;
  static constexpr LongIndex DUMMY    = INVALID_LONG_INDEX;

  using Item  = T;
  using Index = UnsignedCapacity<CAPACITY>;

 public:
  KERFUFFLE_INLINE void clear() { _count = 0; }

  template <typename TValue>
  KERFUFFLE_INLINE LongIndex append(TValue&& value);

  template <typename N>
  KERFUFFLE_INLINE Item& operator[](const N i);

  template <typename N>
  KERFUFFLE_INLINE const Item& operator[](const N i) const;

  KERFUFFLE_INLINE LongIndex count() const { return _count; }

  KERFUFFLE_INLINE Iterator<Array> begin() { return Iterator<Array>(*this, 0); }
  KERFUFFLE_INLINE Iterator<const Array> begin() const {
    return Iterator<const Array>(*this, 0);
  }
  KERFUFFLE_INLINE Iterator<const Array> cbegin() const {
    return Iterator<const Array>(*this, 0);
  }

  KERFUFFLE_INLINE Iterator<Array> end() {
    return Iterator<Array>(*this, DUMMY);
  }
  KERFUFFLE_INLINE Iterator<const Array> end() const {
    return Iterator<const Array>(*this, DUMMY);
  }
  KERFUFFLE_INLINE Iterator<const Array> cend() const {
    return Iterator<const Array>(*this, DUMMY);
  }

 private:
  KERFUFFLE_INLINE LongIndex next(const LongIndex i) const { return i + 1; }
  KERFUFFLE_INLINE LongIndex limit() const { return _count; }

 private:
  LongIndex _count = 0;

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning( \
      disable : 4324)  // structure was padded due to alignment specifier
#endif

  Item _items[CAPACITY];  // warning 4324 triggers for 'StructureStateInfo'

#ifdef _MSC_VER
#  pragma warning(pop)
#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
class Array<T, 0> {};

////////////////////////////////////////////////////////////////////////////////

}  // namespace detail
}  // namespace kerfuffle

#include "array.inl"
