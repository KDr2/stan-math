#ifdef STAN_OPENCL
#include <stan/math/opencl/rev.hpp>
#include <stan/math.hpp>
#include <gtest/gtest.h>
#include <test/unit/math/opencl/util.hpp>

auto log10_functor = [](const auto& a) { return stan::math::log10(a); };

TEST(OpenCL_log10, prim_rev_values_small) {
  Eigen::VectorXd a(8);
  a << -2.2, -0.8, 0.5, 1 + std::numeric_limits<double>::epsilon(), 1.5, 3, 3.4,
      4;
  stan::math::test::compare_cpu_opencl_prim_rev(log10_functor, a);
}

TEST(OpenCL_log10, prim_rev_size_0) {
  int N = 0;

  Eigen::MatrixXd a(N, N);
  stan::math::test::compare_cpu_opencl_prim_rev(log10_functor, a);
}

TEST(OpenCL_log10, prim_rev_values_large) {
  int N = 71;

  Eigen::MatrixXd a = Eigen::MatrixXd::Random(N, N);
  stan::math::test::compare_cpu_opencl_prim_rev(log10_functor, a);
}

#endif
