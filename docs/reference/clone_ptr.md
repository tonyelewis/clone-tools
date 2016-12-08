[//]: # (TODO: Can these notes be put into a header?)
[//]: # (<sup>**Note**: this library requires C++11 or higher</sup> <br />)

# `clone_tools::clone_ptr<T>` 

<p class="namespace-subhead"><sup class="namespace-subhead">Defined in header &lt;clone/clone_ptr.hpp&gt;</sup></p>


Like [**`std::unique_ptr`**](http://en.cppreference.com/w/cpp/memory/unique_ptr) but requires the managed type to support cloning and then uses that to support copy-construction and copy-assignment by cloning the managed object.

Doesn't currently support:

 * deleter
 * array version
 

## Member types

| |
|:--- |:--- |
| `pointer type` &nbsp; | `std::remove_reference<Deleter>::type::pointer` if that type exists, otherwise T*. Must satisfy NullablePointer.</sup> |
| `element_type` | T, the type of the object managed by this unique_ptr</sup> |

## Member functions

| |
|:--- |:--- |
| [**` (constructor)                   `** ](http://bbc.com) | constructs a new clone_tools::clone_ptr                                <br /> <sup> (public member function) </sup> |
| [**` (destructor)                    `** ](http://bbc.com) | destructs the managed object                                           <br /> <sup> (public member function) </sup> |
| [**` operator=                       `** ](http://bbc.com) | assigns the clone_tools::clone_ptr                                     <br /> <sup> (public member function) </sup> |
| <h3> Modifiers </h3> |
| [**` release                         `** ](http://bbc.com) &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; | returns a pointer to the managed object and releases the ownership     <br /> <sup> (public member function) </sup> |
| [**` reset                           `** ](http://bbc.com) | replaces the managed object                                            <br /> <sup> (public member function) </sup> |
| [**` swap                            `** ](http://bbc.com) | swaps the managed objects                                              <br /> <sup> (public member function) </sup> |
| <h3> Observers </h3> |
| [**` get                             `** ](http://bbc.com) | returns a pointer to the managed object                                <br /> <sup> (public member function) </sup> |
[//]: # ( | [**` get_deleter           `** ](http://bbc.com) | returns the deleter that is used for destruction of the managed object <br /> <sup> (public member function) </sup> | )
| [**` operator bool                   `** ](http://bbc.com) | checks if there is associated managed object                           <br /> <sup> (public member function) </sup> |
| [**` operator*`**<br />**`operator-> `** ](http://bbc.com) | dereferences pointer to the managed object                             <br /> <sup> (public member function) </sup> |

## Non-member functions

<p class="namespace-subhead"><sup class="namespace-subhead">Within namespace <code>clone_tools</code></sup></p>

**TODO**: make clear what (if anything) is the point of each of these functions. <br />

| |
| --- | --- | --- |
| [**`make_unique                `**](http://bbc.com) | constructs a new std::unique_ptr <br /> <sup>(function template)</sup> | <sup>*`(until C++14)`*</sup> |
| [**`make_clone                 `**](http://bbc.com) | constructs a new std::clone_ptr  <br /> <sup>(function template)</sup> | |
| [**`make_clone_of_uptr         `**](http://bbc.com) | creates a clone_ptr from a std::unique_ptr (copy/move) <br /> <sup>(function template)</sup> | |
| [**`make_const_clone_of_uptr   `**](http://bbc.com) | creates a const clone_ptr from a std::unique_ptr (copy/move) <br /> <sup>(function template)</sup> | |
| [**`clone_and_make_clone       `**](http://bbc.com) | clones an object into a clone_ptr<br /> <sup>(function template)</sup> | |
| [**`clone_and_make_const_clone `**](http://bbc.com) | clones an object into a const clone_ptr<br /> <sup>(function template)</sup> | |


## Type trait gubbins

<p class="namespace-subhead"><sup class="namespace-subhead">Within namespace <code>clone_tools</code></sup></p>

|     |     |     |
| --- | --- | --- |
| <h3> Observers </h3> |
| [**`is_clone_v<>`**](http://bbc.com) | whether the type is a clone gubbins thing<br /> <sup>(variable template)</sup> | <sup>*`since C++14`*</sup> |
| [**`is_clone_v<>`**](http://bbc.com) | whether the type is a clone gubbins thing<br /> <sup>(variable template)</sup> | <sup>*`since C++14`*</sup> |
| <h3> Observers </h3> |
| [**`is_unique_ptr_v<>`**](http://bbc.com) | whether the type is a clone gubbins thing<br /> <sup>(variable template)</sup> | <sup>*`since C++14`*</sup> |
| [**`is_unique_ptr_after_decay_v<>`**](http://bbc.com) | whether the type is a clone gubbins thing<br /> <sup>(variable template)</sup> | <sup>*`since C++14`*</sup> |

## Example

~~~~~cpp
#include <clone/clone_ptr.hpp>

const auto ptr_a = make_clone<concrete_type_a, base_type>( "hello" );
const auto ptr_b{ ptr_a };
~~~~~
