/// \file
/// \brief The clone_ptr header

#ifndef CLONE_CLONE_PTR_H
#define CLONE_CLONE_PTR_H

#include "clone/detail/is_cloneable.hpp"
#include "clone/detail/make_unique.hpp"
#include "clone/type/is_clone_ptr.hpp"
#include "clone/type/is_unique_ptr.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <type_traits>

#ifndef NDEBUG
#include <typeinfo>
#endif

namespace clone_tools {

	/// \brief TODOCUMENT
	template <typename T>
	auto take_clone(const T &arg_type) -> decltype( arg_type.clone() ) {
		return arg_type.clone();
	}

	namespace detail {

		/// \brief TODOCUMENT
		template <typename From, typename To>
		struct is_clone_convertible final {
			static constexpr bool value = (
				std::is_same       <                 From,                  To  >::value
				||
				std::is_convertible< std::unique_ptr<From>, std::unique_ptr<To> >::value
			);

			is_clone_convertible()  = delete;
			~is_clone_convertible() = delete;
		};

		/// \brief TODOCUMENT
		struct not_the_same_thing_impl final {
			/// \brief TODOCUMENT
			template <typename LHS, typename RHS>
			bool operator()(const LHS &/*arg_lhs*/, ///< TODOCUMENT
			                const RHS &/*arg_rhs*/  ///< TODOCUMENT
			                ) const {
				return true;
			}

			/// \brief TODOCUMENT
			template <typename LHS>
			bool operator()(const LHS &arg_lhs, ///< TODOCUMENT
			                const LHS &arg_rhs  ///< TODOCUMENT
			                ) const {
				
				return ( &arg_lhs != &arg_rhs );
			}
		};

		/// \brief TODOCUMENT
		constexpr not_the_same_thing_impl not_the_same_thing{};

		/// \brief TODOCUMENT
		template <typename T>
		using default_clone_element_t = unique_ptr_element_t< decltype( take_clone( std::declval<const T &>() ) ) >;

	}

	/// \brief A light wrapper for std::unique_ptr that provides a copy ctor and assignment operator
	///        by using the fact that the template class provides a sensible clone method.
	///
	/// is_cloneable is used to check (at compile time) that the template class does indeed
	/// have a sensible clone method.
	///
	/// \todo Consider adding handling of deleter types
	template <typename T>
	class clone_ptr final {
	private:
		static_assert( is_cloneable<T>::value,
			"clone_ptr<T> can only be used for cloneable T" );

		/// \brief TODOCUMENT
		template <typename U>
		struct is_convertible_from final {
			static constexpr bool value = detail::is_clone_convertible<U, T>::value;

			is_convertible_from()  = delete;
			~is_convertible_from() = delete;
		};

		template <typename U>
		friend class clone_ptr;

		/// \brief TODOCUMENT
		std::unique_ptr<T> ptr;

		/// \brief TODOCUMENT
		struct lvalue_smart_pointer_ctor_tag {
			lvalue_smart_pointer_ctor_tag(const lvalue_smart_pointer_ctor_tag &) = delete;
			void operator=(const lvalue_smart_pointer_ctor_tag &)                = delete;
		};

		/// \brief TODOCUMENT
		///
		/// \todo Create and deploy is_cloneable_v<U>
		template <typename U,
		          typename V = detail::enable_if_t< ( is_clone_ptr<U>::value || is_unique_ptr<U>::value )
		                                            && is_convertible_from<typename U::element_type>::value
		                                            > >
		clone_ptr(const U &arg_ptr, ///< TODOCUMENT
		          const lvalue_smart_pointer_ctor_tag &
		          ) : ptr{ ( arg_ptr.get() == nullptr ) ? std::unique_ptr<T>{}
		                                                : take_clone( *arg_ptr ) } {
			// If the ptr has been constructed from a clone, not just from a default constructed unique_ptr
			// and if in debug mode, then check ptr and arg_ptr both point to something of the same dynamic type
			if ( get() != nullptr ) {
				assert( typeid( *get() ) == typeid( *arg_ptr.get() ) ); // Check the dynamic type returned by clone()
			}
		}


