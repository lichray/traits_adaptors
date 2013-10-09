#include "higher_order_traits.h"

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
}