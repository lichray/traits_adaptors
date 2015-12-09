#include "traits_adaptors.h"
#include <string>
#include <functional>

using namespace stdex;

template <typename T>
using is_transparent_t = typename T::is_transparent;

template <typename T>
using diff_t = typename T::difference_type;

int main()
{
	static_assert(
	    std::is_same
	    <
	        composed
		<
		    std::remove_cv,
		    std::remove_pointer,
		    std::remove_reference
		>
		::call<char const*&>::type,
		char
	    >
	    ::value, "composed type");

	static_assert(
	    std::is_same
	    <
	        planned
		<
		    std::remove_reference,
		    std::remove_pointer,
		    std::remove_cv
		>
		::call<char const*&>::type,
		char
	    >
	    ::value, "planned operations");

	static_assert(
	    composed
	    <
	        std::is_signed,
		std::make_signed,
		std::remove_reference
	    >
	    ::call<unsigned&>(), "composed integral_constant");

	static_assert(
	    neither
	    <
	        std::is_array,
		std::is_unsigned,
		std::is_pointer
	    >
	    ::call<int>(), "neither, either, and negatively");

	static_assert(
	    both
	    <
	        std::is_signed,
		std::is_scalar,
		std::is_const
	    >
	    ::call<float const>(), "both");

	static_assert(
	    curried<std::is_convertible>
	    ::call<int>::call<long>(), "curried");

	static_assert(
	    composed
	    <
	        curried<std::is_constructible, 3>
		::call<std::string>
		::call,      // this call is composed
		std::decay   // by std::decay
	    >
	    ::call<char[1]>  // and continued here
	    ::call<size_t>(), "sized string ctor");

	static_assert(
	    flipped<curried<std::is_convertible>>
	    ::apply<std::string, char*>(), "flipped");

	static_assert(
	    flipped<curried<std::is_constructible, 4>, 3>
	    ::apply<size_t, char*, std::string, std::allocator<char>>(),
	    "swap 1st parameter with 3rd");

	static_assert(
	    flipped<curried<std::is_constructible, 4>, 4>
	    ::apply<std::allocator<char>, char*, size_t, std::string>(),
	    "swap 1st parameter with 4th");

	static_assert(
	    std::is_same
	    <
	        curried<std::common_type>
		::call<char>
		::apply<short, bool>
		::type,
		int
	    >
	    ::value, "apply (auto uncurry)");

	static_assert(
	    std::is_same
	    <
	        composed
		<
		    flipped<curried<std::common_type, 3>>::call,
		    std::remove_pointer
		>
		::apply<short*, char>::type,
		int
	    >
	    ::value, "apply forwarding");

	static_assert(
	    all_type<std::is_arithmetic>() and
	    all_type<std::is_arithmetic, int, double, unsigned>(), "all");

	static_assert(
	    no_type<std::is_array>() and
	    no_type<std::is_array, int, double, unsigned>(), "any, none");

	static_assert(
	    std::is_same<If<std::true_type>, void>() and
	    std::is_same
	    <
	        if_else<std::true_type, identity_of<char>, identity_of<int>>
		::type,
		char
	    >
	    ::value and
	    std::is_same
	    <
	        if_else<std::false_type, identity_of<char>, identity_of<int>>
		::type,
		int
	    >
	    ::value, "lazy_enable_if and lazy_conditional");

	using decay =
		planned
		<
		    std::remove_reference,
		    conditionally
		    <
		        std::is_array,
			planned
			<
			    std::remove_extent,
			    std::add_pointer
			>
			::call,
			conditionally
			<
			    std::is_function,
			    std::add_pointer,
			    std::remove_cv
			>
			::call
		    >
		    ::call
		>;

	using decay_equiv =
		planned
		<
		    decay::call,
		    curried<std::is_same>::call
		>;

	static_assert(decay_equiv::apply<const int&, int>(), "lr");
	static_assert(decay_equiv::apply<int[2], int*>(), "array-p");
	static_assert(decay_equiv::apply<int(int), int(*)(int)>(), "fn-p");

	using is_transparent = detector_of<is_transparent_t>;

	static_assert(not is_transparent::call<std::less<int>>(), "old");
	static_assert(is_transparent::call<std::less<>>(), "new");

	using difference_type = detector_of<diff_t, ptrdiff_t>;

	static_assert(
	    std::is_same
	    <
		difference_type::call<int*>::type,
		ptrdiff_t
	    >
	    ::value, "raw");
}
