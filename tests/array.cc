//
// Created by 方泓睿 on 2020/8/6.
//

#include "helper.h"

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "cert-err58-cpp"

namespace kerfuffle {
namespace test {
class ArrayTest : public testing::Test {};

using ElementType       = kerfuffle::ShortIndex;
constexpr auto CAPACITY = 10;

using StaticArray = kerfuffle::detail::StaticArray<ElementType, CAPACITY>;
using Array       = kerfuffle::detail::Array<ElementType, CAPACITY>;

TEST_F(ArrayTest, TestStaticArray) {
  StaticArray array{};

  ASSERT_EQ(array.count(), CAPACITY);

  for (unsigned i = 0; i < CAPACITY; i++) array[i] = i;

  for (unsigned i = 0; i < CAPACITY; i++) ASSERT_EQ(array[i], i);

  array.clear();

  for (unsigned i = 0; i < CAPACITY; i++)
    ASSERT_EQ(array[i], kerfuffle::INVALID_SHORT_INDEX);
}

TEST_F(ArrayTest, ArrayTest) {
  Array array{};

  ASSERT_EQ(array.count(), 0);

  for (unsigned i = 0; i < CAPACITY; i++) {
    array.append(ElementType(i));
    ASSERT_EQ(array.count(), i + 1);
  };

  ASSERT_EQ(array.count(), CAPACITY);

  for (unsigned i = 0; i < CAPACITY; i++) {
    ASSERT_EQ(array[i], i);
  };

  array.clear();
  ASSERT_EQ(array.count(), 0);
}
}  // namespace test
}  // namespace kerfuffle
#pragma clang diagnostic pop