		/// \brief TODOCUMENT
		///
		/// \todo Create and deploy is_cloneable_v<U>
		template <typename U,
		          typename V = detail::enable_if_t< ( is_clone_ptr<U>::value || is_unique_ptr<U>::value )
		                                            && is_convertible_from<typename U::element_type>::value
		                                            > >
		clone_ptr & assign_from_lvalue_smart_pointer(const U &arg_ptr ///< TODOCUMENT
		                                             ) {
			// If not self-assignment
			if ( detail::not_the_same_thing( *this, arg_ptr ) ) {
				// If arg_ptr is a nullptr, just assign a nullptr
				if ( arg_ptr.get() == nullptr ) {
					ptr = nullptr;
				}
				// Otherwise assign the result of taking a clone from the other pointer
				// (and if in debug mode, check this and arg_ptr both point to something of the same dynamic type)
				else {
					ptr = take_clone( *arg_ptr );
					assert( typeid( *get() ) == typeid( *arg_ptr.get() ) );
				}
			}
			return *this;
		}

	public:
		/// \brief A type alias for the type of pointer
		using pointer      = typename std::unique_ptr<T>::pointer;

		/// \brief A type alias for the type of the object managed by this clone_ptr
		using element_type = typename std::unique_ptr<T>::element_type;



		/// @{

		/// \brief Default constructor, creates a unique_ptr that owns nothing
		constexpr clone_ptr() noexcept = default;

		/// @}



		/// @{

		// /// \brief Creates a unique_ptr that owns nothing
		constexpr clone_ptr(std::nullptr_t) noexcept : ptr( nullptr ) {}


		/// \brief Ctor from a raw pointer
		///
		/// Pass through to unique_ptr's ctor from a pointer
		///
		/// Is explicit because implicit conversions from raw pointers can be unhelpful
		explicit clone_ptr(T * arg_ptr ///< TODOCUMENT
		                   ) : ptr{ arg_ptr } {
		}

		/// @}



		/// @{

		/// \brief A copy constructor which uses the clone method of T
		clone_ptr(const clone_ptr &arg_ptr ///< TODOCUMENT
		          ) : clone_ptr( arg_ptr, lvalue_smart_pointer_ctor_tag{} ) {
		}

		/// \brief A move-ctor that uses the default behaviour (ie the move-ctor of unique_ptr)
		clone_ptr(clone_ptr &&) noexcept = default;

		/// \brief TODOCUMENT
		template <typename U,
		          typename V = detail::enable_if_t< is_convertible_from<U>::value > >
		clone_ptr(const clone_ptr<U> &arg_ptr ///< The pointer from which this clone_ptr should be constructed
		          ) : clone_ptr( arg_ptr, lvalue_smart_pointer_ctor_tag{} ) {
		}

		/// \brief Ctor from an rvalue-reference to a clone_ptr of some other type convertible to T
		template <typename U,
		          typename V = detail::enable_if_t< is_convertible_from<U>::value > >
		clone_ptr(clone_ptr<U> &&arg_ptr ///< The pointer from which this clone_ptr should be constructed
		          ) noexcept : ptr( std::move( arg_ptr.ptr ) ) {
		}

		/// @}



		/// @{

		/// \brief Ctor from a unique_ptr const lvalue reference, which takes a clone from it
		clone_ptr(const std::unique_ptr<T> &arg_ptr ///< TODOCUMENT
		          ) : clone_ptr( arg_ptr, lvalue_smart_pointer_ctor_tag{} ) {
		}

		/// \brief Ctor from a unique_ptr rvalue reference, which moves it
		clone_ptr(std::unique_ptr<T> &&arg_ptr ///< TODOCUMENT
		          ) noexcept : ptr( std::move( arg_ptr ) ) {
		}

		/// \brief TODOCUMENT
		///
		/// \todo Create and deploy is_cloneable_v<U>
		template <typename U,
		          typename V = detail::enable_if_t< is_convertible_from<U>::value > >
		explicit clone_ptr(const std::unique_ptr<U> &arg_ptr ///< The pointer from which this clone_ptr should be constructed
		                   ) : clone_ptr( arg_ptr, lvalue_smart_pointer_ctor_tag{} ) {
		}

		/// \brief Ctor from an rvalue-reference to a unique_ptr of some other type convertible to T
		template <typename U,
		          typename V = detail::enable_if_t< is_convertible_from<U>::value > >
		explicit clone_ptr(std::unique_ptr<U> &&arg_ptr ///< The pointer from which this clone_ptr should be constructed
		                   ) noexcept : ptr( std::move( arg_ptr ) ) {
		}

		/// @}



		/// @{

		/// \brief TODOCUMENT
		clone_ptr & operator=(std::nullptr_t ///< TODOCUMENT
		                      ) {
			ptr = nullptr;
			return *this;
		}

		/// @}



		/// @{

		/// \brief An assignment operator which uses the clone method of T
		clone_ptr & operator=(const clone_ptr &arg_ptr ///< TODOCUMENT
		                      ) {
			return assign_from_lvalue_smart_pointer( arg_ptr );
		}

