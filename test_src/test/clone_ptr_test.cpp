/// \file
/// \brief The clone_ptr test suite

#include "catch/catch.hpp"

#include "clone/clone_ptr.hpp"
#include "clone/type/is_clone_ptr.hpp"
#include "clone/type/is_unique_ptr.hpp"
#include "clone/utility.hpp"

#include <atomic>

namespace clone_tools { namespace test { } }

using namespace clone_tools::test;
using namespace clone_tools;

using clone_tools::detail::make_unique;
using std::is_same;
using std::unique_ptr;

namespace clone_tools {
	namespace test {

		/// \brief TODOCUMENT
		///
		/// The value is arbitrary and is just used to distinguish between methods from concrete1 / concrete2
		constexpr size_t CONCRETE1_METHOD_RESULT = 3984756;

		/// \brief TODOCUMENT
		///
		/// The value is arbitrary and is just used to distinguish between methods from concrete1 / concrete2
		constexpr size_t CONCRETE2_METHOD_RESULT =     836;

		class clone_ptr_test_abstract_base {
		private:
			/// \brief A counter of the number of clones in the history/ancestry of this clone_ptr_test_abstract_base
			///
			/// This is:
			///  * mutable so that it can be incremented in the const source of copy-constructs/clones
			///  * atomic so that the const members are thread-safe
			mutable std::atomic<size_t> clone_ctr { 0 };

			virtual unique_ptr<clone_ptr_test_abstract_base> do_clone() const = 0;

		public:
			clone_ptr_test_abstract_base() = default;
			clone_ptr_test_abstract_base(const clone_ptr_test_abstract_base &arg_base
			                             ) : clone_ctr ( ++arg_base.clone_ctr ) {
			}
			unique_ptr<clone_ptr_test_abstract_base> clone() const {
				return check_uptr_clone_against_this( do_clone(), *this );
			}
			virtual ~clone_ptr_test_abstract_base() noexcept = default;

			virtual size_t method() const = 0;
			size_t clone_count() const {
				return clone_ctr;
			}
		};


		class clone_ptr_test_concrete1 : public clone_ptr_test_abstract_base {
		private:
			virtual unique_ptr<clone_ptr_test_abstract_base> do_clone() const override final {
				return { make_uptr_clone( *this ) };
			}

		public:
			virtual ~clone_ptr_test_concrete1() noexcept override = default;
			virtual size_t method() const override final {
				return CONCRETE1_METHOD_RESULT;
			}
		};


		class clone_ptr_test_concrete2 : public clone_ptr_test_abstract_base {
		private:
			virtual unique_ptr<clone_ptr_test_abstract_base> do_clone() const override final {
				return { make_uptr_clone( *this ) };
			}

		public:
			virtual ~clone_ptr_test_concrete2() noexcept override = default;
			virtual size_t method() const override final {
				return CONCRETE2_METHOD_RESULT;
			}
		};

		bool operator==(const clone_ptr_test_abstract_base &arg_obj1, ///< TODOCUMENT
		                const clone_ptr_test_abstract_base &arg_obj2  ///< TODOCUMENT
		                ) {
			return
				( typeid( arg_obj1 ) == typeid( arg_obj2 ) )
				&&
				( arg_obj1.method()  == arg_obj2.method()  );
		}

		/// \brief Check that a clone_ptr and smart pointer of same element_type (modulo constness) are both null
		template <typename T, typename U>
		void check_both_null(const clone_ptr<T> &arg_clone_ptr, ///< The clone_ptr<> to check for null-ness
		                     const U            &arg_source_ptr ///< The other smart pointer to check for null-ness
		                     ) {
			using non_const_T       = typename std::remove_const< T                        >::type;
			using non_const_U_elmnt = typename std::remove_const< typename U::element_type >::type;
			static_assert( is_same< non_const_T, non_const_U_elmnt>::value,
				"check_both_null() can only be called by a clone_ptr and another"
				" smart pointer with the same element_type (modulo constness)"
			);
			REQUIRE( ! arg_clone_ptr  );
			REQUIRE( ! arg_source_ptr );
			REQUIRE( arg_clone_ptr.get()  == nullptr );
			REQUIRE( arg_source_ptr.get() == nullptr );
		}

