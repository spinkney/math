#ifndef STAN_MATH_REV_CORE_VARI_HPP
#define STAN_MATH_REV_CORE_VARI_HPP

#include <stan/math/rev/core/chainable_alloc.hpp>
#include <stan/math/rev/core/chainablestack.hpp>
#include <stan/math/prim/meta.hpp>
#include <ostream>
#include <type_traits>

namespace stan {
namespace math {

// forward declaration of var
template <typename T, typename>
class var_value;

/**
 * The variable implementation base class.
 *
 * This class is complete (not abstract) and may be used for
 * constants.
 *
 * A variable implementation is constructed with a constant
 * value. It also stores the adjoint for storing the partial
 * derivative with respect to the root of the derivative tree.
 *
 * The chain() method applies the chain rule. Concrete extensions
 * of this class will represent base variables or the result
 * of operations such as addition or subtraction. These extended
 * classes will store operand variables and propagate derivative
 * information via an implementation of chain().
 */
template <typename T, typename>
class vari_value;

template <typename T>
class vari_value<T, std::enable_if_t<std::is_floating_point<T>::value>> {
 private:
  template <typename, typename>
  friend class var_value;

 public:
  using Scalar = T;
  using value_type = Scalar;
  /**
   * The value of this variable.
   */
  const Scalar val_;

  /**
   * The adjoint of this variable, which is the partial derivative
   * of this variable with respect to the root variable.
   */
  Scalar adj_;

  /**
   * Construct a variable implementation from a value.  The
   * adjoint is initialized to zero.
   *
   * All constructed variables are added to the stack.  Variables
   * should be constructed before variables on which they depend
   * to insure proper partial derivative propagation.  During
   * derivative propagation, the chain() method of each variable
   * will be called in the reverse order of construction.
   *
   * @tparam S an Arithmetic type.
   * @param x Value of the constructed variable.
   */
  template <typename S,
            std::enable_if_t<std::is_convertible<S&, Scalar>::value>* = nullptr>
  vari_value(S x) : val_(x), adj_(0.0) {  // NOLINT
    ChainableStack::instance_->var_stack_.push_back(this);
  }

  /**
   * Construct a variable implementation from a value.  The
   *  adjoint is initialized to zero and if `stacked` is `false` this vari
   *  will be moved to the nochain stack s.t. it's chain method will not be
   *  called when calling `grad()`.
   *
   * All constructed variables are added to the stack.  Variables
   *  should be constructed before variables on which they depend
   *  to insure proper partial derivative propagation.  During
   *  derivative propagation, the chain() method of each variable
   *  will be called in the reverse order of construction.
   *
   * @tparam S an Arithmetic type.
   * @param x Value of the constructed variable.
   * @param stacked If false will put this this vari on the nochain stack so
   * that its `chain()` method is not called.
   */
  template <typename S,
            std::enable_if_t<std::is_convertible<S&, Scalar>::value>* = nullptr>
  vari_value(S x, bool stacked) : val_(x), adj_(0.0) {
    if (stacked) {
      ChainableStack::instance_->var_stack_.push_back(this);
    } else {
      ChainableStack::instance_->var_nochain_stack_.push_back(this);
    }
  }

  /**
   * Constructor from vari_value
   * @tparam S An arithmetic type
   * @param x A vari_value
   */
  template <typename S,
            std::enable_if_t<std::is_arithmetic<S>::value>* = nullptr>
  vari_value(const vari_value<S>& x) : val_(x.val_), adj_(x.adj_) {
    ChainableStack::instance_->var_stack_.push_back(this);
  }

  /**
   * Initialize the adjoint for this (dependent) variable to 1.
   * This operation is applied to the dependent variable before
   * propagating derivatives, setting the derivative of the
   * result with respect to itself to be 1.
   */
  inline void init_dependent() { adj_ = 1.0; }

  /**
   * Set the adjoint value of this variable to 0.  This is used to
   * reset adjoints before propagating derivatives again (for
   * example in a Jacobian calculation).
   */
  inline void set_zero_adjoint() { adj_ = 0.0; }

  virtual void chain() {}

  /**
   * Insertion operator for vari. Prints the current value and
   * the adjoint value.
   *
   * @param os [in, out] ostream to modify
   * @param v [in] vari object to print.
   *
   * @return The modified ostream.
   */
  friend std::ostream& operator<<(std::ostream& os, const vari_value<T>* v) {
    return os << v->val_ << ":" << v->adj_;
  }

  /**
   * Allocate memory from the underlying memory pool.  This memory is
   * is managed as a whole externally.
   *
   * Warning: Classes should not be allocated with this operator
   * if they have non-trivial destructors.
   *
   * @param nbytes Number of bytes to allocate.
   * @return Pointer to allocated bytes.
   */
  static inline void* operator new(size_t nbytes) {
    return ChainableStack::instance_->memalloc_.alloc(nbytes);
  }

  /**
   * Delete a pointer from the underlying memory pool.
   *
   * This no-op implementation enables a subclass to throw
   * exceptions in its constructor.  An exception thrown in the
   * constructor of a subclass will result in an error being
   * raised, which is in turn caught and calls delete().
   *
   * See the discussion of "plugging the memory leak" in:
   *   http://www.parashift.com/c++-faq/memory-pools.html
   */
  static inline void operator delete(void* /* ignore arg */) { /* no op */
  }
};

// For backwards compatability the default is double
using vari = vari_value<double>;

class vari_zero_adj : public boost::static_visitor<> {
 public:
  inline void operator()(vari_value<double>*& x) const { x->adj_ = 0.0; }
  inline void operator()(vari_value<float>*& x) const { x->adj_ = 0.0; }
  inline void operator()(vari_value<long double>*& x) const { x->adj_ = 0.0; }
};

class vari_chainer : public boost::static_visitor<> {
 public:
  template <typename T>
  inline void operator()(vari_value<double>*& x) const {
    x->chain();
  }
  inline void operator()(vari_value<float>*& x) const { x->chain(); }
  inline void operator()(vari_value<long double>*& x) const { x->chain(); }
};

class vari_printer : public boost::static_visitor<> {
 public:
  int i_{0};
  std::ostream& o_;
  vari_printer(std::ostream& o, int i) : o_(o), i_(i) {}
  template <typename T>
  void operator()(T*& x) const {
    o_ << i_ << "  " << x << "  " << x->val_ << " : " << x->adj_ << std::endl;
  }
};
}  // namespace math
}  // namespace stan
#endif
