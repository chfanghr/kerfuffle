//
// Created by 方泓睿 on 2020/8/7.
//

#define KERFUFFLE_ENABLE_UTILITY_THEORY
#include "helper.h"

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "cert-err58-cpp"

namespace kerfuffle {
namespace test {
template <typename T, unsigned NSample = 10>
void testUniformity(const int average) {
  using Type   = T;
  using Random = kerfuffle::RandomT<Type>;

  Random rng{0};
  int    histogram[NSample] = {0};

  for (int i = 0; i < NSample * average; i++) {
    const auto real = rng.next();
    ASSERT_TRUE(real < Type{1} && real >= Type{0});
    const unsigned n = real * NSample;
    ASSERT_TRUE(n < NSample);

    ++histogram[n];
  }

  for (unsigned i = 0; i < NSample; ++i) {
    const auto delta    = std::abs(average - histogram[i]);
    const auto relative = 1.f * delta / average;

    ASSERT_TRUE(relative < 2.f);
  }
}

class RandomGeneratorTest : public testing::Test {};

TEST_F(RandomGeneratorTest, GeneralTest) { testUniformity<float>(100); }
}  // namespace test
}  // namespace kerfuffle
#pragma clang diagnostic pop