		/// \brief TODOCUMENT
		template <typename T>
		void check_moved(const T                                    &arg_clone_ptr,     ///< TODOCUMENT
		                 const clone_ptr_test_abstract_base * const  arg_source_raw_ptr ///< TODOCUMENT
		                 ) {
			static_assert(
				is_clone_ptr<T>::value || is_unique_ptr<T>::value,
				"check_moved() can ony be called with either a clone_ptr type or a unique_ptr type"
			);
			using non_const_T_elmnt = typename std::remove_const< typename T::element_type >::type;
			static_assert( is_same< non_const_T_elmnt, clone_ptr_test_abstract_base>::value,
				"check_moved() can only be called with a smart pointer type with"
				" element_type clone_ptr_test_abstract_base (or const clone_ptr_test_abstract_base)"
			);

			REQUIRE( arg_clone_ptr                                            );
			REQUIRE( arg_clone_ptr.get()          == arg_source_raw_ptr       );
			REQUIRE( arg_clone_ptr->clone_count() == static_cast<size_t>( 0 ) );
		}

		/// \brief TODOCUMENT
		template <typename T, typename U>
		void check_cloned_from(const T                                    &arg_clone_ptr,      ///< TODOCUMENT
		                       const clone_ptr_test_abstract_base * const  arg_source_raw_ptr, ///< TODOCUMENT
		                       const U                                    &arg_source_ptr      ///< TODOCUMENT
		                       ) {
			constexpr bool is_clone_and_unique = is_clone_ptr <T>::value && is_unique_ptr<U>::value;
			constexpr bool is_unique_and_clone = is_unique_ptr<T>::value && is_clone_ptr <U>::value;
			constexpr bool is_clone_and_clone  = is_clone_ptr <T>::value && is_clone_ptr <U>::value;
			static_assert(
				is_clone_and_unique || is_unique_and_clone || is_clone_and_clone,
				"check_cloned_from() can ony be called with one of:"
				" a clone_ptr type and a unique_ptr type,"
				" a unique_ptr type and a clone_ptr type, or"
				" a unique_ptr type and a clone_ptr type"
			);

			using non_const_T_elmnt = typename std::remove_const< typename T::element_type >::type;
			using non_const_U_elmnt = typename std::remove_const< typename U::element_type >::type;
			static_assert( is_same< non_const_T_elmnt, clone_ptr_test_abstract_base>::value,
				"check_cloned_from() can only be called with a first smart pointer type with"
				" element_type clone_ptr_test_abstract_base (or const clone_ptr_test_abstract_base)"
			);
			static_assert( is_same< non_const_U_elmnt, clone_ptr_test_abstract_base>::value,
				"check_cloned_from() can only be called by a second smart pointer type with"
				" element_type clone_ptr_test_abstract_base (or const clone_ptr_test_abstract_base)"
			);

			REQUIRE( arg_clone_ptr                                             );
			REQUIRE( arg_source_ptr                                            );

			REQUIRE( arg_clone_ptr.get()           != arg_source_raw_ptr       );
			REQUIRE( arg_source_ptr.get()          == arg_source_raw_ptr       );

			REQUIRE( arg_source_ptr->clone_count() == static_cast<size_t>( 1 ) );
			REQUIRE( arg_clone_ptr->clone_count()  == static_cast<size_t>( 1 ) );
		}

	}
}

static_assert(   is_cloneable<      clone_ptr_test_concrete1    >::value, "is_cloneable should allow a  concrete       type with sensible clone method" );
static_assert(   is_cloneable<      clone_ptr_test_concrete2    >::value, "is_cloneable should allow a  concrete       type with sensible clone method" );
static_assert(   is_cloneable<      clone_ptr_test_abstract_base>::value, "is_cloneable should allow an abstract       type with sensible clone method" );

static_assert(   is_cloneable<const clone_ptr_test_concrete1    >::value, "is_cloneable should allow a  concrete, const type with sensible clone method" );
static_assert(   is_cloneable<const clone_ptr_test_concrete2    >::value, "is_cloneable should allow a  concrete, const type with sensible clone method" );
static_assert(   is_cloneable<const clone_ptr_test_abstract_base>::value, "is_cloneable should allow an abstract, const type with sensible clone method" );

static_assert( ! is_cloneable<      int                         >::value, "is_cloneable shouldn't allow       int (because it doesn't have a clone method)" );
static_assert( ! is_cloneable<const int                         >::value, "is_cloneable shouldn't allow const int (because it doesn't have a clone method)" );



// TEST_SUITE(test_structures_test)

TEST_CASE( "concrete_methods_return_different_values" ) {
	clone_ptr_test_concrete1 the_clone_ptr_test_concrete1;
	clone_ptr_test_concrete2 the_clone_ptr_test_concrete2;
	REQUIRE( the_clone_ptr_test_concrete1.method() != the_clone_ptr_test_concrete2.method() );
}

// TEST_SUITE_END()