		/// \brief A move assignment operator that uses the default behaviour (ie the move assignment operator of unique_ptr)
		clone_ptr & operator=(clone_ptr &&) noexcept = default;

		/// \brief TODOCUMENT
		///
		/// Consider implementing all assignment operators from clone_ptr/unique_ptr
		/// from two private implementation functions: one for lvalue arguments and one for rvalue arguments
		template<typename U,
		         typename V = detail::enable_if_t< is_convertible_from<U>::value > >
		clone_ptr & operator=(const clone_ptr<U> &arg_ptr ///< An rvalue reference to a clone_ptr of some other type convertible to T
		                      ) noexcept {
			return assign_from_lvalue_smart_pointer( arg_ptr );
		}

		/// \brief A pass-through move-assignment operator from a clone_ptr<> of some other type convertible to T
		template<typename U,
		         typename V = detail::enable_if_t< is_convertible_from<U>::value > >
		clone_ptr & operator=(clone_ptr<U> &&arg_ptr ///< An rvalue reference to a clone_ptr of some other type convertible to T
		                      ) noexcept {
			ptr = std::move( arg_ptr.ptr );
			return *this;
		}


		/// \brief A copy assignment operator from a unique_ptr lvalue reference
		clone_ptr & operator=(const std::unique_ptr<T> &arg_ptr ///< TODOCUMENT
		                      ) {
			return assign_from_lvalue_smart_pointer( arg_ptr );
		}

		/// \brief A move assignment operator from a unique_ptr rvalue reference
		clone_ptr & operator=(std::unique_ptr<T> &&arg_ptr ///< TODOCUMENT
		                      ) noexcept {
			ptr = std::move( arg_ptr );
			return *this;
		}

		/// \brief TODOCUMENT
		///
		/// \todo Create and deploy is_cloneable_v<U>
		template<typename U,
		         typename V = detail::enable_if_t< is_convertible_from<U>::value > >
		clone_ptr & operator=(const std::unique_ptr<U> &arg_ptr ///< TODOCUMENT
		                      ) {
			return assign_from_lvalue_smart_pointer( arg_ptr );
		}

		/// \brief TODOCUMENT
		template<typename U,
		         typename V = detail::enable_if_t< is_convertible_from<U>::value > >
		clone_ptr & operator=(std::unique_ptr<U> &&arg_ptr ///< TODOCUMENT
		                      ) noexcept {
			ptr = std::move( arg_ptr );
			return *this;
		}

		/// @}


		/// \brief Pass-through to std::unique_ptr's reset method
		template <typename... Ts>
		void reset(Ts ...args ///< The arguments to pass-through to unique_ptr's reset() method
		           ) {
			ptr.reset( args... );
		}

		/// \brief Pass-through to std::unique_ptr's release() method
		pointer release() {
			return ptr.release();
		}
		/// \brief Pass-through to std::unique_ptr's operator* method
		detail::add_lvalue_reference_t<T> operator*() const {
			return ptr.operator*();
		}
		/// \brief Pass-through to std::unique_ptr's operator-> method
		pointer operator->() const {
			return ptr.operator->();
		}
		/// \brief Pass-through to std::unique_ptr's get method
		pointer get() const {
			return ptr.get();
		}
		/// \brief TODOCUMENT
		explicit operator bool() const {
			return static_cast<bool>( ptr );
		}
		/// \brief Pass through to std::unique_ptr's swap method
		void swap(clone_ptr &arg_clone_ptr ///< TODOCUMENT
		          ) noexcept {
			ptr.swap( arg_clone_ptr.ptr );
		}



		/// @{

		/// \brief TODOCUMENT
		explicit operator std::unique_ptr<T>() const & {
			return take_clone( *ptr );
		}

		/// \brief TODOCUMENT
		explicit operator std::unique_ptr<T>() && {
			return std::move( ptr );
		}

		/// @}
	};

	/// \brief TODOCUMENT
	///
	/// \relates clone_ptr
	template <typename U, typename V>
	bool operator<(const clone_ptr<U> &arg_clone_ptr_a, ///< TODOCUMENT
	               const clone_ptr<V> &arg_clone_ptr_b  ///< TODOCUMENT
	               ) {
		return arg_clone_ptr_a.get() < arg_clone_ptr_b.get();
	}

	namespace detail {

