/// \file
/// \brief The is_unique_ptr test suite

#include "clone/type/is_unique_ptr.hpp"

#include <vector>

using namespace clone_tools;

using std::unique_ptr;
using std::vector;

/// \todo Check is_clone_ptr(_after_decay) on unique_ptr and vice versa

static_assert( ! is_unique_ptr_after_decay<       vector    < int >   >::value, "" );
static_assert(   is_unique_ptr_after_decay<       unique_ptr< int >   >::value, "" );
static_assert(   is_unique_ptr_after_decay<       unique_ptr< int > & >::value, "" );
static_assert(   is_unique_ptr_after_decay< const unique_ptr< int >   >::value, "" );
static_assert(   is_unique_ptr_after_decay< const unique_ptr< int > & >::value, "" );

static_assert( !             is_unique_ptr<       vector    < int >   >::value, "" );
static_assert(               is_unique_ptr<       unique_ptr< int >   >::value, "" );
static_assert( !             is_unique_ptr<       unique_ptr< int > & >::value, "" );
static_assert( !             is_unique_ptr< const unique_ptr< int >   >::value, "" );
static_assert( !             is_unique_ptr< const unique_ptr< int > & >::value, "" );
