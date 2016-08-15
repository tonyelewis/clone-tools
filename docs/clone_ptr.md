[//]: # (TODO: Can these notes be put into a header?)
[//]: # (<sup>**Note**: this library requires C++11 or higher</sup> <br />)
[//]: # (<sup>**Note**: this documentation draws content and style from the excellent [cppreference.com](http://cppreference.com); please use and support that site. </sup>)

# `clone_tools::clone_ptr<T>`

<sup>Defined in header &lt;clone/clone_ptr.hpp&gt;</sup><br />
Like [std::unique_ptr](http://cppreference.com/SOME_GUFF_HERE/unique_ptr) but requires the managed type to support cloning and then uses that to support copy-construction and copy-assignment by cloning the managed object.

Doesn't currently support:

 * deleter
 * array version
 

## Member types

| Member type | Definition |
|:--- |:--- |
| [`pointer type` &nbsp; | `std::remove_reference<Deleter>::type::pointer` if that type exists, otherwise T*. Must satisfy NullablePointer.</sup> |
| [`element_type` | T, the type of the object managed by this unique_ptr</sup> |
<br />


## Member functions

|     |     |     |
|:--- |:--- |:--- |
| [**` (constructor)                   `** ](http://bbc.com) | constructs a new clone_tools::clone_ptr                                <br /> <sup> (public member function) </sup> |
| [**` (destructor)                    `** ](http://bbc.com) | destructs the managed object                                           <br /> <sup> (public member function) </sup> |
| [**` operator=                       `** ](http://bbc.com) | assigns the clone_tools::clone_ptr                                     <br /> <sup> (public member function) </sup> |


### Modifiers

|     |     |     |
| --- | --- | --- |
| [**` release                         `** ](http://bbc.com) &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; | returns a pointer to the managed object and releases the ownership     <br /> <sup> (public member function) </sup> |
| [**` reset                           `** ](http://bbc.com) | replaces the managed object                                            <br /> <sup> (public member function) </sup> |
| [**` swap                            `** ](http://bbc.com) | swaps the managed objects                                              <br /> <sup> (public member function) </sup> |


### Observers

|     |     |     |
| --- | --- | --- |
| [**` get                             `** ](http://bbc.com) | returns a pointer to the managed object                                <br /> <sup> (public member function) </sup> |
[//]: # ( | [**` get_deleter           `** ](http://bbc.com) | returns the deleter that is used for destruction of the managed object <br /> <sup> (public member function) </sup> | )
| [**` operator bool                   `** ](http://bbc.com) | checks if there is associated managed object                           <br /> <sup> (public member function) </sup> |
| [**` operator*`**<br />**`operator-> `** ](http://bbc.com) | dereferences pointer to the managed object                             <br /> <sup> (public member function) </sup> |
<br />


## Non-member functions

**TODO**: make clear what (if anything is the point of each of these functions). <br />
<sub>Namespace `clone_tools`</sub>

|     |     |     |
| --- | --- | --- |
| [**`make_unique                `**](http://bbc.com) | constructs a new std::unique_ptr <br /> <sup>(function template)</sup> | <sub>*`until C++14`*</sub> |
| [**`make_clone                 `**](http://bbc.com) | constructs a new std::clone_ptr  <br /> <sup>(function template)</sup> | |
| [**`make_clone_of_uptr         `**](http://bbc.com) | creates a clone_ptr from a std::unique_ptr (copy/move) <br /> <sup>(function template)</sup> | |
| [**`make_const_clone_of_uptr   `**](http://bbc.com) | creates a const clone_ptr from a std::unique_ptr (copy/move) <br /> <sup>(function template)</sup> | |
| [**`clone_and_make_clone       `**](http://bbc.com) | clones an object into a clone_ptr<br /> <sup>(function template)</sup> | |
| [**`clone_and_make_const_clone `**](http://bbc.com) | clones an object into a const clone_ptr<br /> <sup>(function template)</sup> | |


## Type trait gubbins

<sub>Namespace `clone_tools`</sub>

|     |     |     |
| --- | --- | --- |
| [**`is_clone_v<>`**](http://bbc.com) | whether the type is a clone gubbins thing<br /> <sup>(variable template)</sup> | <sub>*`since C++14`*</sub> |
| [**`is_clone_v<>`**](http://bbc.com) | whether the type is a clone gubbins thing<br /> <sup>(variable template)</sup> | <sub>*`since C++14`*</sub> |



## Example

~~~~~cpp
const auto ptr_a = make_clone<concrete_type_a, base_type>( "hello" );
const auto ptr_b{ ptr_a };
~~~~~