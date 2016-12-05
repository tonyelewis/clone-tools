/// \file
/// \brief The void_t header

#ifndef CLONE_TYPE_DETAIL_VOID_T_H
#define CLONE_TYPE_DETAIL_VOID_T_H

namespace clone_tools {
	namespace detail {

		/// \brief Implementation for void_t, below
		template <typename... Ts>
		struct make_void {
			using type = void;
		};

		/// \brief The standard `void_t` to be added to std in C++17
		template <typename... Ts>
		using void_t = typename make_void<Ts...>::type;

	} // namespace detail
} // namespace clone_tools

#endif
