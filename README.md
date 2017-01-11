# clone-tools [![Build Status](https://travis-ci.org/tonyelewis/clone-tools.svg?branch=master)](https://travis-ci.org/tonyelewis/clone-tools) [![Documentation Status](https://readthedocs.org/projects/clone-tools/badge/?version=latest)](http://clone-tools.readthedocs.io/en/latest/?badge=latest)

A small, header-only Modern C++ library to make cloning (ie copying polymorphic types) easy

Resources
-------

<table class="front-links-table" style="border-collapse: separate; border-spacing: 20px 0; margin-left: 20px;">
<tbody>
<tr>
<td align="left"><a href="http://clone-tools.readthedocs.io/en/latest/" title="The CATH Tools user documentation at Read the Docs"> Docs       </a> </td>
<td align="left"><a href="https://github.com/tonyelewis/clone-tools"    title="The clone-tools GitHub respository"                > Code       </a> </td>
<td align="left"><a href="https://travis-ci.org/tonyelewis/clone-tools" title="The clone-tools Travis-CI builds"                  > CI/Testing </a> </td>
</tr>
</tbody>
</table>

Purpose
-------

`clone_ptr<>` is just like `std::unique_ptr<>` but with natural copy semantics which are provided by demanding that the wrapped type provides a `clone()` method (ie a pseudo polymorphic-copy-constructor).

Features
--------

 * Zero overhead over `unique_ptr` for equivalent operations (ie modern compilers should easily optimise the extra code away)
 * Header-only
 * Simple to use

How to use
----------

**Step 1** <br>
Add a `clone()` method (ie a pseudo polymorphic-copy-constructor) to your class hierarchy

~~~~~cpp
struct my_base {
	virtual ~my_base() noexcept = default; ///< Make dtor virtual

	virtual std::unique_ptr<my_base> clone() const = 0;
};

struct my_derived : public my_base {
	virtual std::unique_ptr<my_base> clone() const override final {
		return { std::make_unique<my_derived>( *this ) };
	}
};
~~~~~

<sup>(...but prefer using the [NVI](https://en.wikipedia.org/wiki/Non-virtual_interface_pattern) pattern and `clone::make_uptr_clone()`, see [here](http://clone-tools.readthedocs.io/en/latest/reference/utility/) for more).</sup>

**Step 2** <br>
Create and use a `clone_ptr` just like a `unique_ptr`:

~~~~~cpp
using clone::clone_ptr;
using clone::make_clone;

const clone_ptr<my_base> ptr1 = make_clone<my_derived>();

// Or equivalently...
const auto ptr2 = make_clone<my_derived, my_base>();
~~~~~

**Step 3** <br>
Enjoy completely natural copy semantics

~~~~~cpp
const clone_ptr<my_base> ptr_copy{ ptr1 };
~~~~~

...which copies the pointed-to object using its `clone()` method.

To find out more, please [dive into the docs](http://clone-tools.readthedocs.io/en/latest/reference/clone_ptr/).

Feedback
--------

Please tell us your `clone-tools` bugs/suggestions [here](https://github.com/tonyelewis/clone-tools/issues/new).

Supported compilers
-------------------

The library should work on any conformant C++11 / C++14 compiler. At present, the tests are built and run on [Travis-CI](https://travis-ci.org/tonyelewis/clone-tools "The clone-tools Travis-CI builds") under:

 * Linux, Clang 3.5.0 (with libc++)
 * Linux, GCC 4.8.4 (with libstdc++)
 * Mac, Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)

&nbsp;

&nbsp;

&nbsp;

&nbsp;

<small>
**TODOCUMENT** :

 * document all headers
 * all gubbins
 * examples on all pages
 * motivation and simple usage
 * comparison with other clone_ptrs
 * comparison with [`polymorphic_value`](https://github.com/jbcoe/polymorphic_value)
</small>