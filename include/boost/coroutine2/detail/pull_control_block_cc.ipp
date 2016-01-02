
//          Copyright Oliver Kowalke 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES2_DETAIL_PULL_CONTROL_BLOCK_IPP
#define BOOST_COROUTINES2_DETAIL_PULL_CONTROL_BLOCK_IPP

#include <algorithm>
#include <exception>
#include <memory>
#include <tuple>

#include <boost/assert.hpp>
#include <boost/config.hpp>

#include <boost/context/captured_context.hpp>

#include <boost/coroutine2/detail/config.hpp>
#include <boost/coroutine2/detail/forced_unwind.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace coroutines2 {
namespace detail {

// pull_coroutine< T >

template< typename T >
template< typename StackAllocator, typename Fn >
pull_coroutine< T >::control_block::control_block( context::preallocated palloc, StackAllocator salloc,
                                                   Fn && fn) :
    other{ nullptr },
#if defined(BOOST_NO_CXX14_GENERIC_LAMBDAS)
    ctx{ std::allocator_arg, palloc, salloc,
        std::move( 
         std::bind(
             [this]( typename std::decay< Fn >::type & fn_, boost::context::captured_context ctx, void *) mutable {
                // create synthesized push_coroutine< T >
                typename push_coroutine< T >::control_block synthesized_cb{ this, ctx };
                push_coroutine< T > synthesized{ & synthesized_cb };
                other = & synthesized_cb;
                try {
                    auto fn = std::move( fn_);
                    // call coroutine-fn with synthesized push_coroutine as argument
                    fn( synthesized);
                } catch ( boost::context::detail::forced_unwind const&) {
                    throw;
                } catch (...) {
                    // store other exceptions in exception-pointer
                    except = std::current_exception();
                }
                // jump back to ctx
                return std::move( other->ctx);
             },
             std::forward< Fn >( fn),
             std::placeholders::_1,
             std::placeholders::_2))},
#else
    ctx{ std::allocator_arg, palloc, salloc,
         [this,fn_=std::forward< Fn >( fn)]( boost::context::captured_context ctx, void *) mutable {
            // create synthesized push_coroutine< T >
            typename push_coroutine< T >::control_block synthesized_cb{ this, ctx };
            push_coroutine< T > synthesized{ & synthesized_cb };
            other = & synthesized_cb;
            try {
                auto fn = std::move( fn_);
                // call coroutine-fn with synthesized push_coroutine as argument
                fn( synthesized);
            } catch ( boost::context::detail::forced_unwind const&) {
                throw;
            } catch (...) {
                // store other exceptions in exception-pointer
                except = std::current_exception();
            }
            // jump back to ctx
            return std::move( other->ctx);
         }},
#endif
    except{},
    bvalid{ false },
    storage{} {
    // enter coroutine-fn in order to have first value available after ctor (of `*this`) returns
    void * data = nullptr;
    std::tie( ctx, data) = ctx();
    set( static_cast< T * >( data) );
}

template< typename T >
pull_coroutine< T >::control_block::control_block( typename push_coroutine< T >::control_block * cb,
                                                   boost::context::captured_context & ctx_) noexcept :
    other{ cb },
    ctx{ std::move( ctx_) },
    except{},
    bvalid{ false },
    storage{} {
}

template< typename T >
pull_coroutine< T >::control_block::~control_block() {
    // destroy data if set
    if ( bvalid) {
        reinterpret_cast< T * >( std::addressof( storage) )->~T();
    }
}

template< typename T >
void
pull_coroutine< T >::control_block::resume() {
    void * data;
    std::tie( ctx, data) = ctx(); 
    set( static_cast< T * >( data) );
    if ( except) {
        std::rethrow_exception( except);
    }
}

template< typename T >
void
pull_coroutine< T >::control_block::set( T * t) {
    // destroy data if set
    if ( bvalid) {
        reinterpret_cast< T * >( std::addressof( storage) )->~T();
    }
    if ( nullptr != t) {
        ::new ( static_cast< void * >( std::addressof( storage) ) ) T( std::move( * t) );
        bvalid = true;
    } else {
        bvalid = false;
    }
}

template< typename T >
T &
pull_coroutine< T >::control_block::get() noexcept {
    return * reinterpret_cast< T * >( std::addressof( storage) );
}

template< typename T >
bool
pull_coroutine< T >::control_block::valid() const noexcept {
    return nullptr != other && ctx && bvalid;
}


// pull_coroutine< T & >

template< typename T >
template< typename StackAllocator, typename Fn >
pull_coroutine< T & >::control_block::control_block( context::preallocated palloc, StackAllocator salloc,
                                                     Fn && fn) :
    other{ nullptr },
#if defined(BOOST_NO_CXX14_GENERIC_LAMBDAS)
    ctx{ std::allocator_arg, palloc, salloc,
        std::move( 
         std::bind(
             [this]( typename std::decay< Fn >::type & fn_, boost::context::captured_context ctx, void *) mutable {
                // create synthesized push_coroutine< T >
                typename push_coroutine< T & >::control_block synthesized_cb{ this, ctx };
                push_coroutine< T & > synthesized{ & synthesized_cb };
                other = & synthesized_cb;
                try {
                    auto fn = std::move( fn_);
                    // call coroutine-fn with synthesized push_coroutine as argument
                    fn( synthesized);
                } catch ( boost::context::detail::forced_unwind const&) {
                    throw;
                } catch (...) {
                    // store other exceptions in exception-pointer
                    except = std::current_exception();
                }
                // jump back to ctx
                return std::move( other->ctx);
             },
             std::forward< Fn >( fn),
             std::placeholders::_1,
             std::placeholders::_2))},
#else
    ctx{ std::allocator_arg, palloc, salloc,
         [this,fn_=std::forward< Fn >( fn)]( boost::context::captured_context ctx, void *) mutable {
            // create synthesized push_coroutine< T >
            typename push_coroutine< T & >::control_block synthesized_cb{ this, ctx };
            push_coroutine< T & > synthesized{ & synthesized_cb };
            other = & synthesized_cb;
            try {
                auto fn = std::move( fn_);
                // call coroutine-fn with synthesized push_coroutine as argument
                fn( synthesized);
            } catch ( boost::context::detail::forced_unwind const&) {
                throw;
            } catch (...) {
                // store other exceptions in exception-pointer
                except = std::current_exception();
            }
            // jump back to ctx
            return std::move( other->ctx);
         }},
#endif
    except{},
    t{ nullptr } {
    // enter coroutine-fn in order to have first value available after ctor (of `*this`) returns
    void * data = nullptr;
    std::tie( ctx, data) = ctx();
    t = static_cast< T * >( data);
}

template< typename T >
pull_coroutine< T & >::control_block::control_block( typename push_coroutine< T & >::control_block * cb,
                                                     boost::context::captured_context & ctx_) noexcept :
    other{ cb },
    ctx{ std::move( ctx_) },
    except{},
    t{ nullptr } {
}

template< typename T >
void
pull_coroutine< T & >::control_block::resume() {
    void * data;
    std::tie( ctx, data) = ctx(); 
    t = static_cast< T * >( data);
    if ( except) {
        std::rethrow_exception( except);
    }
}

template< typename T >
T &
pull_coroutine< T & >::control_block::get() noexcept {
    return * static_cast< T * >( t);
}

template< typename T >
bool
pull_coroutine< T & >::control_block::valid() const noexcept {
    return nullptr != other && ctx && nullptr != t;
}


// pull_coroutine< void >

template< typename StackAllocator, typename Fn >
pull_coroutine< void >::control_block::control_block( context::preallocated palloc, StackAllocator salloc,
                                                      Fn && fn) :
    other{ nullptr },
#if defined(BOOST_NO_CXX14_GENERIC_LAMBDAS)
    ctx{ std::allocator_arg, palloc, salloc,
        std::move( 
         std::bind(
             [this]( typename std::decay< Fn >::type & fn_, boost::context::captured_context ctx, void *) mutable {
                // create synthesized push_coroutine< T >
                typename push_coroutine< void >::control_block synthesized_cb{ this, ctx };
                push_coroutine< void > synthesized{ & synthesized_cb };
                other = & synthesized_cb;
                try {
                    auto fn = std::move( fn_);
                    // call coroutine-fn with synthesized push_coroutine as argument
                    fn( synthesized);
                } catch ( boost::context::detail::forced_unwind const&) {
                    throw;
                } catch (...) {
                    // store other exceptions in exception-pointer
                    except = std::current_exception();
                }
                // jump back to ctx
                return std::move( other->ctx);
             },
             std::forward< Fn >( fn),
             std::placeholders::_1,
             std::placeholders::_2))},
#else
    ctx{ std::allocator_arg, palloc, salloc,
         [this,fn_=std::forward< Fn >( fn)]( boost::context::captured_context ctx, void *) mutable {
            // create synthesized push_coroutine< T >
            typename push_coroutine< void >::control_block synthesized_cb{ this, ctx };
            push_coroutine< void > synthesized{ & synthesized_cb };
            other = & synthesized_cb;
            try {
                auto fn = std::move( fn_);
                // call coroutine-fn with synthesized push_coroutine as argument
                fn( synthesized);
            } catch ( boost::context::detail::forced_unwind const&) {
                throw;
            } catch (...) {
                // store other exceptions in exception-pointer
                except = std::current_exception();
            }
            // jump back to ctx
            return std::move( other->ctx);
         }},
#endif
    except{} {
    // enter coroutine-fn in order to have first value available after ctor (of `*this`) returns
    void * ignored = nullptr;
    std::tie( ctx, ignored) = ctx();
}

inline
pull_coroutine< void >::control_block::control_block( push_coroutine< void >::control_block * cb,
                                                      boost::context::captured_context & ctx_) noexcept :
    other{ cb },
    ctx{ std::move( ctx_) },
    except{} {
}

inline
void
pull_coroutine< void >::control_block::resume() {
    void * ignored;
    std::tie( ctx, ignored) = ctx(); 
    if ( except) {
        std::rethrow_exception( except);
    }
}

inline
bool
pull_coroutine< void >::control_block::valid() const noexcept {
    return nullptr != other && ctx;
}

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES2_DETAIL_PULL_CONTROL_BLOCK_IPP
