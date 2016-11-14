/// \file
/// \brief The clone_ptr test suite

#include "catch/catch.hpp"

#include "clone/clone_ptr.hpp"
#include "clone/detail/not_the_same_thing.hpp"
#include "clone/type/is_clone_ptr.hpp"
#include "clone/type/is_unique_ptr.hpp"
#include "clone/utility.hpp"
#include "test/support/example_cloner.hpp"

#include <atomic>

using namespace clone_tools::test;
using namespace clone_tools;

using clone_tools::detail::make_unique_wrapper;
using std::is_same;
using std::unique_ptr;

namespace clone_tools {
	namespace test {

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
		                 const example_cloner_abstract_base * const  arg_source_raw_ptr ///< TODOCUMENT
		                 ) {
			static_assert(
				is_clone_ptr<T>::value || is_unique_ptr<T>::value,
				"check_moved() can ony be called with either a clone_ptr type or a unique_ptr type"
			);
			using non_const_T_elmnt = typename std::remove_const< typename T::element_type >::type;
			static_assert( is_same< non_const_T_elmnt, example_cloner_abstract_base>::value,
				"check_moved() can only be called with a smart pointer type with"
				" element_type example_cloner_abstract_base (or const example_cloner_abstract_base)"
			);

			REQUIRE( arg_clone_ptr                                            );
			REQUIRE( arg_clone_ptr.get()          == arg_source_raw_ptr       );
			REQUIRE( arg_clone_ptr->clone_count() == static_cast<size_t>( 0 ) );
		}

