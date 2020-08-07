//
// Created by 方泓睿 on 2020/8/7.
//
#include "helper.h"

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "cert-err58-cpp"

namespace kerfuffle {
namespace test {
class BitStreamTest : public testing::Test {};

constexpr unsigned CAPACITY = 46;
using WriteStream           = kerfuffle::detail::BitWriteStream<CAPACITY>;
using ReadStream            = kerfuffle::detail::BitReadStream<CAPACITY>;
using StreamBuffer          = typename WriteStream ::Buffer;

TEST_F(BitStreamTest, GeneralTest) {
  StreamBuffer buffer;

  {
    WriteStream stream{buffer};
    stream.write<5>((uint8_t)27);
    stream.write<4>((uint8_t)11);
    stream.write<3>((uint8_t)5);
    stream.write<12>((uint16_t)1472);
    stream.write<21>((uint32_t)1000000);
    ASSERT_EQ(buffer.bitSize, 45);
  }

  {
    ReadStream stream{buffer};
    ASSERT_EQ(stream.read<5>(), 27);
    ASSERT_EQ(stream.read<4>(), 11);
    ASSERT_EQ(stream.read<3>(), 5);
    ASSERT_EQ(stream.read<12>(), 1472);
    ASSERT_EQ(stream.read<21>(), 1000000);
    ASSERT_EQ(stream.cursor(), 45);
  }
}
}  // namespace test

}  // namespace kerfuffle
#pragma clang diagnostic pop