namespace kerfuffle {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NC>
template <typename... TA>
LongIndex List<T, NC>::emplace(TA... args) {
  if (_count < CAPACITY) {
    KERFUFFLE_ASSERT(_vacantHead < CAPACITY);
    KERFUFFLE_ASSERT(_vacantTail < CAPACITY);

    const Index result = _vacantHead;
    auto&       cell   = _cells[result];
    ++_count;

    if (_vacantHead != _vacantTail) {
      // recycle
      KERFUFFLE_ASSERT(cell.links.prev == INVALID);
      KERFUFFLE_ASSERT(cell.links.next != INVALID);

      _vacantHead = cell.links.next;

      auto& head = _cells[_vacantHead];
      KERFUFFLE_ASSERT(head.links.prev == result);
      head.links.prev = INVALID;
    } else if (_last < CAPACITY) {
      // grow
      ++_last;
      _vacantHead = _last;
      _vacantTail = _last;

      auto& nextVacant      = _cells[_vacantHead];
      nextVacant.links.prev = INVALID;
      nextVacant.links.next = INVALID;
    } else {
      KERFUFFLE_ASSERT(_count == CAPACITY);

      _vacantHead = INVALID;
      _vacantTail = INVALID;
    }

    KERFUFFLE_IF_ASSERT(verifyStructure());

    new (&cell.item) Item{std::forward<TA>(args)...};

    return result;
  } else {
    // full
    KERFUFFLE_ASSERT(_vacantHead == INVALID);
    KERFUFFLE_ASSERT(_vacantTail == INVALID);
    KERFUFFLE_ASSERT(_count == CAPACITY);
    KERFUFFLE_BREAK();

    return INVALID;
  }
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC>
void List<T, NC>::remove(const Index i) {
  KERFUFFLE_ASSERT(i < CAPACITY && _count);

  auto& fresh = _cells[i];

  if (_count < CAPACITY) {
    KERFUFFLE_ASSERT(_vacantHead < CAPACITY);
    KERFUFFLE_ASSERT(_vacantTail < CAPACITY);

    fresh.links.prev = INVALID;
    fresh.links.next = _vacantHead;

    auto& head      = _cells[_vacantHead];
    head.links.prev = i;

    _vacantHead = i;
  } else {
    // 0 -> 1
    KERFUFFLE_ASSERT(_count == CAPACITY);
    KERFUFFLE_ASSERT(_vacantHead == INVALID);
    KERFUFFLE_ASSERT(_vacantTail == INVALID);

    fresh.links.prev = INVALID;
    fresh.links.next = INVALID;

    _vacantHead = i;
    _vacantTail = i;
  }

  --_count;

  KERFUFFLE_IF_ASSERT(verifyStructure());
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC>
T& List<T, NC>::operator[](const Index i) {
  KERFUFFLE_IF_ASSERT(verifyStructure());

  return _cells[i].item;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, LongIndex NC>
const T& List<T, NC>::operator[](const Index i) const {
  KERFUFFLE_IF_ASSERT(verifyStructure());

  return _cells[i].item;
}

//------------------------------------------------------------------------------

#ifdef KERFUFFLE_ENABLE_ASSERT

template <typename T, LongIndex NC>
void List<T, NC>::verifyStructure(const Index occupied) const {
  if (_count < CAPACITY) {
    KERFUFFLE_ASSERT(_vacantHead < CAPACITY);
    KERFUFFLE_ASSERT(_vacantTail < CAPACITY);

    KERFUFFLE_ASSERT(_cells[_vacantHead].links.prev == INVALID);
    KERFUFFLE_ASSERT(_cells[_vacantTail].links.next == INVALID);

    auto emptyCount = 1;

    for (auto c = _vacantHead; c != _vacantTail;) {
      KERFUFFLE_ASSERT(occupied != c);

      const auto& current = _cells[c];

      const auto f = current.links.next;
      if (f != INVALID) {
        // next
        const auto& following = _cells[f];

        KERFUFFLE_ASSERT(following.links.prev == c);

        c = f;
        continue;
      } else {
        // end
        KERFUFFLE_ASSERT(_vacantTail == c);
        KERFUFFLE_ASSERT(_count == CAPACITY - emptyCount);

        break;
      }
    }
  } else {
    KERFUFFLE_ASSERT(_vacantHead == INVALID);
    KERFUFFLE_ASSERT(_vacantTail == INVALID);
  }
}

#endif

////////////////////////////////////////////////////////////////////////////////

}  // namespace detail
}  // namespace kerfuffle
