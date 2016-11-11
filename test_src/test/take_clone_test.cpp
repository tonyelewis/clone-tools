/// \file
/// \brief The clone_ptr test suite

#include "catch/catch.hpp"

#include "test/support/example_cloner.hpp"
#include "clone/take_clone.hpp"

#include <type_traits>

using namespace clone_tools::test;

using clone_tools::detail::decay_t;
using clone_tools::detail::make_unique_wrapper;
using std::is_same;
using std::unique_ptr;

namespace clone_tools {
	namespace test {

		/// \brief TODOCUMENT
		struct constructs_from_int final {
			constructs_from_int(const int &) {}
		};

		/// \brief TODOCUMENT
		template <typename T, typename U>
		using decayed_is_uptr_of = is_same< decay_t<T>, unique_ptr<U> >;
	}
}


// TEST_SUITE(test_structures_test)

template <typename T> class TD;
template <bool     T> class BD;

TEST_CASE( "take_clone_handles_normal_cloneable_type_a" ) {
	const unique_ptr<example_cloner_abstract_base> clone_input_ptr = make_unique_wrapper<example_cloner_concrete1>();

	const auto clone_output_ptr = clone_tools::take_clone( *clone_input_ptr );

	static_assert( decayed_is_uptr_of< decltype( clone_output_ptr ), example_cloner_abstract_base>::value, "" );
	REQUIRE( *clone_output_ptr == *clone_input_ptr );
}

TEST_CASE( "take_clone_handles_normal_cloneable_type_b" ) {
	const unique_ptr<example_cloner_abstract_base> clone_input_ptr = make_unique_wrapper<example_cloner_concrete1>();

	const auto clone_output_ptr = clone_tools::take_clone.operator()<example_cloner_abstract_base>( *clone_input_ptr );

	static_assert( decayed_is_uptr_of< decltype( clone_output_ptr ), example_cloner_abstract_base>::value, "" );
	REQUIRE( *clone_output_ptr == *clone_input_ptr );
}

TEST_CASE( "take_clone_handles_normal_cloneable_type_c" ) {
	const unique_ptr<example_cloner_concrete1> clone_input_ptr = make_unique_wrapper<example_cloner_concrete1>();

	const auto clone_output_ptr = clone_tools::take_clone( *clone_input_ptr );

	static_assert( decayed_is_uptr_of< decltype( clone_output_ptr ), example_cloner_abstract_base>::value, "" );
	REQUIRE( *clone_output_ptr == *clone_input_ptr );
}

// ****************************************************************************************************
// ****************************************************************************************************
// ** Commented out because it's requires a conversion from unique_ptr<base> to unique_ptr<concrete> **
// ** and it isn't reasonable to try clever copy-ctor because may point to subclass evenconcreter    **
// ****************************************************************************************************
// ****************************************************************************************************
// TEST_CASE( "take_clone_handles_normal_cloneable_type_d" ) {
// 	const unique_ptr<example_cloner_concrete1> clone_input_ptr = make_unique_wrapper<example_cloner_concrete1>();

// 	const auto clone_output_ptr = clone_tools::take_clone.operator()<example_cloner_concrete1>( *clone_input_ptr );

// 	static_assert( decayed_is_uptr_of< decltype( clone_output_ptr ), example_cloner_concrete1>::value, "" );
// 	REQUIRE( *clone_output_ptr == *clone_input_ptr );
// }


// TEST_SUITE_END()

