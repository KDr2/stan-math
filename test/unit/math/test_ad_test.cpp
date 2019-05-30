#include <test/unit/math/test_ad.hpp>
#include <gtest/gtest.h>

TEST(test_unit_math_test_ad, to_std_vector) {
  Eigen::VectorXd u(0);
  EXPECT_EQ(0, stan::test::to_std_vector(u).size());

  Eigen::VectorXd x(3);
  x << 1, 2, 3;
  std::vector<double> y = stan::test::to_std_vector(x);
  EXPECT_EQ(3, y.size());
  for (int i = 0; i < 3; ++i)
    EXPECT_EQ(x(i), y[i]);
}

TEST(test_unit_math_test_ad, to_eigen_vector) {
  std::vector<double> u;
  EXPECT_EQ(0, stan::test::to_eigen_vector(u).size());

  std::vector<double> v{1, 2, 3};
  Eigen::VectorXd vv = stan::test::to_eigen_vector(v);
  EXPECT_EQ(3, vv.size());
  for (int i = 0; i < 3; ++i)
    EXPECT_EQ(v[i], vv(i));
}

struct foo {
  template <typename T>
  Eigen::Matrix<T, -1, -1> operator()(
      const Eigen::Matrix<T, -1, -1>& bar) const {
    return -bar;
  }
};

TEST(test_unit_math_test_ad, test_ad) {
  Eigen::MatrixXd x(2, 3);
  x << 1, 2, 3, 4, 5, 6;
  foo f;
  stan::test::expect_ad(f, x);
}
