/// \file
/// \brief The clone utility header

#ifndef CLONE_UTILITY_H
#define CLONE_UTILITY_H

#include "clone/detail/make_unique.hpp"

#include <cassert>
#include <memory>

namespace clone_tools {

	namespace detail {

		/// \brief TODOCUMENT
		template <typename... Ts> inline void ignore_unused(const Ts &...) {}

	} // namespace detail

	/// \brief Standard approach to achieving a virtual copy-ctor
	///
	/// This allows a (non-virtual interface) clone() method to be implemented in one simple line:
	///
	/// ~~~~~.cpp
	/// return check_uptr_clone_against_this( do_clone(), *this );
	/// ~~~~~
	template <typename B, typename D>
	inline std::unique_ptr<B> check_uptr_clone_against_this(std::unique_ptr<B>  arg_clone, ///< TODOCUMENT
	                                                        const D            &arg_clonee ///< TODOCUMENT
	                                                        ) {
		assert( typeid( *arg_clone ) == typeid( arg_clonee ) );
		detail::ignore_unused( arg_clonee );
		return arg_clone;
	}

	/// \brief Make a unique_ptr clone of the specified object using its copy-ctor
	///
	/// This allows do_clone() methods to be implemented in one simple line:
	///
	/// ~~~~~.cpp
	/// return { make_uptr_clone( *this ) };
	/// ~~~~~
	template <typename T>
	inline auto make_uptr_clone(const T &arg_clonee ///< The object to be cloned (via its copy-ctor) into a unique_ptr
	                            ) -> decltype( detail::make_unique<T>( arg_clonee ) ) {
		return detail::make_unique<T>( arg_clonee );
	}

} // namespace clone_tools

#endif

