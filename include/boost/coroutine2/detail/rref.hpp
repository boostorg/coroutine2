
//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES2_DETAIL_RREF_H
#define BOOST_COROUTINES2_DETAIL_RREF_H

#include <boost/config.hpp>

#include <boost/coroutine2/detail/config.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace coroutines2 {
namespace detail {

// helper class for capture move-only objects
// generalized lambda captures are supported by C++14
template< typename Fn >
class rref {
public:
    rref( Fn && fn) :
        fn_( std::forward< Fn >( fn) ) {
    }

    rref( rref & other) :
        fn_( std::forward< Fn >( other.fn_) ) {
    }

    rref( rref && other) :
        fn_( std::forward< Fn >( other.fn_) ) {
    }

    rref( rref const& other) = delete;
    rref & operator=( rref const& other) = delete;

    template< typename S >
    void operator()( S & s) {
        return fn_( s);
    }

private:
    Fn  fn_;
};

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES2_DETAIL_RREF_H
