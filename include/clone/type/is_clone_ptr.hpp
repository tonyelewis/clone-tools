/// \file
/// \brief The is_clone_ptr header

#ifndef CLONE_TYPE_IS_CLONE_PTR_H
#define CLONE_TYPE_IS_CLONE_PTR_H

#include "clone/detail/type_trait_helper.hpp"

#include <type_traits>

namespace clone_tools { template <typename T> class clone_ptr; }

namespace clone_tools {

		namespace detail {

			/// \brief Implementation of is_clone_ptr using non-specialised template inheriting from false-type
			template <typename T>
			struct is_clone_ptr_impl final : public std::false_type {};

			/// \brief Implementation of is_clone_ptr, partially specialised for non-const `std::clone_ptr<...>`
			template <typename T>
			struct is_clone_ptr_impl<clone_ptr<T>> final : public std::true_type {};

		} // namespace detail


		/// \brief Whether some type is a `std::clone_ptr<...>`
		///
		/// Note that this is strict: it only evaluates to true for `clone_ptr< ... >`; it's false for:
		///  *       `clone_ptr< ... > &`
		///  * `const clone_ptr< ... >`
		///  * `const clone_ptr< ... > &`
		///
		/// For a less strict version, use is_clone_ptr_after_decay
		template <typename T>
		struct is_clone_ptr {
			static constexpr bool value = detail::is_clone_ptr_impl<T>::value;

			is_clone_ptr()  = delete;
			~is_clone_ptr() = delete;
		};

		/// \brief Whether some type is a `std::clone_ptr<...>`
		///
		/// This non-strict version is true for all of:
		///  *       `clone_ptr< ... >`
		///  *       `clone_ptr< ... > &`
		///  * `const clone_ptr< ... >`
		///  * `const clone_ptr< ... > &`
		template <typename T>
		struct is_clone_ptr_after_decay {
			static constexpr bool value = is_clone_ptr< detail::decay_t< T > >::value;

			is_clone_ptr_after_decay()  = delete;
			~is_clone_ptr_after_decay() = delete;
		};

} // namespace clone_tools

#endif
