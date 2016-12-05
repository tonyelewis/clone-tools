/// \file
/// \brief The is_clone_ptr header

#ifndef CLONE_TYPE_IS_CLONE_PTR_H
#define CLONE_TYPE_IS_CLONE_PTR_H

#include "clone/detail/type_trait_helper.hpp"
#include "clone/type/detail/is_template_of_type.hpp"

#include <type_traits>

namespace clone_tools { template <typename T> class clone_ptr; }

namespace clone_tools {

	/// \brief Whether some type is a `clone_tools::clone_ptr<...>`
	///
	/// Note that this is strict: it only evaluates to true for `clone_ptr< ... >`; it's false for:
	///  *       `clone_ptr< ... > &`
	///  * `const clone_ptr< ... >`
	///  * `const clone_ptr< ... > &`
	///
	/// For a less strict version, use is_clone_ptr_after_decay
	template <typename T> struct is_clone_ptr : detail::is_template_of_type< T, ::clone_tools::clone_ptr > {};

	/// \brief Whether some type is a `clone_tools::clone_ptr<...>`
	///
	/// This non-strict version is true for all of:
	///  *       `clone_ptr< ... >`
	///  *       `clone_ptr< ... > &`
	///  * `const clone_ptr< ... >`
	///  * `const clone_ptr< ... > &`
	template <typename T> struct is_clone_ptr_after_decay : detail::is_template_of_type< detail::decay_t< T >, ::clone_tools::clone_ptr > {};

#ifdef __cpp_variable_templates

	/// \brief Variable template for whether some type is a `clone_tools::clone_ptr<...>`
	template <typename T>
	static constexpr bool is_clone_ptr_v             = is_clone_ptr            <T>::value;

	/// \brief Variable template for whether some type is a `clone_tools::clone_ptr<...>`
	template <typename T>
	static constexpr bool is_clone_ptr_after_decay_v = is_clone_ptr_after_decay<T>::value;

#endif // __cpp_variable_templates


} // namespace clone_tools

#endif
