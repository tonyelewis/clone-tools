/// \file
/// \brief The take_clone header

#ifndef CLONE_TAKE_CLONE_H
#define CLONE_TAKE_CLONE_H

#include "clone/detail/tag.hpp"
#include "clone/detail/is_member_cloneable_base.hpp"
#include "clone/detail/type_trait_helper.hpp"
#include "detail/make_static_const.hpp"

#include <type_traits>
#include <utility>

namespace clone_tools {
	namespace detail {

		/// \brief TODOCUMENT
		template <typename T, typename U>
		using conditional_member_clone_pointee_t = conditional_t< std::is_same< T, clone_to_default_tag >::value,
		                                                          unique_ptr_element_t< member_clone_type_t< U > >,
		                                                          T>;

		/// \brief TODOCUMENT
		///
		/// \todo Remove this from overload set if U fails is_member_cloneable
		template <typename T = clone_to_default_tag,
		          typename U,
		          typename V = detail::conditional_member_clone_pointee_t< T, U > >
		std::unique_ptr<V> take_clone(const U &arg_type) {
			return { arg_type.clone() };
		}


		// /// \brief TODOCUMENT
		// template <typename T>
		// constexpr auto take_clone(T &&arg) -> decltype( std::forward<T>( arg ).clone() ) {
		// 	return std::forward<T>( arg ).clone();
		// }


		// /// \brief TODOCUMENT
		// template <typename T, typename U, bool Copy>
		// T copy_or_clone_impl(const U &);

		// /// \brief TODOCUMENT
		// template <typename T, typename U, bool Copy>
		// T copy_or_clone_impl<T, U, false>(U &&arg
		//                                   ) {
		// 	;
		// }

		// /// \brief TODOCUMENT
		// template <typename T, bool Copy>
		// T copy_or_clone_impl<T, T, true>(T &&arg
		//                                  ) {
		// 	return ;
		// }

		// T == clone_to_default_tag
		// || ( is_member_cloneable_base< remove_reference_t<U> > && member_clones_to< remove_reference_t<U>, std::unique_ptr<T> > )


		struct tagged_take_clone_fns final {
			/// \brief TODOCUMENT
			template <typename T,
			          typename U>
			static constexpr auto take_clone_fn_impl(U &&arg,
			                                         const std::false_type &
			                                         )
			                                         noexcept( noexcept( take_clone( std::forward<U>( arg ) ) ) ) ->
			                                         decltype( take_clone( std::forward<U>( arg ) ) ) {
				return take_clone( std::forward<U>( arg ) );
			}

			/// \brief TODOCUMENT
			template <typename T,
			          typename U>
			static constexpr auto take_clone_fn_impl(U &&arg,
			                                         const std::true_type &
			                                         )
			                                         noexcept( noexcept( take_clone<T>( std::forward<U>( arg ) ) ) ) ->
			                                         decltype( take_clone<T>( std::forward<U>( arg ) ) ) {
				return take_clone<T>( std::forward<U>( arg ) );
			}
		};

		/// \brief TODOCUMENT
		struct take_clone_fn {
		private:
			/// \brief TODOCUMENT
			template <typename T>
			using is_non_default_bool_t = std::integral_constant<bool, ! std::is_same< T, clone_to_default_tag >::value >;

		public:
			/// \brief TODOCUMENT
			template <typename T = clone_to_default_tag,
			          typename U>
			constexpr auto operator()(U &&arg
			                          ) const
			                          noexcept( noexcept( tagged_take_clone_fns::take_clone_fn_impl<T>( std::forward<U>( arg ), is_non_default_bool_t<T>{} ) ) )
			                          -> decltype( tagged_take_clone_fns::take_clone_fn_impl<T>( std::forward<U>( arg ), is_non_default_bool_t<T>{} ) ) {
				return tagged_take_clone_fns::take_clone_fn_impl<T>( std::forward<U>( arg ), is_non_default_bool_t<T>{} );
			}

			take_clone_fn()                       = delete;
			take_clone_fn(const take_clone_fn &)  = delete;
			void operator=(const take_clone_fn &) = delete;
		};

	} // namespace detail

	MAKE_STATIC_CONST( detail::take_clone_fn, take_clone )

} // namespace clone_tools

#endif
