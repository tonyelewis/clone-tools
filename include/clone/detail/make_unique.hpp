/// \file
/// \brief The make_unique header

#ifndef CLONE_DETAIL_MAKE_UNIQUE_H
#define CLONE_DETAIL_MAKE_UNIQUE_H

#include <memory>

namespace clone_tools {

// \todo 
//
// It'd be much better to use __cpp_lib_make_unique here but libc++
// doesn't appear to define such feature checks. Great.
#if __cplusplus < 201401

	/// \brief A standard `make_unique` for when compiling under C++11, which doesn't provide it
	template <typename T, typename... Args>
	std::unique_ptr<T> make_unique(Args&&... args ///< The arguments to forward to T's ctor
	                               ) {
		return std::unique_ptr<T>( new T( std::forward<Args>( args )... ) );
	}

#endif // __cplusplus >= 201401

	namespace detail {

		/// \brief Internal wrapper to call whichever `make_unique` is active
		template <typename T, typename... Args>
		auto make_unique_wrapper(Args&&... args ///< The arguments to be forwarded to the correct `make_unique`

// It'd be much better to use __cpp_lib_make_unique here but libc++
// doesn't appear to define such feature checks. Great.
#if __cplusplus < 201401
		                         ) -> decltype( ::clone_tools::make_unique<T>( std::forward<Args>( args )... ) ) {
			return ::clone_tools::make_unique<T>( std::forward<Args>( args )... ) ;
#else
		                         ) -> decltype( ::std::make_unique<T>( std::forward<Args>( args )... ) ) {
			return ::std::make_unique<T>( std::forward<Args>( args )... ) ;
#endif

		}

	} // namespace detail


} // namespace clone_tools

#endif

