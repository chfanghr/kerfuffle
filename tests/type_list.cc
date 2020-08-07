//
// Created by 方泓睿 on 2020/8/6.
//

#include "helper.h"

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "cert-err58-cpp"

namespace kerfuffle {
namespace test {
class TypeListTest : public testing::Test {};

TEST_F(TypeListTest, TestIndexSequence) {
  {
    using Type1 = kerfuffle::detail::MakeIndexSequence<0>;
    using Type2 = kerfuffle::detail::IndexSequenceFor<>;

    ASSERT_TRUE((std::is_same<Type1, Type2>::value));
  }
  {
    using Type1 = kerfuffle::detail::MakeIndexSequence<0>;
    using Type2 = kerfuffle::detail::IndexSequenceFor<int>;

    ASSERT_TRUE(!(std::is_same<Type1, Type2>::value));
  }
  {
    using Type1 = kerfuffle::detail::MakeIndexSequence<3>;
    using Type2 = kerfuffle::detail::IndexSequenceFor<int, int, int>;
    using Type3 = kerfuffle::detail::IndexSequenceFor<int, int, double>;
    ASSERT_TRUE((std::is_same<Type1, Type2>::value));
    ASSERT_TRUE((std::is_same<Type1, Type3>::value));
  }
  {
    using Type1 = kerfuffle::detail::IndexSequenceFor<int, double, int>;
    using Type2 = kerfuffle::detail::IndexSequenceFor<int>;

    ASSERT_TRUE(!(std::is_same<Type1, Type2>::value));
  }
}

TEST_F(TypeListTest, TestMergeTypes) {
  {
    using Type1 = kerfuffle::detail::ITL_<int, int, int>;
    using Type2 = kerfuffle::detail::ITL_<double>;

    using TypeMerged = kerfuffle::detail::MergeT<Type1, Type2>::Type;

    ASSERT_TRUE(
        (std::is_same<TypeMerged,
                      kerfuffle::detail::ITL_<int, int, int, double>>::value));
  }
  {
    using Type1 = kerfuffle::detail::ITL_<int, int, int>;
    using Type2 = kerfuffle::detail::ITL_<>;

    using TypeMerged = kerfuffle::detail::MergeT<Type1, Type2>::Type;

    ASSERT_TRUE((std::is_same<TypeMerged,
                              kerfuffle::detail::ITL_<int, int, int>>::value));
  }

  {
    using Type1 = kerfuffle::detail::ITL_<>;
    using Type2 = kerfuffle::detail::ITL_<double>;

    using TypeMerged = kerfuffle::detail::MergeT<Type1, Type2>::Type;

    ASSERT_TRUE(
        (std::is_same<TypeMerged, kerfuffle::detail::ITL_<double>>::value));
  }

  {
    using Type1 = kerfuffle::detail::ITL_<>;
    using Type2 = kerfuffle::detail::ITL_<>;

    using TypeMerged = kerfuffle::detail::MergeT<Type1, Type2>::Type;

    ASSERT_TRUE((std::is_same<TypeMerged, kerfuffle::detail::ITL_<>>::value));
  }
}

TEST_F(TypeListTest, TestPrepend) {
  {
    using Type1         = kerfuffle::detail::ITL_<int, int, int>;
    using Type2         = kerfuffle::detail::ITL_<double>;
    using TypePrepended = kerfuffle::detail::PrependT<Type1, Type2>::Type;
    using TypeExpected =
        kerfuffle::detail::ITL_<kerfuffle::detail::ITL_<int, int, int>, double>;

    ASSERT_TRUE((std::is_same<TypePrepended, TypeExpected>::value));
  }
}
}  // namespace test
}  // namespace kerfuffle
#pragma clang diagnostic pop