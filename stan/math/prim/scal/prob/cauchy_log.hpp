#ifndef STAN_MATH_PRIM_SCAL_PROB_CAUCHY_LOG_HPP
#define STAN_MATH_PRIM_SCAL_PROB_CAUCHY_LOG_HPP

#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/scal/prob/cauchy_lpdf.hpp>
#include <utility>

namespace stan {
namespace math {

/**
 * The log of the Cauchy density for the specified scalar(s) given
 * the specified location parameter(s) and scale parameter(s). y,
 * mu, or sigma can each either be scalar a vector.  Any vector
 * inputs must be the same length.
 *
 * <p> The result log probability is defined to be the sum of
 * the log probabilities for each observation/mu/sigma triple.
 *
 * @deprecated use <code>cauchy_lpdf</code>
 *
 * @param y (Sequence of) scalar(s).
 * @param mu (Sequence of) location(s).
 * @param sigma (Sequence of) scale(s).
 * @return The log of the product of densities.
 * @tparam T_y Type of scalar outcome.
 * @tparam T_loc Type of location.
 * @tparam T_scale Type of scale.
 */
template <bool propto, typename T_y, typename T_loc, typename T_scale>
inline auto cauchy_log(T_y&& y, T_loc&& mu, T_scale&& sigma) {
  return cauchy_lpdf<propto>(std::forward<T_y>(y), std::forward<T_loc>(mu), std::forward<T_scale>(sigma));
}

/**
 * @deprecated use <code>cauchy_lpdf</code>
 */
template <typename T_y, typename T_loc, typename T_scale>
inline auto cauchy_log(T_y&& y, T_loc&& mu, T_scale&& sigma) {
  return cauchy_lpdf(std::forward<T_y>(y), std::forward<T_loc>(mu), std::forward<T_scale>(sigma));
}

}  // namespace math
}  // namespace stan
#endif
