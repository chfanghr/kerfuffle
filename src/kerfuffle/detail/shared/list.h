namespace kerfuffle {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TItem, LongIndex NCapacity>
class List {
 public:
  using Index = LongIndex;

  static constexpr Index CAPACITY = NCapacity;

  static constexpr Index INVALID = Index(-1);

 private:
  using Item = TItem;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct Links {
    Index prev;
    Index next;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  union Cell {
    Item  item;
    Links links;

    KERFUFFLE_INLINE Cell() : links{} {}
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 public:
  template <typename... TArgs>
  Index emplace(TArgs... args);

  void remove(const Index i);

  KERFUFFLE_INLINE Item& operator[](const Index i);
  KERFUFFLE_INLINE const Item& operator[](const Index i) const;

  KERFUFFLE_INLINE Index count() const { return _count; }

 private:
  KERFUFFLE_IF_ASSERT(void verifyStructure(const Index occupied = INVALID)
                          const);

 private:
  Cell  _cells[CAPACITY];
  Index _vacantHead = 0;
  Index _vacantTail = 0;
  Index _last       = 0;
  Index _count      = 0;
};

//------------------------------------------------------------------------------

template <typename TItem>
class List<TItem, 0> {};

////////////////////////////////////////////////////////////////////////////////

}  // namespace detail
}  // namespace kerfuffle

#include "list.inl"
