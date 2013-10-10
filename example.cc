#include "traits_adaptors.h"
#include <string>

using namespace stdex;

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
	    composed
	    <
	        std::is_signed,
		std::make_signed,
		std::remove_reference
	    >
	    ::call<unsigned&>::value, "composed integral_constant");

	static_assert(
	    neither
	    <
	        std::is_array,
		std::is_unsigned,
		std::is_pointer
	    >
	    ::call<int>::value, "neither, either, and negated");

	static_assert(
	    both
	    <
	        std::is_signed,
		std::is_scalar,
		std::is_const
	    >
	    ::call<float const>::value, "both");

	static_assert(
	    curried<std::is_convertible>
	    ::call<int>::call<long>::value, "curried");

	static_assert(
	    composed
	    <
	        curried<std::is_constructible, 3>
		::call<std::string>
		::call,      // this call is composed
		std::decay   // by std::decay
	    >
	    ::call<char[1]>  // and continued here
	    ::call<size_t>::value, "sized string ctor");

	static_assert(
	    flipped<curried<std::is_convertible>::call>
	    ::apply<std::string, char*>::value, "flipped");

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
		    flipped<curried<std::common_type, 3>::call>::call,
		    std::remove_pointer
		>
		::apply<short*, char>::type,
		int
	    >
	    ::value, "apply forwarding");
}
