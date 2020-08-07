//
// Created by 方泓睿 on 2020/7/22.
//

#include <fmt/format.h>
#include <kerfuffle.h>

using Type1 = kerfuffle::detail::ITL_<int, int, int>;
using Type2 = kerfuffle::detail::ITL_<double>;

using TypeMerged    = kerfuffle::detail::MergeT<Type1, Type2>::Type;
using TypePrepended = kerfuffle::detail::PrependT<Type1, Type2>::Type;

int main() {
  fmt::print(
      "{}\n{}\n", typeid(TypeMerged).name(), typeid(TypePrepended).name());
}  // DO NOTHING