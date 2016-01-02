
//          Copyright Oliver Kowalke 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES2_DETAIL_COROUTINE_HPP
#define BOOST_COROUTINES2_DETAIL_COROUTINE_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace coroutines2 {
namespace detail {

template< typename T >
class pull_coroutine;

template< typename T >
class push_coroutine;

}}}

#include <boost/coroutine2/detail/pull_coroutine.hpp>
#include <boost/coroutine2/detail/push_coroutine.hpp>

#if ! defined(BOOST_USE_SEGMENTED_STACKS)
#include <boost/coroutine2/detail/pull_control_block_cc.hpp>
#include <boost/coroutine2/detail/push_control_block_cc.hpp>
#else
#include <boost/coroutine2/detail/pull_control_block_ec.hpp>
#include <boost/coroutine2/detail/push_control_block_ec.hpp>
#endif

#include <boost/coroutine2/detail/pull_coroutine.ipp>
#include <boost/coroutine2/detail/push_coroutine.ipp>

#if ! defined(BOOST_USE_SEGMENTED_STACKS)
#include <boost/coroutine2/detail/pull_control_block_cc.ipp>
#include <boost/coroutine2/detail/push_control_block_cc.ipp>
#else
#include <boost/coroutine2/detail/pull_control_block_ec.ipp>
#include <boost/coroutine2/detail/push_control_block_ec.ipp>
#endif

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES2_DETAIL_COROUTINE_HPP
