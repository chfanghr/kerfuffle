//
// Created by 方泓睿 on 2020/8/6.
//

#include "helper.h"

#pragma clang dianostic push
#pragma ide diagnostic  ignored "cert-err58-cpp"

namespace kerfuffle {
namespace test {
class ListTest : public ::testing::Test {};

static constexpr auto CAPACITY = 16;
using ElementType              = unsigned;

using List = kerfuffle::detail::List<ElementType, CAPACITY>;

static_assert(List::CAPACITY == CAPACITY, "list capacity should be identical");

TEST_F(ListTest, FillDeleteAndReinsertAnElement) {
  List list{};

  ASSERT_EQ(list.count(), 0);

  for (List::Index i = 0; i < CAPACITY; ++i) {
    const auto index = list.emplace(i);

    ASSERT_EQ(index, i);
    ASSERT_EQ(list.count(), i + 1);
  }

  for (List::Index i = 0; i < CAPACITY; ++i) {
    ASSERT_EQ(list[i], i);
  }

  auto testAtIndex = [&](const List::Index idx) {
    ASSERT_EQ(list.count(), CAPACITY);

    list.remove(idx);
    ASSERT_EQ(list.count(), CAPACITY - 1);

    const auto index = list.emplace(unsigned(idx));
    ASSERT_EQ(index, idx);
    ASSERT_EQ(list.count(), CAPACITY);
  };

  testAtIndex(0);
  testAtIndex(CAPACITY / 2);
  testAtIndex(CAPACITY - 1);
}

TEST_F(ListTest, FillDeleteAllAndReinsertAllElements) {
  List list{};

  ASSERT_EQ(list.count(), 0);

  for (List::Index i = 0; i < CAPACITY; ++i) {
    const auto index = list.emplace(i);

    ASSERT_EQ(index, i);
    ASSERT_EQ(list.count(), i + 1);
  }

  for (List::Index i = 0; i < CAPACITY; ++i) {
    ASSERT_EQ(list[i], i);
  }

  {
    ASSERT_EQ(list.count(), CAPACITY);
    for (List::Index i = 0; i < CAPACITY; ++i) {
      list.remove(i);

      ASSERT_EQ(list.count(), CAPACITY - i - 1);
    }
    ASSERT_EQ(list.count(), 0);

    for (List::Index i = 0; i < CAPACITY; ++i) {
      const auto index = list.emplace(i);

      ASSERT_EQ(index, CAPACITY - 1 - i);
      ASSERT_EQ(list.count(), i + 1);
    }
  }

  { ASSERT_EQ(list.count(), CAPACITY); }

  {
    ASSERT_EQ(list.count(), CAPACITY);
    for (List::Index i = 0; i < CAPACITY; ++i) {
      list.remove(CAPACITY - i - 1);

      ASSERT_EQ(list.count(), CAPACITY - i - 1);
    }
    ASSERT_EQ(list.count(), 0);

    for (List::Index i = 0; i < CAPACITY; ++i) {
      const auto index = list.emplace(i);

      ASSERT_EQ(index, i);
      ASSERT_EQ(list.count(), i + 1);
    }
  }
  ASSERT_EQ(list.count(), CAPACITY);
}
}  // namespace test
}  // namespace kerfuffle

#pragma clang dianostic pop
