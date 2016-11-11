/// \file
/// \brief The not_the_same_thing header

#ifndef CLONE_DETAIL_NOT_THE_SAME_THING_H
#define CLONE_DETAIL_NOT_THE_SAME_THING_H

#include <type_traits>

#include "clone/detail/make_static_const.hpp"
#include "clone/detail/type_trait_helper.hpp"

namespace clone_tools {
	namespace detail {

		/// \brief Check whether two values are the same thing
		///        (ie have the same static type and same address)
		///
		/// It should be easy for the compiler to optimise calls with
		/// different (decayed) argument types to `true`
		struct not_the_same_thing_fn final {
			not_the_same_thing_fn(const not_the_same_thing_fn &) = delete;
			void operator=(const not_the_same_thing_fn &)        = delete;

		private:

			/// \brief TODOCUMENT
			template <typename Lhs, typename Rhs>
			constexpr bool tagged_dispatchee(const Lhs        &, ///< TODOCUMENT
			                                 const Rhs        &, ///< TODOCUMENT
			                                 const std::false_type &
			                                 ) const {
				return true;
			}

			/// \brief TODOCUMENT
			template <typename Common>
			constexpr bool tagged_dispatchee(const Common    &arg_lhs, ///< TODOCUMENT
			                                 const Common    &arg_rhs, ///< TODOCUMENT
			                                 const std::true_type &
			                                 ) const {
				return ( &arg_lhs != &arg_rhs );
			}

		public:

			/// \brief TODOCUMENT
			///
			/// Tag dispatch makes it clearer to see that this will do what we expect
			/// than trying overloads/specialisations of operator() templates
			template <typename Lhs, typename Rhs>
			constexpr bool operator()(const Lhs &arg_lhs, ///< TODOCUMENT
			                          const Rhs &arg_rhs  ///< TODOCUMENT
			                          ) const {
				return tagged_dispatchee(
					arg_lhs,
					arg_rhs, 
					std::integral_constant<bool, is_decayed_same<Lhs, Rhs>::value >{}
				);
			}
		};

		MAKE_STATIC_CONST( not_the_same_thing_fn, not_the_same_thing )

	} // namespace detail
} // namespace clone_tools

#endif
