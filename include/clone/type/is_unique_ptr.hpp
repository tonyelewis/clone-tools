/// \file
/// \brief The is_unique_ptr header

#ifndef CLONE_TYPE_IS_UNIQUE_PTR_H
#define CLONE_TYPE_IS_UNIQUE_PTR_H

#include "clone/detail/type_trait_helper.hpp"

#include <memory>
#include <type_traits>

namespace clone_tools {
	namespace detail {

		/// \brief Implementation of is_unique_ptr using non-specialised template inheriting from false-type
		template <typename T>
		struct is_unique_ptr_impl final : public std::false_type {};

		/// \brief Implementation of is_unique_ptr, partially specialised for non-const `std::unique_ptr<...>`
		template <typename T>
		struct is_unique_ptr_impl<std::unique_ptr<T>> final : public std::true_type {};

	} // namespace detail

	/// \brief Variable template for whether some type is a `std::unique_ptr<...>`
	///
	/// Note that this is strict: it only evaluates to true for `unique_ptr< ... >`; it's false for:
	///  *       `unique_ptr< ... > &`
	///  * `const unique_ptr< ... >`
	///  * `const unique_ptr< ... > &`
	///
	/// For a less strict version, use is_unique_ptr_after_decay
	template <typename T>
	struct is_unique_ptr final {
		static constexpr bool value = detail::is_unique_ptr_impl<T>::value;

		is_unique_ptr()  = delete;
		~is_unique_ptr() = delete;
	};

	/// \brief Variable template for whether some type is a `std::unique_ptr<...>`
	///
	/// This non-strict version is true for all of:
	///  *       `unique_ptr< ... >`
	///  *       `unique_ptr< ... > &`
	///  * `const unique_ptr< ... >`
	///  * `const unique_ptr< ... > &`
	template <typename T>
	struct is_unique_ptr_after_decay final {
		static constexpr bool value = is_unique_ptr< detail::decay_t< T > >::value;

		is_unique_ptr_after_decay()  = delete;
		~is_unique_ptr_after_decay() = delete;
	};

#ifdef __cpp_variable_templates

	/// \brief Variable template for whether some type is a `std::unique_ptr<...>`
	template <typename T>
	static constexpr bool is_unique_ptr_v             = is_unique_ptr            <T>::value;

	/// \brief Variable template for whether some type is a `std::unique_ptr<...>`
	template <typename T>
	static constexpr bool is_unique_ptr_after_decay_v = is_unique_ptr_after_decay<T>::value;

#endif // __cpp_variable_templates

} // namespace clone_tools

#endif
