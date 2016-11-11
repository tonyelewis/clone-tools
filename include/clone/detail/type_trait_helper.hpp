/// \file
/// \brief The type_trait_helper header

#ifndef CLONE_DETAIL_TYPE_TRAIT_HELPER_H
#define CLONE_DETAIL_TYPE_TRAIT_HELPER_H

#include <type_traits>
#include <utility>

namespace clone_tools {
	namespace detail {

		/// \brief TODOCUMENT
		template <typename T>
		using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

		/// \brief TODOCUMENT
		template <bool B, typename T, typename F>
		using conditional_t          = typename std::conditional<B, T, F>::type;

		/// \brief TODOCUMENT
		template <typename T>
		using decay_t                = typename std::decay<T>::type;

		/// \brief TODOCUMENT
		template <bool B, typename T = void>
		using enable_if_t            = typename std::enable_if<B ,T>::type;

		/// \brief TODOCUMENT
		template <typename T>
		using remove_const_t         = typename std::remove_const<T>::type;

		/// \brief TODOCUMENT
		template <typename T>
		using remove_reference_t     = typename std::remove_reference<T>::type;

		/// \brief TODOCUMENT
		template <std::size_t I, typename T>
		using tuple_element_t        = typename std::tuple_element<I, T>::type;

		/// \brief TODOCUMENT
		template <typename T>
		using tuple_0_t              = typename std::tuple_element<0, T>::type;

		/// \brief TODOCUMENT
		template <typename T>
		using tuple_1_t              = typename std::tuple_element<1, T>::type;

		/// \brief TODOCUMENT
		template <typename T>
		using unique_ptr_element_t   = typename T::element_type;

		/// \brief TODOCUMENT
		template <typename T, typename U>
		using is_decayed_same        = std::is_same< decay_t< T >, decay_t< U > >;

	} // namespace common
} // namespace clone_tools

#endif

