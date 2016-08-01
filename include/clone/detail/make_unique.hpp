/// \file
/// \brief The make_unique header

#ifndef CLONE_DETAIL_MAKE_UNIQUE_H
#define CLONE_DETAIL_MAKE_UNIQUE_H

#include <memory>

namespace clone_tools {
	namespace detail {

		/// \brief TODOCUMENT
		template <typename T, typename... Args>
		std::unique_ptr<T> make_unique(Args&&... args) {
			return std::unique_ptr<T>( new T( std::forward<Args>( args )... ) );
		}

	} // namespace common
} // namespace clone_tools

#endif

