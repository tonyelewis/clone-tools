/// \file
/// \brief The is_clone_ptr test suite

#include "clone/type/is_clone_ptr.hpp"

#include <vector>

using namespace clone_tools;

using std::unique_ptr;
using std::vector;

/// \todo Check is_clone_ptr(_after_decay) on unique_ptr and vice versa

static_assert( ! is_clone_ptr_after_decay<       vector   < int >   >::value, "" );
static_assert(   is_clone_ptr_after_decay<       clone_ptr< int >   >::value, "" );
static_assert(   is_clone_ptr_after_decay<       clone_ptr< int > & >::value, "" );
static_assert(   is_clone_ptr_after_decay< const clone_ptr< int >   >::value, "" );
static_assert(   is_clone_ptr_after_decay< const clone_ptr< int > & >::value, "" );

static_assert( !             is_clone_ptr<       vector   < int >   >::value, "" );
static_assert(               is_clone_ptr<       clone_ptr< int >   >::value, "" );
static_assert( !             is_clone_ptr<       clone_ptr< int > & >::value, "" );
static_assert( !             is_clone_ptr< const clone_ptr< int >   >::value, "" );
static_assert( !             is_clone_ptr< const clone_ptr< int > & >::value, "" );
