/// \file
/// \brief The is_template_of_type header

#ifndef CLONE_TYPE_DETAIL_IS_TEMPLATE_OF_TYPE_H
#define CLONE_TYPE_DETAIL_IS_TEMPLATE_OF_TYPE_H

#include "clone/type/detail/void_t.hpp"

#include <type_traits>

namespace clone_tools {
	namespace detail {

		/// \brief Type trait for whether the first type is a template of the second template type
		///
		/// This is the generic template, which matches all types that aren't template types
		template <typename T,
		          template <typename...> class U,
		          typename = void_t<>
		          >
		struct is_template_of_type : std::false_type {};

		/// \brief Type trait for whether the first type is a template of the second template type
		///
		/// This is the partial specialisation that matches all template types with type parameters that can be used
		/// to instantiate the specified template type. This just then checks whether whether the two template types
		/// are the same when instantiated with those same type parameters.
		///
		/// The use of void_t is necessary to take this specialisation out when the second (template) type
		/// can't be instantiated with the first type's template parameters. This was motivated by the case of there
		/// being more parameters than the second (template) type can accept
		/// (eg `is_template_of_type<set<int, less<int>, allocator<int>>, pair`),
		/// which otherwise causes compiler errors
		template <template <typename...> class TOut,
		          typename... TIns,
		          template <typename...> class U>
		struct is_template_of_type< TOut< TIns... >, U, void_t< U< TIns... > > > : std::is_same< U< TIns... >, TOut< TIns... > > {};

#ifdef __cpp_variable_templates

		/// \brief Variable template for whether the first type is a template of the second template type
		template <typename T,
		          template <typename...> class U>
		constexpr bool is_template_of_type_v = is_template_of_type<T, U>::value;

#endif // __cpp_variable_templates

	} // namespace detail
} // namespace clone_tools

#endif
