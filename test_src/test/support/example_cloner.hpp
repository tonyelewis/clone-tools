/// \file
/// \brief The example_cloner classes

#ifndef TEST_SUPPORT_EXAMPLE_CLONER_H
#define TEST_SUPPORT_EXAMPLE_CLONER_H

#include "clone/utility.hpp"

#include <atomic>
#include <memory>

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

		static_assert( CONCRETE1_METHOD_RESULT != CONCRETE2_METHOD_RESULT, "The concrete method results must differ for the tests to work" );

		class example_cloner_abstract_base;

		class example_cloner_abstract_base_base {
		private:
			virtual std::unique_ptr<example_cloner_abstract_base> do_clone() const = 0;
		public:
			example_cloner_abstract_base_base() = default;
			virtual ~example_cloner_abstract_base_base() noexcept = default;

			example_cloner_abstract_base_base(const example_cloner_abstract_base_base &) = default;
			example_cloner_abstract_base_base(example_cloner_abstract_base_base &&) = default;
			example_cloner_abstract_base_base & operator=(const example_cloner_abstract_base_base &) = default;
			example_cloner_abstract_base_base & operator=(example_cloner_abstract_base_base &&) = default;

			std::unique_ptr<example_cloner_abstract_base> clone() const {
				return check_uptr_clone_against_this( do_clone(), *this );
			}
		};

		class example_cloner_abstract_base : public example_cloner_abstract_base_base {
		private:
			/// \brief A counter of the number of clones in the history/ancestry of this example_cloner_abstract_base
			///
			/// This is:
			///  * mutable so that it can be incremented in the const source of copy-constructs/clones
			///  * atomic so that the const members are thread-safe
			mutable std::atomic<size_t> clone_ctr { 0 };

		public:
			example_cloner_abstract_base() = default;
			example_cloner_abstract_base(const example_cloner_abstract_base &arg_base ///< TODOCUMENT
			                             ) : example_cloner_abstract_base_base{                      },
			                                 clone_ctr                        { ++arg_base.clone_ctr } {
			}

			virtual size_t method() const = 0;
			size_t clone_count() const {
				return clone_ctr;
			}
		};


		class example_cloner_concrete1 : public example_cloner_abstract_base {
		private:
			virtual std::unique_ptr<example_cloner_abstract_base> do_clone() const override final {
				return { make_uptr_clone( *this ) };
			}

		public:
			virtual size_t method() const override final {
				return CONCRETE1_METHOD_RESULT;
			}
		};


		class example_cloner_concrete2 : public example_cloner_abstract_base {
		private:
			virtual std::unique_ptr<example_cloner_abstract_base> do_clone() const override final {
				return { make_uptr_clone( *this ) };
			}

		public:
			virtual size_t method() const override final {
				return CONCRETE2_METHOD_RESULT;
			}
		};

		inline bool operator==(const example_cloner_abstract_base &arg_obj1, ///< TODOCUMENT
		                       const example_cloner_abstract_base &arg_obj2  ///< TODOCUMENT
		                       ) {
			return
				( typeid( arg_obj1 ) == typeid( arg_obj2 ) )
				&&
				( arg_obj1.method()  == arg_obj2.method()  );
		}

	}
}

#endif