TEST_CASE( "make_clone_builds" ) {
	const auto ptr = make_clone<clone_ptr_test_concrete1>();
	REQUIRE( ptr                                    );
	REQUIRE( ptr.get() !=     nullptr                 );
	REQUIRE( ptr->method() == CONCRETE1_METHOD_RESULT );
}

TEST_CASE( "dereferences" ) {
	const auto ptr = make_clone<clone_ptr_test_concrete1>();
	clone_ptr_test_abstract_base &ref = *ptr;
	REQUIRE( ref.method() == CONCRETE1_METHOD_RESULT );
}

TEST_CASE( "member_of_pointer_operator" ) {
	const auto ptr1 = make_clone<clone_ptr_test_concrete1>();
	const auto ptr2 = make_clone<clone_ptr_test_concrete2>();
	REQUIRE( ptr1->method() == CONCRETE1_METHOD_RESULT );
}



// \brief Test the clone_ptr's various constructors
//
// Note: Construction from raw pointer is implicitly tested by the use of make_clone
// TEST_SUITE(constructor)

TEST_CASE( "default_constructs" ) {
	const auto const_ptr = clone_ptr<const clone_ptr_test_abstract_base>{};

	REQUIRE( ! const_ptr );
	REQUIRE( const_ptr.get() == nullptr );
}


TEST_CASE( "ctor_from_rvalue_null_clone_ptr_makes_null" ) {
	auto       source_ptr = clone_ptr<clone_ptr_test_abstract_base>{};
	const auto ptr        = clone_ptr<clone_ptr_test_abstract_base>{ std::move( source_ptr ) };
	check_both_null( ptr, source_ptr );
}

TEST_CASE( "ctor_from_lvalue_null_clone_ptr_makes_null" ) {
	const auto source_ptr = clone_ptr<clone_ptr_test_abstract_base>{};
	const auto ptr        = clone_ptr<clone_ptr_test_abstract_base>{ source_ptr };
	check_both_null( ptr, source_ptr );
}

TEST_CASE( "ctor_from_rvalue_null_unique_ptr_makes_null" ) {
	auto       source_ptr = unique_ptr<clone_ptr_test_abstract_base>{};
	const auto ptr        = clone_ptr<clone_ptr_test_abstract_base>{ std::move( source_ptr ) };
	check_both_null( ptr, source_ptr );
}

TEST_CASE( "ctor_from_lvalue_null_uptr_makes_null" ) {
	const auto source_ptr = unique_ptr<clone_ptr_test_abstract_base>{};
	const auto ptr        = clone_ptr<clone_ptr_test_abstract_base>{ source_ptr };
	check_both_null( ptr, source_ptr );
}



TEST_CASE( "ctor_from_rvalue_clone_ptr_of_convertible_type_moves" ) {
	auto               source_ptr = make_clone<clone_ptr_test_concrete1>();
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<clone_ptr_test_abstract_base>{ std::move( source_ptr ) };

	check_moved( ptr, raw_ptr );
}

TEST_CASE( "ctor_from_lvalue_to_unique_ptr_clones" ) {
	const auto         source_ptr = unique_ptr<clone_ptr_test_abstract_base>{ make_unique<clone_ptr_test_concrete1>() };
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<clone_ptr_test_abstract_base>{ source_ptr };

	check_cloned_from( ptr, raw_ptr, source_ptr );
}

TEST_CASE( "ctor_from_rvalue_to_unique_ptr_moves" ) {
	auto               source_ptr = unique_ptr<clone_ptr_test_abstract_base>{ make_unique<clone_ptr_test_concrete1>() };
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<clone_ptr_test_abstract_base>{ std::move( source_ptr ) };

	check_moved( ptr, raw_ptr );
}

TEST_CASE( "ctor_from_lvalue_to_unique_ptr_to_const_clones" ) {
	const auto         source_ptr  = unique_ptr<const clone_ptr_test_abstract_base>{ make_unique<clone_ptr_test_concrete1>() };
	const auto * const raw_ptr     = source_ptr.get();
	const auto         ptr         = clone_ptr<const clone_ptr_test_abstract_base>{ source_ptr };

	check_cloned_from( ptr, raw_ptr, source_ptr );
}

TEST_CASE( "ctor_from_rvalue_to_unique_ptr_to_const_moves" ) {
	auto               source_ptr = unique_ptr<const clone_ptr_test_abstract_base>{ make_unique<clone_ptr_test_concrete1>() };
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<const clone_ptr_test_abstract_base>{ std::move( source_ptr ) };

	check_moved( ptr, raw_ptr );
}

