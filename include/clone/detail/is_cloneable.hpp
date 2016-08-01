/// \file
/// \brief The is_cloneable concept template class

#ifndef CLONE_DETAIL_IS_CLONEABLE_H
#define CLONE_DETAIL_IS_CLONEABLE_H

#include "clone/detail/type_trait_helper.hpp"

#include <memory>
#include <type_traits>

namespace clone_tools {
	namespace detail {

		/// \brief TODOCUMENT
		///
		/// Requirements:
		///  * Must have a nullary method clone() that returns a unique_ptr<...> of T or some base of T
		///
		/// This allows for: T is a const type but T's clone() returns a unique_ptr to non-const T.
		// template <typename T>
		struct cloneable_impl final {

			template <typename T>
			static auto requires() -> decltype( decltype( std::declval<T>().clone() ){ std::unique_ptr<T>{ nullptr } } );

			cloneable_impl()  = delete;
			~cloneable_impl() = delete;
		};



		/// \brief TODOCUMENT
		struct is_cloneable_impl final {
			template <typename T>
			static std::false_type sfinae_fn(...);

			template <typename T>
			static std::true_type sfinae_fn( decltype( cloneable_impl::requires< remove_const_t<T> >() ) * = 0 );

			is_cloneable_impl()  = delete;
			~is_cloneable_impl() = delete;
		};

	} // namespace detail

	/// \brief TODOCUMENT
	template <typename T>
	struct is_cloneable final {
		static constexpr bool value = decltype( detail::is_cloneable_impl::template sfinae_fn<T>( 0 ) )::value;

		is_cloneable()  = delete;
		~is_cloneable() = delete;
	};

} // namespace clone_tools

#endif
