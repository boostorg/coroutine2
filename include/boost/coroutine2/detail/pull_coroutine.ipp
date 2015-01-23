
//          Copyright Oliver Kowalke 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES2_DETAIL_PULL_COROUTINE_IPP
#define BOOST_COROUTINES2_DETAIL_PULL_COROUTINE_IPP

#include <algorithm>
#include <utility>

#include <boost/assert.hpp>
#include <boost/config.hpp>

#include <boost/context/execution_context.hpp>
#include <boost/context/stack_context.hpp>

#include <boost/coroutine2/detail/config.hpp>
#include <boost/coroutine2/detail/rref.hpp>
#include <boost/coroutine2/fixedsize_stack.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace coroutines2 {
namespace detail {

// pull_coroutine< T >

template< typename T >
pull_coroutine< T >::pull_coroutine( control_block * cb) :
    cb_( cb) {
}

template< typename T >
bool
pull_coroutine< T >::has_result_() const {
    return nullptr != cb_->other->t;
}

template< typename T >
template< typename Fn >
pull_coroutine< T >::pull_coroutine( Fn && fn, bool preserve_fpu) :
    pull_coroutine( fixedsize_stack(), std::forward< Fn >( fn), preserve_fpu) {
}

template< typename T >
template< typename StackAllocator, typename Fn >
pull_coroutine< T >::pull_coroutine( StackAllocator salloc, Fn && fn, bool preserve_fpu) :
    cb_( nullptr) {
    context::stack_context sctx( salloc.allocate() );
    void * sp = static_cast< char * >( sctx.sp) - sizeof( control_block);
    std::size_t size = sctx.size - sizeof( control_block);
    cb_ = new ( sp) control_block( context::preallocated( sp, size, sctx),
                                   salloc, rref< Fn >( std::forward< Fn >( fn) ), preserve_fpu);
}

template< typename T >
pull_coroutine< T >::~pull_coroutine() {
    if ( nullptr != cb_) {
        cb_->~control_block();
    }
}

template< typename T >
pull_coroutine< T >::pull_coroutine( pull_coroutine && other) :
    cb_( nullptr) {
    std::swap( cb_, other.cb_);
}

template< typename T >
pull_coroutine< T > & 
pull_coroutine< T >::operator()() {
    cb_->jump_to();
    return * this;
}

template< typename T >
pull_coroutine< T >::operator bool() const noexcept {
    return nullptr != cb_ && cb_->valid();
}

template< typename T >
bool
pull_coroutine< T >::operator!() const noexcept {
    return nullptr == cb_ || ! cb_->valid();
}

template< typename T >
T
pull_coroutine< T >::get() const noexcept {
    return std::move( * cb_->other->t);
}


// pull_coroutine< T & >

template< typename T >
pull_coroutine< T & >::pull_coroutine( control_block * cb) :
    cb_( cb) {
}

template< typename T >
bool
pull_coroutine< T & >::has_result_() const {
    return nullptr != cb_->other->t;
}

template< typename T >
template< typename Fn >
pull_coroutine< T & >::pull_coroutine( Fn && fn, bool preserve_fpu) :
    pull_coroutine( fixedsize_stack(), std::forward< Fn >( fn), preserve_fpu) {
}

template< typename T >
template< typename StackAllocator, typename Fn >
pull_coroutine< T & >::pull_coroutine( StackAllocator salloc, Fn && fn, bool preserve_fpu) :
    cb_( nullptr) {
    context::stack_context sctx( salloc.allocate() );
    void * sp = static_cast< char * >( sctx.sp) - sizeof( control_block);
    std::size_t size = sctx.size - sizeof( control_block);
    cb_ = new ( sp) control_block( context::preallocated( sp, size, sctx),
                                   salloc, rref< Fn >( std::forward< Fn >( fn) ), preserve_fpu);
}

template< typename T >
pull_coroutine< T & >::~pull_coroutine() {
    if ( nullptr != cb_) {
        cb_->~control_block();
    }
}

template< typename T >
pull_coroutine< T & >::pull_coroutine( pull_coroutine && other) :
    cb_( nullptr) {
    std::swap( cb_, other.cb_);
}

template< typename T >
pull_coroutine< T & > &
pull_coroutine< T & >::operator()() {
    cb_->jump_to();
    return * this;
}

template< typename T >
pull_coroutine< T & >::operator bool() const noexcept {
    return nullptr != cb_ && cb_->valid();
}

template< typename T >
bool
pull_coroutine< T & >::operator!() const noexcept {
    return nullptr == cb_ || ! cb_->valid();
}

template< typename T >
T &
pull_coroutine< T & >::get() const noexcept {
    return * cb_->other->t;
}


// pull_coroutine< void >

inline
pull_coroutine< void >::pull_coroutine( control_block * cb) :
    cb_( cb) {
}

template< typename Fn >
pull_coroutine< void >::pull_coroutine( Fn && fn, bool preserve_fpu) :
    pull_coroutine( fixedsize_stack(), std::forward< Fn >( fn), preserve_fpu) {
}

template< typename StackAllocator, typename Fn >
pull_coroutine< void >::pull_coroutine( StackAllocator salloc, Fn && fn, bool preserve_fpu) :
    cb_( nullptr) {
    context::stack_context sctx( salloc.allocate() );
    void * sp = static_cast< char * >( sctx.sp) - sizeof( control_block);
    std::size_t size = sctx.size - sizeof( control_block);
    cb_ = new ( sp) control_block( context::preallocated( sp, size, sctx),
                                   salloc, rref< Fn >( std::forward< Fn >( fn) ), preserve_fpu);
}

inline
pull_coroutine< void >::~pull_coroutine() {
    if ( nullptr != cb_) {
        cb_->~control_block();
    }
}

inline
pull_coroutine< void >::pull_coroutine( pull_coroutine && other) :
    cb_( nullptr) {
    std::swap( cb_, other.cb_);
}

inline
pull_coroutine< void > &
pull_coroutine< void >::operator()() {
    cb_->jump_to();
    return * this;
}

inline
pull_coroutine< void >::operator bool() const noexcept {
    return nullptr != cb_ && cb_->valid();
}

inline
bool
pull_coroutine< void >::operator!() const noexcept {
    return nullptr == cb_ || ! cb_->valid();
}

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES2_DETAIL_PULL_COROUTINE_IPP
