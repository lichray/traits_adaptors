/*-
 * Copyright (c) 2013 Zhihao Yuan.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _STDEX_HOTT_H
#define _STDEX_HOTT_H 1

#include <type_traits>

namespace stdex {

template <template <typename> class F, template <typename> class... Fs>
struct composed
{
	template <typename T>
	using call = F
		<
		    typename composed<Fs...>::template call<T>::type
		>;
};

template <template <typename> class F, template <typename> class G>
struct composed<F, G>
{
	template <typename T>
	using call = F
		<
		    typename G<T>::type
		>;
};

namespace detail {

template <int N, template <typename...> class F, typename... As>
struct currying
{
	template <typename T>
	using call = currying<N - 1, F, As..., T>;

	template <typename... Ts>
	using apply = F<As..., Ts...>;
};

template <template <typename...> class F, typename... As>
struct currying<1, F, As...>
{
	template <typename T>
	using call = F<As..., T>;

	template <typename... Ts>
	using apply = F<As..., Ts...>;
};

template <template <typename> class F, typename T2>
struct bind2
{
	template <typename T1>
	using call = typename F<T1>::template call<T2>;
};

}

template <template <typename...> class F, int N = 2>
using curried = detail::currying<N, F>;

template <template <typename> class F>
struct flipped
{
	template <typename T>
	using call = detail::bind2<F, T>;
};

template <template <typename> class F>
struct negated
{
	template <typename T>
	struct call : std::integral_constant
		<
		    bool,
		    not F<T>::value
		>
	{};
};

template <template <typename> class F, template <typename> class... Fs>
struct both
{
	template <typename T>
	struct call : std::integral_constant
		<
		    bool,
		    F<T>::value and both<Fs...>::template call<T>::value
		>
	{};
};

template <template <typename> class F, template <typename> class G>
struct both<F, G>
{
	template <typename T>
	struct call : std::integral_constant
		<
		    bool,
		    F<T>::value and G<T>::value
		>
	{};
};

template <template <typename> class F, template <typename> class... Fs>
struct either
{
	template <typename T>
	struct call : std::integral_constant
		<
		    bool,
		    F<T>::value or either<Fs...>::template call<T>::value
		>
	{};
};

template <template <typename> class F, template <typename> class G>
struct either<F, G>
{
	template <typename T>
	struct call : std::integral_constant
		<
		    bool,
		    F<T>::value or G<T>::value
		>
	{};
};

template <template <typename> class... Fs>
struct neither
{
	template <typename T>
	using call = typename negated
		<
			either<Fs...>::template call
		>
		::template call<T>;
};

}

#endif
