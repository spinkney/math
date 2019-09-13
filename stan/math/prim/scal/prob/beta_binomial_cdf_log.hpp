#ifndef STAN_MATH_PRIM_SCAL_PROB_BETA_BINOMIAL_CDF_LOG_HPP
#define STAN_MATH_PRIM_SCAL_PROB_BETA_BINOMIAL_CDF_LOG_HPP

#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/scal/prob/beta_binomial_lcdf.hpp>

namespace stan {
namespace math {

/**
 * @deprecated use <code>beta_binomial_lcdf</code>
 */
template <typename T_n, typename T_N, typename T_size1, typename T_size2>
inline auto beta_binomial_cdf_log(T_n&& n, T_N&& N,
                                  T_size1&& alpha, T_size2&& beta) {
  return beta_binomial_lcdf(n, N, alpha, beta);
}

}  // namespace math
}  // namespace stan
#endif