TEST_CASE( "copy_ctor_from_lvalue_clones" ) {
	const auto         source_ptr = make_clone<clone_ptr_test_concrete1, clone_ptr_test_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<clone_ptr_test_abstract_base>{ source_ptr };

	check_cloned_from( ptr, raw_ptr, source_ptr );
}

TEST_CASE( "copy_ctor_from_rvalue_moves" ) {
	auto               source_ptr = make_clone<clone_ptr_test_concrete1, clone_ptr_test_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<clone_ptr_test_abstract_base>{ std::move( source_ptr ) };

	check_moved( ptr, raw_ptr );
}

TEST_CASE( "copy_ctor_from_lvalue_to_const_clones" ) {
	const auto         source_ptr = make_clone<const clone_ptr_test_concrete1, const clone_ptr_test_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<const clone_ptr_test_abstract_base>{ source_ptr };

	check_cloned_from( ptr, raw_ptr, source_ptr );
}

TEST_CASE( "copy_ctor_from_rvalue_to_const_moves" ) {
	auto               source_ptr = make_clone<const clone_ptr_test_concrete1, const clone_ptr_test_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<const clone_ptr_test_abstract_base>{ std::move( source_ptr ) };

	check_moved( ptr, raw_ptr );
}

// TEST_SUITE_END()




// TEST_SUITE(assignment)

TEST_CASE( "assignment_from_rvalue_null_clone_ptr_makes_null" ) {
	auto       source_ptr = clone_ptr<clone_ptr_test_abstract_base>{};
	auto       ptr        = clone_ptr<clone_ptr_test_abstract_base>{};
	ptr = std::move( source_ptr );
	check_both_null( ptr, source_ptr );
}

TEST_CASE( "assignment_from_lvalue_null_clone_ptr_makes_null" ) {
	const auto source_ptr = clone_ptr<clone_ptr_test_abstract_base>{};
	auto       ptr        = clone_ptr<clone_ptr_test_abstract_base>{};
	ptr = source_ptr;
	check_both_null( ptr, source_ptr );
}

TEST_CASE( "assignment_from_rvalue_null_unique_ptr_makes_null" ) {
	auto source_ptr = unique_ptr<clone_ptr_test_abstract_base>{};
	auto ptr        = clone_ptr<clone_ptr_test_abstract_base>{};
	ptr = std::move( source_ptr );
	check_both_null( ptr, source_ptr );
}

TEST_CASE( "assignment_from_lvalue_null_uptr_makes_null" ) {
	const auto source_ptr = unique_ptr<clone_ptr_test_abstract_base>{};
	auto       ptr        = clone_ptr<clone_ptr_test_abstract_base>{};
	ptr = source_ptr;
	check_both_null( ptr, source_ptr );
}


TEST_CASE( "assignment_from_rvalue_clone_ptr_of_convertible_type_moves" ) {
	auto               source_ptr = make_clone<clone_ptr_test_concrete1>();
	const auto * const raw_ptr    = source_ptr.get();
	auto ptr = clone_ptr<clone_ptr_test_abstract_base>{};
	ptr = std::move( source_ptr );

	check_moved( ptr, raw_ptr );
}

/// \todo Should separate this into two tests: move-assignment (ie from rvalue) and copy-assignment
TEST_CASE( "assignment_to_null_from_rvalue_moves" ) {
	auto               source_ptr = make_clone<clone_ptr_test_concrete1, clone_ptr_test_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	auto               ptr        = clone_ptr<clone_ptr_test_abstract_base>{};
	ptr = std::move( source_ptr );

	check_moved( ptr, raw_ptr );
}

TEST_CASE( "assignment_to_null_from_lvalue_clones" ) {
	const auto         source_ptr = make_clone<clone_ptr_test_concrete1, clone_ptr_test_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	auto               ptr        = clone_ptr<clone_ptr_test_abstract_base>{};
	ptr = source_ptr;

	check_cloned_from( ptr, raw_ptr, source_ptr );
}

TEST_CASE( "assignment_to_nonnull_from_rvalue_moves" ) {
	auto               source_ptr = make_clone<clone_ptr_test_concrete1, clone_ptr_test_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	auto               ptr        = make_clone<clone_ptr_test_concrete2, clone_ptr_test_abstract_base>();
	ptr = std::move( source_ptr );
	
	check_moved( ptr, raw_ptr );

	REQUIRE( ptr->method() == CONCRETE1_METHOD_RESULT );
}

