
//          Copyright Oliver Kowalke 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES2_DETAIL_CONFIG_H
#define BOOST_COROUTINES2_DETAIL_CONFIG_H

#include <boost/config.hpp>
#include <boost/context/detail/config.hpp>
#include <boost/detail/workaround.hpp>

#ifdef BOOST_COROUTINES2_DECL
# undef BOOST_COROUTINES2_DECL
#endif

#if (defined(BOOST_ALL_DYN_LINK) || defined(BOOST_COROUTINES2_DYN_LINK) ) && ! defined(BOOST_COROUTINES2_STATIC_LINK)
# if defined(BOOST_COROUTINES2_SOURCE)
#  define BOOST_COROUTINES2_DECL BOOST_SYMBOL_EXPORT
# else
#  define BOOST_COROUTINES2_DECL BOOST_SYMBOL_IMPORT
# endif
#endif

#if ! defined(BOOST_COROUTINES2_DECL)
# define BOOST_COROUTINES2_DECL
#endif

// __has_include is currently supported by GCC and Clang. However GCC 4.9 may have issues and
// returns 1 for 'defined( __has_include )', while '__has_include' is actually not supported:
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=63662
#if defined( __has_include ) && (!defined( BOOST_GCC ) || (__GNUC__ + 0) >= 5)
# if __has_include(<cxxabi.h>)
#  define BOOST_COROUTINES2_HAS_CXXABI_H
# endif
#elif defined( __GLIBCXX__ ) || defined( __GLIBCPP__ )
# define BOOST_COROUTINES2_HAS_CXXABI_H
#endif

#if defined( BOOST_COROUTINES2_HAS_CXXABI_H )
# include <cxxabi.h>
#endif

#endif // BOOST_COROUTINES2_DETAIL_CONFIG_H
