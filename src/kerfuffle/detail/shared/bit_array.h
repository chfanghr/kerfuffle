namespace kerfuffle {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct Units {
  ShortIndex unit;
  ShortIndex width;
};

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
class BitArray final {
 public:
  using Index = TIndex;
  using Unit  = unsigned char;

  static constexpr Index CAPACITY = NCapacity;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  class Bits {
    template <typename, ShortIndex>
    friend class BitArray;

   private:
    KERFUFFLE_INLINE explicit Bits(Unit* const storage, const Index width)
        : _storage{storage}, _width{width} {}

   public:
    KERFUFFLE_INLINE explicit operator bool() const;

    KERFUFFLE_INLINE void clear();

    template <ShortIndex NIndex>
    KERFUFFLE_INLINE bool get() const;

    template <ShortIndex NIndex>
    KERFUFFLE_INLINE void set();

    template <ShortIndex NIndex>
    KERFUFFLE_INLINE void reset();

    KERFUFFLE_INLINE bool get(const Index index) const;
    KERFUFFLE_INLINE void set(const Index index);
    KERFUFFLE_INLINE void reset(const Index index);

   private:
    Unit* const _storage;
    const Index _width;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  class ConstBits {
    template <typename, ShortIndex>
    friend class BitArray;

   private:
    KERFUFFLE_INLINE explicit ConstBits(const Unit* const storage,
                                        const Index       width)
        : _storage{storage}, _width{width} {}

   public:
    KERFUFFLE_INLINE explicit operator bool() const;

    template <ShortIndex NIndex>
    KERFUFFLE_INLINE bool get() const;

    KERFUFFLE_INLINE bool get(const Index index) const;

   private:
    const Unit* const _storage;
    const Index       _width;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 public:
  BitArray() { clear(); }

  KERFUFFLE_INLINE void clear();

  template <ShortIndex NIndex>
  KERFUFFLE_INLINE bool get() const;

  template <ShortIndex NIndex>
  KERFUFFLE_INLINE void set();

  template <ShortIndex NIndex>
  KERFUFFLE_INLINE void reset();

  KERFUFFLE_INLINE bool get(const Index index) const;
  KERFUFFLE_INLINE void set(const Index index);
  // KERFUFFLE_INLINE void reset(const Index index);

  template <ShortIndex NUnit, ShortIndex NWidth>
  KERFUFFLE_INLINE Bits bits();

  template <ShortIndex NUnit, ShortIndex NWidth>
  KERFUFFLE_INLINE ConstBits bits() const;

  KERFUFFLE_INLINE Bits      bits(const Units& units);
  KERFUFFLE_INLINE ConstBits bits(const Units& units) const;

 private:
  Unit _storage[CAPACITY];
};

//------------------------------------------------------------------------------

template <typename TIndex>
class BitArray<TIndex, 0> final {
 public:
  KERFUFFLE_INLINE void clear() {}
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace detail
}  // namespace kerfuffle

#include "bit_array.inl"
