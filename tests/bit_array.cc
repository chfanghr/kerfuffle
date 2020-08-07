//
// Created by 方泓睿 on 2020/8/6.
//

#include "helper.h"

#pragma clang dianostic push
#pragma ide diagnostic  ignored "cert-err58-cpp"

using BitArray = kerfuffle::detail::BitArray<kerfuffle::ShortIndex, 6>;
using Bits     = typename BitArray::Bits;

namespace kerfuffle {
namespace test {
class BitArrayTest : public testing::Test {
 protected:
  BitArray bitArray{};

 public:
  BitArrayTest() { bitArray.clear(); }
};

TEST_F(BitArrayTest, TestStaticMethods) {
  bitArray.clear();
  {
    const Bits bits = bitArray.bits<3, 7>();

    ASSERT_FALSE(bits);

    ASSERT_FALSE(bits.get<0>());
    ASSERT_FALSE(bits.get<1>());
    ASSERT_FALSE(bits.get<2>());
    ASSERT_FALSE(bits.get<3>());
    ASSERT_FALSE(bits.get<4>());
    ASSERT_FALSE(bits.get<5>());
    ASSERT_FALSE(bits.get<6>());
  }

  {
    Bits bits = bitArray.bits<3, 7>();
    bits.set<4>();
  }

  {
    const Bits bits = bitArray.bits<2, 13>();

    ASSERT_FALSE(bits.get<0>());
    ASSERT_FALSE(bits.get<1>());
    ASSERT_FALSE(bits.get<2>());
    ASSERT_FALSE(bits.get<3>());
    ASSERT_FALSE(bits.get<4>());
    ASSERT_FALSE(bits.get<5>());
    ASSERT_FALSE(bits.get<6>());
    ASSERT_FALSE(bits.get<7>());
    ASSERT_FALSE(bits.get<8>());
    ASSERT_FALSE(bits.get<9>());
    ASSERT_FALSE(bits.get<10>());
    ASSERT_FALSE(bits.get<11>());
    ASSERT_TRUE(bits.get<12>());
  }

  {
    Bits bits = bitArray.bits<2, 13>();
    bits.reset<12>();
  }

  {
    const Bits bits = bitArray.bits<3, 7>();

    ASSERT_FALSE(bits);

    ASSERT_FALSE(bits.get<0>());
    ASSERT_FALSE(bits.get<1>());
    ASSERT_FALSE(bits.get<2>());
    ASSERT_FALSE(bits.get<3>());
    ASSERT_FALSE(bits.get<4>());
    ASSERT_FALSE(bits.get<5>());
    ASSERT_FALSE(bits.get<6>());
  }
}

TEST_F(BitArrayTest, TestDynamicMethods) {
  bitArray.clear();
  {
    const Bits bits = bitArray.bits<3, 7>();

    ASSERT_FALSE(bits);

    ASSERT_FALSE(bits.get(0));
    ASSERT_FALSE(bits.get(1));
    ASSERT_FALSE(bits.get(2));
    ASSERT_FALSE(bits.get(3));
    ASSERT_FALSE(bits.get(4));
    ASSERT_FALSE(bits.get(5));
    ASSERT_FALSE(bits.get(6));
  }

  {
    Bits bits = bitArray.bits<3, 7>();
    bits.set(4);
  }

  {
    const Bits bits = bitArray.bits<2, 13>();

    ASSERT_FALSE(bits.get(0));
    ASSERT_FALSE(bits.get(1));
    ASSERT_FALSE(bits.get(2));
    ASSERT_FALSE(bits.get(3));
    ASSERT_FALSE(bits.get(4));
    ASSERT_FALSE(bits.get(5));
    ASSERT_FALSE(bits.get(6));
    ASSERT_FALSE(bits.get(7));
    ASSERT_FALSE(bits.get(8));
    ASSERT_FALSE(bits.get(9));
    ASSERT_FALSE(bits.get(10));
    ASSERT_FALSE(bits.get(11));
    ASSERT_TRUE(bits.get(12));
  }

  {
    Bits bits = bitArray.bits<2, 13>();
    bits.reset(12);
  }

  {
    const Bits bits = bitArray.bits<3, 7>();

    ASSERT_FALSE(bits);

    ASSERT_FALSE(bits.get(0));
    ASSERT_FALSE(bits.get(1));
    ASSERT_FALSE(bits.get(2));
    ASSERT_FALSE(bits.get(3));
    ASSERT_FALSE(bits.get(4));
    ASSERT_FALSE(bits.get(5));
    ASSERT_FALSE(bits.get(6));
  }
}
}  // namespace test
}  // namespace kerfuffle

#pragma clang dianostic pop