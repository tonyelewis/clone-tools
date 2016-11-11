/// \file
/// \brief The is_member_cloneable_base concept template class

#ifndef CLONE_DETAIL_IS_MEMBER_CLONEABLE_BASE_H
#define CLONE_DETAIL_IS_MEMBER_CLONEABLE_BASE_H

#include "clone/detail/tag.hpp"
#include "clone/detail/type_trait_helper.hpp"
#include "clone/type/is_unique_ptr.hpp"

#include <memory>
#include <type_traits>

namespace clone_tools {
	namespace detail {

		/// \brief Implementation for is_member_cloneable_base and member_clone_type that uses the required expressions
		struct member_clones_expr_impl final {
			member_clones_expr_impl()  = delete;
			~member_clones_expr_impl() = delete;

			/// \brief TODOCUMENT
			template <typename T>
			static auto requires() -> decltype( std::declval<T>().clone() );
		};


		/// \brief Implmentation for member_clones and member_clone_type that offers two functions which can be selected with SFINAE
		struct member_clones_impl final {
			member_clones_impl()  = delete;
			~member_clones_impl() = delete;

			/// \brief Standard overload that will be selected if the other one is rejected under SFINAE
			template <typename T>
			static std::pair<
				std::false_type,
				does_not_member_clone_tag
			> sfinae_fn(...);

			/// \brief More-specific overload that will be selected if it isn't rejected under SFINAE
			///        for member_clones_expr_impl::requires<T> being invalid
			template <typename T>
			static std::pair<
				std::true_type,
				decltype( member_clones_expr_impl::requires< T >() )
			> sfinae_fn( decltype( member_clones_expr_impl::requires< T >() ) * = 0 );
		};




		/// \brief Implementation for is_member_cloneable_base and member_clone_type that uses the required expressions
		struct is_member_cloneable_base_expr_impl final {
			is_member_cloneable_base_expr_impl()  = delete;
			~is_member_cloneable_base_expr_impl() = delete;

			/// \brief TODOCUMENT
			template <typename T>
			static auto requires() -> decltype( std::unique_ptr<T>{ std::declval<T>().clone() } );

		};

		/// \brief Implmentation for is_member_cloneable_base and member_clone_type that offers two functions which can be selected with SFINAE
		struct is_member_cloneable_base_impl final {
			is_member_cloneable_base_impl()  = delete;
			~is_member_cloneable_base_impl() = delete;

			/// \brief Standard overload that will be selected if the other one is rejected under SFINAE
			template <typename T>
			static std::false_type sfinae_fn(...);

			/// \brief More-specific overload that will be selected if it isn't rejected under SFINAE
			///        for member_cloneable_base_impl::requires<T> being invalid
			template <typename T>
			static std::true_type sfinae_fn( decltype( is_member_cloneable_base_expr_impl::requires< T >() ) * = 0 );
		};

	} // namespace detail

	/// \brief Meta-function to compute bool `::value` indicating whether T has a clone() method
	template <typename T>
	struct member_clones final {
		member_clones()  = delete;
		~member_clones() = delete;

		static constexpr bool value = detail::tuple_0_t< decltype( detail::member_clones_impl::template sfinae_fn<T>( 0 ) ) >::value;
	};

	/// \brief Meta-function to compute the unique_ptr<> type that T's clone() method returns
	template <typename T,
	          bool Permit = false>
	struct member_clone_type final {
		member_clone_type()  = delete;
		~member_clone_type() = delete;

		// If T fails member_clones then generate compile-time error
		static_assert( member_clones<T>::value || Permit,
			"Cannot deduce what this member_clone_type because it fails member_clones" );

		/// \brief Type alias for the type that T's clone() method returns
		using type = detail::tuple_1_t< decltype( detail::member_clones_impl::template sfinae_fn<T>( 0 ) ) >;
	};

	/// \brief Helper type-alias to get the type that T's clone() method returns
	template <typename T>
	using member_clone_type_t = typename member_clone_type<T>::type;

	/// \brief Meta-function to compute bool `::value` indicating whether T has a clone() method that returns
	///        a unique_ptr type that can be put into a std::unique_ptr<T> (via its ctor and assignment operator)
	///
	/// This indicates whether a `clone_ptr<T>` would make sense using a member clone() function.
	template <typename T>
	struct is_member_cloneable_base final {
		is_member_cloneable_base()  = delete;
		~is_member_cloneable_base() = delete;

		static constexpr bool value = member_clones< T >::value
		                              && is_unique_ptr< typename member_clone_type< T, true >::type >::value
		                              && decltype( detail::is_member_cloneable_base_impl::template sfinae_fn< T >( 0 ) )::value;
	};

	/// \brief Meta-function to compute bool `::value` indicating whether T has a clone() method that returns U
	///
	/// \todo Is this used?
	template <typename T, typename U>
	struct member_clones_to {
		static constexpr bool value = member_clones<T>::value && std::is_same<member_clone_type_t<T>, U>::value;
	};

// Tested 11th November 2016: __cpp_variable_templates correctly set in C++11/C++14 modes by g++ 6.2 and clang++ 3.8.1
#ifdef __cpp_variable_templates

	/// \brief Helper variable template to compute whether T has a clone() method
	template <typename T>
	static constexpr bool member_clones_v            = member_clones           <T>::value;

	/// \brief Helper variable template to compute whether T has a clone() method that returns
	///        a unique_ptr<> type that can be put into a std::unique_ptr<T> (via its ctor and assignment operator)
	template <typename T>
	static constexpr bool is_member_cloneable_base_v = is_member_cloneable_base<T>::value;

	/// \brief Helper variable template to compute whether T has a clone() method that returns U
	///
	/// \todo Is this used?
	template <typename T, typename U>
	static constexpr bool member_clones_to_v         = member_clones_to   <T, U>::value;

#endif // __cpp_variable_templates

} // namespace member_clone_tools

#endif
