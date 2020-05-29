#include <stan/math/rev/meta.hpp>
#include <gtest/gtest.h>

TEST(MetaTraitsRevScal, is_var) {
  using stan::is_var;
  using stan::math::var_value;
  using stan::math::var;
  EXPECT_TRUE(is_var<stan::math::var>::value);
  EXPECT_TRUE((is_var<stan::math::var_value<float>>::value));
  EXPECT_TRUE((is_var<stan::math::var_value<int>>::value));
  EXPECT_FALSE(is_var<stan::math::vari>::value);
  EXPECT_FALSE((is_var<double>::value));
  EXPECT_FALSE((is_var<stan::math::vari_value<int>>::value));
}
