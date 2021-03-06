#include <test/unit/math/test_ad.hpp>

TEST(mathMixMatFun, lub_mat_constrain_4) {
  auto f1 = [](const auto& x, const auto& lb, const auto& ub) {
    return stan::math::lub_constrain(x, lb, ub);
  };
  auto f2 = [](const auto& x, const auto& lb, const auto& ub) {
    stan::return_type_t<decltype(x), decltype(lb), decltype(ub)> lp = 0;
    return stan::math::lub_constrain(x, lb, ub, lp);
  };
  auto f3 = [](const auto& x, const auto& lb, const auto& ub) {
    stan::return_type_t<decltype(x), decltype(lb), decltype(ub)> lp = 0;
    stan::math::lub_constrain(x, lb, ub, lp);
    return lp;
  };

  Eigen::MatrixXd x1(2, 2);
  x1 << 0.7, -1.0, 1.1, -3.8;
  Eigen::MatrixXd x2(2, 2);
  x2 << 1.1, 2.0, -0.3, 1.1;
  Eigen::MatrixXd lb(2, 2);
  lb << 2.7, -1.0, -3.0, 1.1;
  Eigen::MatrixXd ub(2, 2);
  ub << 2.3, -0.5, 1.4, 1.2;

  // Error cases
  double lbsb = 100.0;
  double ubsb = -100.0;

  stan::test::expect_ad_matvar(f1, x1, lb, lb);
  stan::test::expect_ad_matvar(f1, x1, lb, ubsb);
  stan::test::expect_ad_matvar(f1, x1, lbsb, ub);
  stan::test::expect_ad_matvar(f1, x2, lb, lb);
  stan::test::expect_ad_matvar(f1, x2, lb, ubsb);
  stan::test::expect_ad_matvar(f1, x2, lbsb, ub);

  stan::test::expect_ad_matvar(f2, x1, lb, lb);
  stan::test::expect_ad_matvar(f2, x1, lb, ubsb);
  stan::test::expect_ad_matvar(f2, x1, lbsb, ub);
}