		/// \brief TODOCUMENT
		struct clone_to_default_tag final {
			clone_to_default_tag()                             = delete;
			~clone_to_default_tag()                            = delete;
			clone_to_default_tag(const clone_to_default_tag &) = delete;
			void operator=(const clone_to_default_tag &)       = delete;
		};

		/// \brief TODOCUMENT
		template <typename T, typename U>
		using conditional_clone_default_t = detail::conditional_t< std::is_same<T, detail::clone_to_default_tag>::value, default_clone_element_t<U>, T>;
	}



	/// @{

	/// \brief TODOCUMENT
	///
	/// \relates clone_ptr
	template <typename T,
	          typename U = T,
	          typename V = detail::enable_if_t< detail::is_clone_convertible<T, U>::value >,
	          typename... Args>
	clone_ptr<U> make_clone(Args&&... args
	                        ) {
		return clone_ptr<U>{
			detail::make_unique<T>( std::forward<Args>( args ) ... )
		};
	}

	/// \brief TODOCUMENT
	///
	/// \relates clone_ptr
	template <typename T = detail::clone_to_default_tag,
	          typename U,
	          typename R = detail::conditional_clone_default_t<T, U> >
	clone_ptr<R> make_clone_of_uptr(const std::unique_ptr<U> &arg_unique_ptr ///< TODOCUMENT
	                                ) {
		static_assert( detail::is_clone_convertible<U, R>::value,
			"Cannot convert between pointer types involved in making clone_ptr" );

		return clone_ptr<R>{ arg_unique_ptr };
	}

	/// \brief TODOCUMENT
	///
	/// \relates clone_ptr
	template <typename T = detail::clone_to_default_tag,
	          typename U,
	          typename R = detail::conditional_clone_default_t<T, U> >
	clone_ptr<R> make_clone_of_uptr(std::unique_ptr<U> &&arg_unique_ptr ///< TODOCUMENT
	                                ) {
		static_assert( detail::is_clone_convertible<U, R>::value,
			"Cannot convert between pointer types involved in making clone_ptr" );

		return clone_ptr<R>{ std::move( arg_unique_ptr ) };
	}

	/// \brief TODOCUMENT
	///
	/// \relates clone_ptr
	template <typename T = detail::clone_to_default_tag,
	          typename U,
	          typename R = detail::conditional_clone_default_t<T, U> >
	clone_ptr<const R> make_const_clone_of_uptr(const std::unique_ptr<U> &arg_unique_ptr ///< TODOCUMENT
	                                            ) {
		static_assert( detail::is_clone_convertible<U, R>::value,
			"Cannot convert between pointer types involved in making clone_ptr" );

		return clone_ptr<const R>{ arg_unique_ptr };
	}

	/// \brief TODOCUMENT
	///
	/// \relates clone_ptr
	template <typename T = detail::clone_to_default_tag,
	          typename U,
	          typename R = detail::conditional_clone_default_t<T, U> >
	clone_ptr<const R> make_const_clone_of_uptr(std::unique_ptr<U> &&arg_unique_ptr ///< TODOCUMENT
	                                            ) {
		static_assert( detail::is_clone_convertible<U, R>::value,
			"Cannot convert between pointer types involved in making clone_ptr" );

		return clone_ptr<const R>{ std::move( arg_unique_ptr ) };
	}

	/// \brief TODOCUMENT
	///
	/// \relates clone_ptr
	template <typename T = detail::clone_to_default_tag,
	          typename U,
	          typename R = detail::conditional_clone_default_t<T, U> >
	clone_ptr<R> clone_and_make_clone(const U &arg_object ///< TODOCUMENT
	                                  ) {
		static_assert( is_cloneable<U>::value,
			"Type passed to clone_and_make_clone() must be cloneable" );

		static_assert( detail::is_clone_convertible<U, R>::value,
			"Cannot convert between pointer types involved in making clone_ptr" );

		return make_clone_of_uptr<R>( take_clone( arg_object ) );
	}

	/// \brief TODOCUMENT
	///
	/// \relates clone_ptr
	template <typename T = detail::clone_to_default_tag,
	          typename U,
	          typename R = detail::conditional_clone_default_t<T, U> >
	clone_ptr<const R> clone_and_make_const_clone(const U &arg_object ///< TODOCUMENT
	                                              ) {
		static_assert( is_cloneable<U>::value,
			"Type passed to clone_and_make_const_clone() must be cloneable" );

		static_assert( detail::is_clone_convertible<U, R>::value,
			"Cannot convert between pointer types involved in making clone_ptr" );

		return make_const_clone_of_uptr<const R>( take_clone( arg_object ) );
	}

	/// @}

} // namespace clone

#endif
