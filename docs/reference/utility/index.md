# `Utility` 

<p class="namespace-subhead"><sup class="namespace-subhead">Defined in header &lt;clone/utility.hpp&gt;</sup></p>


NVI (reference???)

~~~~~cpp

class my_base {
private:
	virtual std::unique_ptr<my_base> do_clone() const = 0;

public:
	virtual ~my_base() noexcept = default; ///< Make dtor virtual

	/// \brief TODOCUMENT
	std::unique_ptr<my_base> clone() const {
		return check_uptr_clone_against_this( do_clone(), *this );
	}
};
~~~~~

~~~~~cpp
class my_derived final : public my_base {
private:
	/// \brief TODOCUMENT
	virtual std::unique_ptr<my_base> do_clone() const override final {
		return { make_uptr_clone( *this ) };
	}
};
~~~~~

Recommend the following in the base class

~~~~~cpp
my_base(const my_base &)                 = default; ///< Specify copy-ctor   required
my_base(my_base &&) noexcept             = default; ///< Specify copy-assign required
my_base & operator=(const my_base &)     = default; ///< Specify move-ctor   required
my_base & operator=(my_base &&) noexcept = default; ///< Specify move-assign required
~~~~~


## Functions

<p class="namespace-subhead"><sup class="namespace-subhead">Within namespace <code>clone_tools</code></sup></p>

| |
|:--- |:--- |
| [**` check_uptr_clone_against_this `** ](http://bbc.com) | Check dynamic type returned by virtual `do_clone()` in debug mode   <br /> <sup> (function template) </sup> |
| [**` make_uptr_clone               `** ](http://bbc.com) | Make a [**`std::unique_ptr`**](http://en.cppreference.com/w/cpp/memory/unique_ptr) clone of the specified object using its copy-ctor <br /> <sup> (function template) </sup> |
