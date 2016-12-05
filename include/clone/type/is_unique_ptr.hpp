/// \file
/// \brief The is_unique_ptr header

#ifndef CLONE_TYPE_IS_UNIQUE_PTR_H
#define CLONE_TYPE_IS_UNIQUE_PTR_H

#include "clone/detail/type_trait_helper.hpp"
#include "clone/type/detail/is_template_of_type.hpp"

#include <memory>
#include <type_traits>


namespace clone_tools {

	/// \brief Variable template for whether some type is a `std::unique_ptr<...>`
	///
	/// Note that this is strict: it only evaluates to true for `unique_ptr< ... >`; it's false for:
	///  *       `unique_ptr< ... > &`
	///  * `const unique_ptr< ... >`
	///  * `const unique_ptr< ... > &`
	///
	/// For a less strict version, use is_unique_ptr_after_decay
	template <typename T> struct is_unique_ptr : detail::is_template_of_type< T, std::unique_ptr > {};

	/// \brief Variable template for whether some type is a `std::unique_ptr<...>`
	///
	/// This non-strict version is true for all of:
	///  *       `unique_ptr< ... >`
	///  *       `unique_ptr< ... > &`
	///  * `const unique_ptr< ... >`
	///  * `const unique_ptr< ... > &`
	template <typename T> struct is_unique_ptr_after_decay : detail::is_template_of_type< detail::decay_t< T >, std::unique_ptr > {};

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
