#ifdef STAN_OPENCL
#include <stan/math/prim.hpp>
#include <test/unit/math/opencl/util.hpp>
#include <test/unit/util.hpp>
#include <gtest/gtest.h>
#include <algorithm>

auto subtract_functor
    = [](const auto& a, const auto& b) { return stan::math::subtract(a, b); };

TEST(OpenCLPrim, subtract_v_small_zero) {
  stan::math::vector_d d1(3), d2(3);
  d1 << 1, 2, 3;
  d2 << 3, 2, 1;
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d1, d2);

  stan::math::vector_d d0(0);
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d0, d0);

  double d3 = 3.0;
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d1, d3);
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d3, d1);
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d3, d0);
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d0, d3);
}

TEST(OpenCLPrim, subtract_rv_small_zero) {
  stan::math::row_vector_d d1(3), d2(3);
  d1 << 1, 2, 3;
  d2 << 3, 2, 1;
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d1, d2);

  stan::math::vector_d d0(0);
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d0, d0);

  double d3 = 3.0;
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d1, d3);
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d3, d1);
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d3, d0);
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d0, d3);
}

TEST(OpenCLPrim, subtract_m_small_zero) {
  stan::math::matrix_d d1(3, 3), d2(3, 3);
  d1 << 1, 2, 3, 4, 5, 6, 7, 8, 9;
  d2 << 10, 100, 1000, 0, -10, -12, 2, 4, 8;
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d1, d2);

  stan::math::matrix_d d0(0, 0);
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d0, d0);

  double d3 = 3.0;
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d1, d3);
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d3, d1);
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d3, d0);
  stan::math::test::compare_cpu_opencl_prim(subtract_functor, d0, d3);
}

TEST(OpenCLPrim, subtract_exceptions) {
  stan::math::vector_d vd1(2), vd2(3);
  stan::math::matrix_cl<double> vd11(vd1);
  stan::math::matrix_cl<double> vd22(vd2);
  EXPECT_THROW(stan::math::subtract(vd11, vd22), std::invalid_argument);

  stan::math::row_vector_d rvd1(2), rvd2(3);
  stan::math::matrix_cl<double> rvd11(rvd1);
  stan::math::matrix_cl<double> rvd22(rvd2);
  EXPECT_THROW(stan::math::subtract(rvd11, rvd22), std::invalid_argument);

  stan::math::matrix_d md1(2, 2), md2(3, 3);
  stan::math::matrix_cl<double> md11(md1);
  stan::math::matrix_cl<double> md22(md2);
  EXPECT_THROW(stan::math::subtract(md11, md22), std::invalid_argument);
}

TEST(MathMatrixCL, subtract_tri_value_check) {
  Eigen::MatrixXd a(3, 3);
  a << 1, 2, 3, 4, 5, 6, 7, 8, 9;
  Eigen::MatrixXd b = Eigen::MatrixXd::Ones(3, 3) * 3;
  stan::math::matrix_cl<double> a_cl(a);
  stan::math::matrix_cl<double> b_cl(b);
  stan::math::matrix_cl<double> c_cl(3, 3);
  Eigen::MatrixXd c(3, 3);

  a_cl.view(stan::math::matrix_cl_view::Lower);
  b_cl.view(stan::math::matrix_cl_view::Lower);
  c_cl = a_cl - b_cl;
  EXPECT_EQ(c_cl.view(), stan::math::matrix_cl_view::Lower);
  c = stan::math::from_matrix_cl(c_cl);
  EXPECT_MATRIX_EQ((Eigen::MatrixXd(a.triangularView<Eigen::Lower>())
                    - Eigen::MatrixXd(b.triangularView<Eigen::Lower>())),
                   c);

  c_cl = stan::math::subtract(a_cl, b_cl);
  EXPECT_EQ(c_cl.view(), stan::math::matrix_cl_view::Lower);
  c = stan::math::from_matrix_cl(c_cl);
  EXPECT_MATRIX_EQ((Eigen::MatrixXd(a.triangularView<Eigen::Lower>())
                    - Eigen::MatrixXd(b.triangularView<Eigen::Lower>())),
                   c);

  a_cl.view(stan::math::matrix_cl_view::Lower);
  b_cl.view(stan::math::matrix_cl_view::Upper);
  c_cl = a_cl - b_cl;
  EXPECT_EQ(c_cl.view(), stan::math::matrix_cl_view::Entire);
  c = stan::math::from_matrix_cl(c_cl);
  EXPECT_MATRIX_EQ((Eigen::MatrixXd(a.triangularView<Eigen::Lower>())
                    - Eigen::MatrixXd(b.triangularView<Eigen::Upper>())),
                   c);

  c_cl = stan::math::subtract(a_cl, b_cl);
  EXPECT_EQ(c_cl.view(), stan::math::matrix_cl_view::Entire);
  c = stan::math::from_matrix_cl(c_cl);
  EXPECT_MATRIX_EQ((Eigen::MatrixXd(a.triangularView<Eigen::Lower>())
                    - Eigen::MatrixXd(b.triangularView<Eigen::Upper>())),
                   c);

  a_cl.view(stan::math::matrix_cl_view::Upper);
  b_cl.view(stan::math::matrix_cl_view::Lower);
  c_cl = a_cl - b_cl;
  EXPECT_EQ(c_cl.view(), stan::math::matrix_cl_view::Entire);
  c = stan::math::from_matrix_cl(c_cl);
  EXPECT_MATRIX_EQ((Eigen::MatrixXd(a.triangularView<Eigen::Upper>())
                    - Eigen::MatrixXd(b.triangularView<Eigen::Lower>())),
                   c);

  a_cl.view(stan::math::matrix_cl_view::Upper);
  b_cl.view(stan::math::matrix_cl_view::Lower);
  c_cl = stan::math::subtract(a_cl, b_cl);
  EXPECT_EQ(c_cl.view(), stan::math::matrix_cl_view::Entire);
  c = stan::math::from_matrix_cl(c_cl);
  EXPECT_MATRIX_EQ((Eigen::MatrixXd(a.triangularView<Eigen::Upper>())
                    - Eigen::MatrixXd(b.triangularView<Eigen::Lower>())),
                   c);

  a_cl.view(stan::math::matrix_cl_view::Entire);
  b_cl.view(stan::math::matrix_cl_view::Lower);
  c_cl = a_cl - b_cl;
  EXPECT_EQ(c_cl.view(), stan::math::matrix_cl_view::Entire);
  c = stan::math::from_matrix_cl(c_cl);
  EXPECT_MATRIX_EQ((a - Eigen::MatrixXd(b.triangularView<Eigen::Lower>())), c);

  a_cl.view(stan::math::matrix_cl_view::Entire);
  b_cl.view(stan::math::matrix_cl_view::Lower);
  c_cl = stan::math::subtract(a_cl, b_cl);
  EXPECT_EQ(c_cl.view(), stan::math::matrix_cl_view::Entire);
  c = stan::math::from_matrix_cl(c_cl);
  EXPECT_MATRIX_EQ((a - Eigen::MatrixXd(b.triangularView<Eigen::Lower>())), c);
}
#endif
