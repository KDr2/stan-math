#ifndef STAN_MATH_PRIM_PROB_CAUCHY_CCDF_LOG_HPP
#define STAN_MATH_PRIM_PROB_CAUCHY_CCDF_LOG_HPP

#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/prob/cauchy_lccdf.hpp>

namespace stan {
namespace math {

/** \ingroup prob_dists
 * @deprecated use <code>cauchy_lccdf</code>
 */
template <typename T_y, typename T_loc, typename T_scale>
return_type_t<T_y, T_loc, T_scale> cauchy_ccdf_log(const T_y& y,
                                                   const T_loc& mu,
                                                   const T_scale& sigma) {
  return cauchy_lccdf<T_y, T_loc, T_scale>(y, mu, sigma);
}

}  // namespace math
}  // namespace stan
#endif