		/// \brief TODOCUMENT
		template <typename T, typename U>
		void check_cloned_from(const T                                    &arg_clone_ptr,      ///< TODOCUMENT
		                       const example_cloner_abstract_base * const  arg_source_raw_ptr, ///< TODOCUMENT
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
			static_assert( is_same< non_const_T_elmnt, example_cloner_abstract_base>::value,
				"check_cloned_from() can only be called with a first smart pointer type with"
				" element_type example_cloner_abstract_base (or const example_cloner_abstract_base)"
			);
			static_assert( is_same< non_const_U_elmnt, example_cloner_abstract_base>::value,
				"check_cloned_from() can only be called by a second smart pointer type with"
				" element_type example_cloner_abstract_base (or const example_cloner_abstract_base)"
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

namespace clone_tools {
	namespace test {

		struct has_non_const_clone final {
			unique_ptr<has_non_const_clone> clone();
		};
	}
}

static_assert( sizeof( unique_ptr < example_cloner_abstract_base > ) == sizeof( clone_ptr  < example_cloner_abstract_base > ), "Size of clone_ptr should be the same as the size of unique_ptr" );

static_assert( ! is_member_cloneable_base<      example_cloner_concrete1                                                    >::value, "is_member_cloneable_base should reject a        concrete type with a clone_type that cant convert to a unique_ptr of the type" );
static_assert( ! is_member_cloneable_base<      example_cloner_concrete2                                                    >::value, "is_member_cloneable_base should reject a        concrete type with a clone_type that cant convert to a unique_ptr of the type" );
static_assert(   is_member_cloneable_base<      example_cloner_abstract_base                                                >::value, "is_member_cloneable_base should allow  an       abstract type with sensible clone method"                                      );
static_assert(   is_member_cloneable_base<      example_cloner_abstract_base_base                                           >::value, "is_member_cloneable_base should allow  an       abstract type with sensible clone method"                                      );

static_assert(   member_clones_to        <      example_cloner_concrete1,          unique_ptr<example_cloner_abstract_base> >::value, "member_clones_to should allow this" );
static_assert(   member_clones_to        <      example_cloner_concrete2,          unique_ptr<example_cloner_abstract_base> >::value, "member_clones_to should allow this" );
static_assert(   member_clones_to        <      example_cloner_abstract_base,      unique_ptr<example_cloner_abstract_base> >::value, "member_clones_to should allow this" );
static_assert(   member_clones_to        <      example_cloner_abstract_base_base, unique_ptr<example_cloner_abstract_base> >::value, "member_clones_to should allow this" );

static_assert( ! is_member_cloneable_base<const example_cloner_concrete1                                                    >::value, "is_member_cloneable_base should reject a  const concrete type with a clone_type that cant convert to a unique_ptr of the type" );
static_assert( ! is_member_cloneable_base<const example_cloner_concrete2                                                    >::value, "is_member_cloneable_base should reject a  const concrete type with a clone_type that cant convert to a unique_ptr of the type" );
static_assert(   is_member_cloneable_base<const example_cloner_abstract_base                                                >::value, "is_member_cloneable_base should allow  a  const abstract type with sensible clone method"                                      );
static_assert(   is_member_cloneable_base<const example_cloner_abstract_base_base                                           >::value, "is_member_cloneable_base should allow  a  const abstract type with sensible clone method"                                      );

static_assert(   member_clones_to        <const example_cloner_concrete1,          unique_ptr<example_cloner_abstract_base> >::value, "member_clones_to should allow this" );
static_assert(   member_clones_to        <const example_cloner_concrete2,          unique_ptr<example_cloner_abstract_base> >::value, "member_clones_to should allow this" );
static_assert(   member_clones_to        <const example_cloner_abstract_base,      unique_ptr<example_cloner_abstract_base> >::value, "member_clones_to should allow this" );
static_assert(   member_clones_to        <const example_cloner_abstract_base_base, unique_ptr<example_cloner_abstract_base> >::value, "member_clones_to should allow this" );

static_assert( ! is_member_cloneable_base<      int                                                                         >::value, "is_member_cloneable_base shouldn't allow       int (because it doesn't have a clone method)" );
static_assert( ! is_member_cloneable_base<const int                                                                         >::value, "is_member_cloneable_base shouldn't allow const int (because it doesn't have a clone method)" );

static_assert( ! is_member_cloneable_base<const has_non_const_clone                                                         >::value, "is_member_cloneable_base shouldn't allow const has_non_const_clone (because it doesn't have a const     clone method)" );
static_assert(   is_member_cloneable_base<      has_non_const_clone                                                         >::value, "is_member_cloneable_base should    allow       has_non_const_clone (because it does    have a non-const clone method)" );
static_assert(   member_clones_to        <      has_non_const_clone,               unique_ptr<has_non_const_clone>          >::value, "member_clones_to should say yes for has_non_const_clone and unique_ptr<member_clones_to>)"                     );

constexpr       int  test_constexpr_val_a = 1;
constexpr       int  test_constexpr_val_b = 1;
constexpr const int &test_constexpr_val_c = test_constexpr_val_a;

static_assert(   clone_tools::detail::not_the_same_thing( test_constexpr_val_a, test_constexpr_val_b ), "not_the_same_thing() should return true for different variables"                         );
static_assert( ! clone_tools::detail::not_the_same_thing( test_constexpr_val_a, test_constexpr_val_a ), "not_the_same_thing() should return false for the same variable"                          );

// \todo Come GCC >= 5, use this static assertion instead of the following testcase:
//
// ~~~~~.cpp
// static_assert( ! clone_tools::detail::not_the_same_thing( test_constexpr_val_a, test_constexpr_val_c ), "not_the_same_thing() should return false for a variable and a reference to the variable" );
// ~~~~~
TEST_CASE( "not_the_same_thing__returns_false_for_reference_to_same_thing" ) {
	REQUIRE( ! clone_tools::detail::not_the_same_thing( test_constexpr_val_a, test_constexpr_val_c ) );
}

// TEST_SUITE(test_structures_test)

TEST_CASE( "concrete_methods_return_different_values" ) {
	example_cloner_concrete1 the_example_cloner_concrete1;
	example_cloner_concrete2 the_example_cloner_concrete2;
	REQUIRE( the_example_cloner_concrete1.method() != the_example_cloner_concrete2.method() );
}

// TEST_SUITE_END()

TEST_CASE( "make_clone_builds" ) {
	const auto ptr = make_clone<example_cloner_concrete1>();
	REQUIRE( ptr                                    );
	REQUIRE( ptr.get() !=     nullptr                 );
	REQUIRE( ptr->method() == CONCRETE1_METHOD_RESULT );
}

TEST_CASE( "dereferences" ) {
	const auto ptr = make_clone<example_cloner_concrete1>();
	example_cloner_abstract_base &ref = *ptr;
	REQUIRE( ref.method() == CONCRETE1_METHOD_RESULT );
}

TEST_CASE( "member_of_pointer_operator" ) {
	const auto ptr1 = make_clone<example_cloner_concrete1>();
	const auto ptr2 = make_clone<example_cloner_concrete2>();
	REQUIRE( ptr1->method() == CONCRETE1_METHOD_RESULT );
}



// \brief Test the clone_ptr's various constructors
//
// Note: Construction from raw pointer is implicitly tested by the use of make_clone
// TEST_SUITE(constructor)

TEST_CASE( "default_constructs" ) {
	const auto const_ptr = clone_ptr<const example_cloner_abstract_base>{};

	REQUIRE( ! const_ptr );
	REQUIRE( const_ptr.get() == nullptr );
}


TEST_CASE( "ctor_from_rvalue_null_clone_ptr_makes_null" ) {
	auto       source_ptr = clone_ptr<example_cloner_abstract_base>{};
	const auto ptr        = clone_ptr<example_cloner_abstract_base>{ std::move( source_ptr ) };
	check_both_null( ptr, source_ptr );
}

TEST_CASE( "ctor_from_lvalue_null_clone_ptr_makes_null" ) {
	const auto source_ptr = clone_ptr<example_cloner_abstract_base>{};
	const auto ptr        = clone_ptr<example_cloner_abstract_base>{ source_ptr };
	check_both_null( ptr, source_ptr );
}

TEST_CASE( "ctor_from_rvalue_null_unique_ptr_makes_null" ) {
	auto       source_ptr = unique_ptr<example_cloner_abstract_base>{};
	const auto ptr        = clone_ptr<example_cloner_abstract_base>{ std::move( source_ptr ) };
	check_both_null( ptr, source_ptr );
}

TEST_CASE( "ctor_from_lvalue_null_uptr_makes_null" ) {
	const auto source_ptr = unique_ptr<example_cloner_abstract_base>{};
	const auto ptr        = clone_ptr<example_cloner_abstract_base>{ source_ptr };
	check_both_null( ptr, source_ptr );
}



TEST_CASE( "ctor_from_rvalue_clone_ptr_of_convertible_type_moves" ) {
	auto               source_ptr = make_clone<example_cloner_concrete1>();
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<example_cloner_abstract_base>{ std::move( source_ptr ) };

	check_moved( ptr, raw_ptr );
}

TEST_CASE( "ctor_from_lvalue_to_unique_ptr_clones" ) {
	const auto         source_ptr = unique_ptr<example_cloner_abstract_base>{ make_unique_wrapper<example_cloner_concrete1>() };
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<example_cloner_abstract_base>{ source_ptr };

	check_cloned_from( ptr, raw_ptr, source_ptr );
}

TEST_CASE( "ctor_from_rvalue_to_unique_ptr_moves" ) {
	auto               source_ptr = unique_ptr<example_cloner_abstract_base>{ make_unique_wrapper<example_cloner_concrete1>() };
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<example_cloner_abstract_base>{ std::move( source_ptr ) };

	check_moved( ptr, raw_ptr );
}

TEST_CASE( "ctor_from_lvalue_to_unique_ptr_to_const_clones" ) {
	const auto         source_ptr  = unique_ptr<const example_cloner_abstract_base>{ make_unique_wrapper<example_cloner_concrete1>() };
	const auto * const raw_ptr     = source_ptr.get();
	const auto         ptr         = clone_ptr<const example_cloner_abstract_base>{ source_ptr };

	check_cloned_from( ptr, raw_ptr, source_ptr );
}

TEST_CASE( "ctor_from_rvalue_to_unique_ptr_to_const_moves" ) {
	auto               source_ptr = unique_ptr<const example_cloner_abstract_base>{ make_unique_wrapper<example_cloner_concrete1>() };
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<const example_cloner_abstract_base>{ std::move( source_ptr ) };

	check_moved( ptr, raw_ptr );
}

TEST_CASE( "copy_ctor_from_lvalue_clones" ) {
	const auto         source_ptr = make_clone<example_cloner_concrete1, example_cloner_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<example_cloner_abstract_base>{ source_ptr };

	check_cloned_from( ptr, raw_ptr, source_ptr );
}

TEST_CASE( "copy_ctor_from_rvalue_moves" ) {
	auto               source_ptr = make_clone<example_cloner_concrete1, example_cloner_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<example_cloner_abstract_base>{ std::move( source_ptr ) };

	check_moved( ptr, raw_ptr );
}

TEST_CASE( "copy_ctor_from_lvalue_to_const_clones" ) {
	const auto         source_ptr = make_clone<const example_cloner_concrete1, const example_cloner_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<const example_cloner_abstract_base>{ source_ptr };

	check_cloned_from( ptr, raw_ptr, source_ptr );
}

TEST_CASE( "copy_ctor_from_rvalue_to_const_moves" ) {
	auto               source_ptr = make_clone<const example_cloner_concrete1, const example_cloner_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = clone_ptr<const example_cloner_abstract_base>{ std::move( source_ptr ) };

	check_moved( ptr, raw_ptr );
}

// TEST_SUITE_END()




// TEST_SUITE(assignment)

TEST_CASE( "assignment_from_rvalue_null_clone_ptr_makes_null" ) {
	auto       source_ptr = clone_ptr<example_cloner_abstract_base>{};
	auto       ptr        = clone_ptr<example_cloner_abstract_base>{};
	ptr = std::move( source_ptr );
	check_both_null( ptr, source_ptr );
}

TEST_CASE( "assignment_from_lvalue_null_clone_ptr_makes_null" ) {
	const auto source_ptr = clone_ptr<example_cloner_abstract_base>{};
	auto       ptr        = clone_ptr<example_cloner_abstract_base>{};
	ptr = source_ptr;
	check_both_null( ptr, source_ptr );
}

TEST_CASE( "assignment_from_rvalue_null_unique_ptr_makes_null" ) {
	auto source_ptr = unique_ptr<example_cloner_abstract_base>{};
	auto ptr        = clone_ptr<example_cloner_abstract_base>{};
	ptr = std::move( source_ptr );
	check_both_null( ptr, source_ptr );
}

TEST_CASE( "assignment_from_lvalue_null_uptr_makes_null" ) {
	const auto source_ptr = unique_ptr<example_cloner_abstract_base>{};
	auto       ptr        = clone_ptr<example_cloner_abstract_base>{};
	ptr = source_ptr;
	check_both_null( ptr, source_ptr );
}


TEST_CASE( "assignment_from_rvalue_clone_ptr_of_convertible_type_moves" ) {
	auto               source_ptr = make_clone<example_cloner_concrete1>();
	const auto * const raw_ptr    = source_ptr.get();
	auto ptr = clone_ptr<example_cloner_abstract_base>{};
	ptr = std::move( source_ptr );

	check_moved( ptr, raw_ptr );
}

/// \todo Should separate this into two tests: move-assignment (ie from rvalue) and copy-assignment
TEST_CASE( "assignment_to_null_from_rvalue_moves" ) {
	auto               source_ptr = make_clone<example_cloner_concrete1, example_cloner_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	auto               ptr        = clone_ptr<example_cloner_abstract_base>{};
	ptr = std::move( source_ptr );

	check_moved( ptr, raw_ptr );
}

TEST_CASE( "assignment_to_null_from_lvalue_clones" ) {
	const auto         source_ptr = make_clone<example_cloner_concrete1, example_cloner_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	auto               ptr        = clone_ptr<example_cloner_abstract_base>{};
	ptr = source_ptr;

	check_cloned_from( ptr, raw_ptr, source_ptr );
}

TEST_CASE( "assignment_to_nonnull_from_rvalue_moves" ) {
	auto               source_ptr = make_clone<example_cloner_concrete1, example_cloner_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	auto               ptr        = make_clone<example_cloner_concrete2, example_cloner_abstract_base>();
	ptr = std::move( source_ptr );
	
	check_moved( ptr, raw_ptr );

	REQUIRE( ptr->method() == CONCRETE1_METHOD_RESULT );
}

TEST_CASE( "assignment_to_nonnull_from_lvalue_clones" ) {
	const auto         source_ptr = make_clone<example_cloner_concrete1, example_cloner_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	auto               ptr        = make_clone<example_cloner_concrete2, example_cloner_abstract_base>();
	ptr = source_ptr;
	
	check_cloned_from( ptr, raw_ptr, source_ptr );

	REQUIRE( ptr->method() == CONCRETE1_METHOD_RESULT );
}

TEST_CASE( "assignment_to_nonnull_from_rvalue_unique_ptr_moves" ) {
	auto               source_ptr = unique_ptr<example_cloner_abstract_base>{ make_unique_wrapper<example_cloner_concrete1>() };
	const auto * const raw_ptr    = source_ptr.get();
	auto               ptr        = make_clone<example_cloner_concrete2, example_cloner_abstract_base>();
	ptr = std::move( source_ptr );

	check_moved( ptr, raw_ptr );

	REQUIRE( ptr->method() == CONCRETE1_METHOD_RESULT );
}

TEST_CASE( "assignment_to_nonnull_from_lvalue_unique_ptr_clones" ) {
	const auto         source_ptr = unique_ptr<example_cloner_abstract_base>{ make_unique_wrapper<example_cloner_concrete1>() };
	const auto * const raw_ptr    = source_ptr.get();
	auto               ptr        = make_clone<example_cloner_concrete2, example_cloner_abstract_base>();
	ptr = source_ptr;

	check_cloned_from( ptr, raw_ptr, source_ptr );

	REQUIRE( ptr->method() == CONCRETE1_METHOD_RESULT );
}

// TEST_SUITE_END()



// TEST_SUITE(base_base)

TEST_CASE( "base_base" ) {
	const auto source_ptr = make_clone<example_cloner_concrete1, example_cloner_abstract_base_base>();
	auto       ptr        = clone_ptr<example_cloner_abstract_base_base>{ source_ptr };
}


// TEST_SUITE_END()



// TEST_SUITE(misc)

TEST_CASE( "resets_to_non_null" ) {
	auto               source_ptr = unique_ptr<example_cloner_abstract_base>{ make_unique_wrapper<example_cloner_concrete1>() };
	const auto * const raw_ptr    = source_ptr.get();
	auto               ptr        = make_clone<example_cloner_concrete2, example_cloner_abstract_base>();
	ptr.reset( source_ptr.release() );
	
	check_moved( ptr, raw_ptr );

	REQUIRE( ptr->method() == CONCRETE1_METHOD_RESULT );
}

TEST_CASE( "resets_to_null" ) {
	auto ptr = make_clone<example_cloner_concrete1, example_cloner_abstract_base>();
	ptr.reset( nullptr );
	REQUIRE( ! ptr                );
	REQUIRE( ptr.get() == nullptr );
}

TEST_CASE( "swaps" ) {
	auto ptr1 = make_clone<example_cloner_concrete1, example_cloner_abstract_base>();
	auto ptr2 = make_clone<example_cloner_concrete2, example_cloner_abstract_base>();
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
	const auto         source_ptr = make_clone<example_cloner_concrete1, example_cloner_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = static_cast<unique_ptr<example_cloner_abstract_base>>( source_ptr );

	check_cloned_from( ptr, raw_ptr, source_ptr );
}

TEST_CASE( "conversion_from_rvalue_clone_ptr_to_unique_ptr_moves" ) {
	auto               source_ptr = make_clone<example_cloner_concrete1, example_cloner_abstract_base>();
	const auto * const raw_ptr    = source_ptr.get();
	const auto         ptr        = static_cast<unique_ptr<example_cloner_abstract_base>>( std::move( source_ptr ) );

	check_moved( ptr, raw_ptr );
	}

// TEST_SUITE_END()



// TEST_SUITE(can_clone_from_concrete)

TEST_CASE( "can_clone_from_concrete" ) {
	REQUIRE( clone_and_make_clone      ( example_cloner_concrete1{} )->method() == CONCRETE1_METHOD_RESULT );
	REQUIRE( clone_and_make_const_clone( example_cloner_concrete1{} )->method() == CONCRETE1_METHOD_RESULT );
}

// TEST_SUITE_END()



// TEST_SUITE_END()