TEST_CASE( "assignment_to_nonnull_from_lvalue_clones" ) {
	const auto         source_ptr = make_clone<clone_ptr_test_concrete1, clone_ptr_test_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	auto               ptr        = make_clone<clone_ptr_test_concrete2, clone_ptr_test_abstract_base>();
	ptr = source_ptr;
	
	check_cloned_from( ptr, raw_ptr, source_ptr );

	REQUIRE( ptr->method() == CONCRETE1_METHOD_RESULT );
}

TEST_CASE( "assignment_to_nonnull_from_rvalue_unique_ptr_moves" ) {
	auto               source_ptr = unique_ptr<clone_ptr_test_abstract_base>{ make_unique<clone_ptr_test_concrete1>() };
	const auto * const raw_ptr    = source_ptr.get();
	auto               ptr        = make_clone<clone_ptr_test_concrete2, clone_ptr_test_abstract_base>();
	ptr = std::move( source_ptr );

	check_moved( ptr, raw_ptr );

	REQUIRE( ptr->method() == CONCRETE1_METHOD_RESULT );
}

TEST_CASE( "assignment_to_nonnull_from_lvalue_unique_ptr_clones" ) {
	const auto         source_ptr = unique_ptr<clone_ptr_test_abstract_base>{ make_unique<clone_ptr_test_concrete1>() };
	const auto * const raw_ptr    = source_ptr.get();
	auto               ptr        = make_clone<clone_ptr_test_concrete2, clone_ptr_test_abstract_base>();
	ptr = source_ptr;

	check_cloned_from( ptr, raw_ptr, source_ptr );

	REQUIRE( ptr->method() == CONCRETE1_METHOD_RESULT );
}

// TEST_SUITE_END()




// TEST_SUITE(misc)

TEST_CASE( "resets_to_non_null" ) {
	auto               source_ptr = unique_ptr<clone_ptr_test_abstract_base>{ make_unique<clone_ptr_test_concrete1>() };
	const auto * const raw_ptr    = source_ptr.get();
	auto               ptr        = make_clone<clone_ptr_test_concrete2, clone_ptr_test_abstract_base>();
	ptr.reset( source_ptr.release() );
	
	check_moved( ptr, raw_ptr );

	REQUIRE( ptr->method() == CONCRETE1_METHOD_RESULT );
}

TEST_CASE( "resets_to_null" ) {
	auto ptr = make_clone<clone_ptr_test_concrete1, clone_ptr_test_abstract_base>();
	ptr.reset( nullptr );
	REQUIRE( ! ptr                );
	REQUIRE( ptr.get() == nullptr );
}

TEST_CASE( "swaps" ) {
	auto ptr1 = make_clone<clone_ptr_test_concrete1, clone_ptr_test_abstract_base>();
	auto ptr2 = make_clone<clone_ptr_test_concrete2, clone_ptr_test_abstract_base>();
	REQUIRE( ptr1->method()      == CONCRETE1_METHOD_RESULT  );
	REQUIRE( ptr2->method()      == CONCRETE2_METHOD_RESULT  );
	ptr1.swap( ptr2 );
	REQUIRE( ptr1->method()      == CONCRETE2_METHOD_RESULT  );
	REQUIRE( ptr2->method()      == CONCRETE1_METHOD_RESULT  );
	REQUIRE( ptr1->clone_count() == static_cast<size_t>( 0 ) );
	REQUIRE( ptr2->clone_count() == static_cast<size_t>( 0 ) );
}

// TEST_SUITE_END()



// TEST_SUITE(conversion_to_unique_ptr)

TEST_CASE( "conversion_from_lvalue_clone_ptr_to_unique_ptr_clones" ) {
	const auto         source_ptr = make_clone<clone_ptr_test_concrete1, clone_ptr_test_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = static_cast<unique_ptr<clone_ptr_test_abstract_base>>( source_ptr );

	check_cloned_from( ptr, raw_ptr, source_ptr );
}

TEST_CASE( "conversion_from_rvalue_clone_ptr_to_unique_ptr_moves" ) {
	auto               source_ptr = make_clone<clone_ptr_test_concrete1, clone_ptr_test_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = static_cast<unique_ptr<clone_ptr_test_abstract_base>>( std::move( source_ptr ) );

	check_moved( ptr, raw_ptr );
	}

// TEST_SUITE_END()



// TEST_SUITE(can_clone_from_concrete)

TEST_CASE( "can_clone_from_concrete" ) {
	REQUIRE( clone_and_make_clone      ( clone_ptr_test_concrete1{} )->method() == CONCRETE1_METHOD_RESULT );
	REQUIRE( clone_and_make_const_clone( clone_ptr_test_concrete1{} )->method() == CONCRETE1_METHOD_RESULT );
}

// TEST_SUITE_END()



// TEST_SUITE_END()
