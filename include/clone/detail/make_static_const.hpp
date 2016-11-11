/// \file
/// \brief The make_static_const header

#ifndef CLONE_DETAIL_MAKE_STATIC_CONST_H
#define CLONE_DETAIL_MAKE_STATIC_CONST_H

// Tested 11th November 2016: __cpp_variable_templates correctly set in C++11/C++14 modes by g++ 6.2 and clang++ 3.8.1
#ifdef __cpp_variable_templates



namespace clone_tools {
	namespace detail {

		/// \brief Simple trick to avoid ODR violations
		///
		/// Hat-tip: Eric Niebler, eg see https://github.com/ericniebler/range-v3
		template <class T>
		constexpr T make_static_const{};

	} // namespace detail
} // namespace clone_tools

#define MAKE_STATIC_CONST( type, name ) inline namespace { constexpr auto const &name = clone_tools::detail::make_static_const<type>; }



#else // __cpp_variable_templates



namespace clone_tools {
	namespace detail {

		/// \brief Simple trick to avoid ODR violations
		///
		/// Hat-tip: Eric Niebler, eg see https://github.com/ericniebler/range-v3
		template <typename T>
		struct make_static_const {
			static constexpr T value{};
		};

		/// \brief TODOCUMENT
		template<typename T>
		constexpr T make_static_const<T>::value;

	} // namespace detail
} // namespace clone_tools

#define MAKE_STATIC_CONST( type, name ) inline namespace { constexpr auto const &name = clone_tools::detail::make_static_const<type>::value; }



#endif // __cpp_variable_templates


#endif
