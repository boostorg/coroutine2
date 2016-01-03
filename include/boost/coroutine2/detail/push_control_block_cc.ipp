
//          Copyright Oliver Kowalke 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES2_DETAIL_PUSH_CONTROL_BLOCK_IPP
#define BOOST_COROUTINES2_DETAIL_PUSH_CONTROL_BLOCK_IPP

#include <algorithm>
#include <exception>
#include <memory>

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

// push_coroutine< T >

template< typename T >
template< typename StackAllocator, typename Fn >
push_coroutine< T >::control_block::control_block( context::preallocated palloc, StackAllocator salloc,
                                                   Fn && fn) :
    other{ nullptr },
#if defined(BOOST_NO_CXX14_GENERIC_LAMBDAS)
    ctx{ std::allocator_arg, palloc, salloc,
        std::move( 
         std::bind(
             [this]( typename std::decay< Fn >::type & fn_, boost::context::captured_context ctx, void * data) mutable {
                // create synthesized pull_coroutine< T >
                typename pull_coroutine< T >::control_block synthesized_cb{ this, ctx };
                pull_coroutine< T > synthesized{ & synthesized_cb };
                other = & synthesized_cb;
                // set transferred value
                synthesized_cb.set( static_cast< T * >( data) );
                try {
                    auto fn = std::move( fn_);
                    // call coroutine-fn with synthesized pull_coroutine as argument
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
         [this,fn_=std::forward< Fn >( fn)]( boost::context::captured_context ctx, void * data) mutable {
            // create synthesized pull_coroutine< T >
            typename pull_coroutine< T >::control_block synthesized_cb{ this, ctx };
            pull_coroutine< T > synthesized{ & synthesized_cb };
            other = & synthesized_cb;
            // set transferred value
            synthesized_cb.set( static_cast< T * >( data) );
            try {
                auto fn = std::move( fn_);
                // call coroutine-fn with synthesized pull_coroutine as argument
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
}

template< typename T >
push_coroutine< T >::control_block::control_block( typename pull_coroutine< T >::control_block * cb,
                                                   boost::context::captured_context & ctx_) noexcept :
    other{ cb },
    ctx{ std::move( ctx_) },
    except{} {
}

template< typename T >
void
push_coroutine< T >::control_block::resume( T const& data) {
    // pass an pointer to other context
    auto result = ctx( const_cast< T * >( & data) );
    ctx = std::move( std::get< 0 >( result) );
    if ( except) {
        std::rethrow_exception( except);
    }
}

template< typename T >
void
push_coroutine< T >::control_block::resume( T && data) {
    // pass an pointer to other context
    auto result = ctx( std::addressof( data) );
    ctx = std::move( std::get< 0 >( result) );
    if ( except) {
        std::rethrow_exception( except);
    }
}

template< typename T >
bool
push_coroutine< T >::control_block::valid() const noexcept {
    return ctx ? true : false;
}


// push_coroutine< T & >

template< typename T >
template< typename StackAllocator, typename Fn >
push_coroutine< T & >::control_block::control_block( context::preallocated palloc, StackAllocator salloc,
                                                     Fn && fn) :
    other{ nullptr },
#if defined(BOOST_NO_CXX14_GENERIC_LAMBDAS)
    ctx{ std::allocator_arg, palloc, salloc,
        std::move( 
         std::bind(
             [this]( typename std::decay< Fn >::type & fn_, boost::context::captured_context ctx, void * data) mutable {
                // create synthesized pull_coroutine< T >
                typename pull_coroutine< T & >::control_block synthesized_cb{ this, ctx };
                pull_coroutine< T & > synthesized{ & synthesized_cb };
                other = & synthesized_cb;
                // set transferred value
                synthesized_cb.t = static_cast< T * >( data);
                try {
                    auto fn = std::move( fn_);
                    // call coroutine-fn with synthesized pull_coroutine as argument
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
         [this,fn_=std::forward< Fn >( fn)]( boost::context::captured_context ctx, void * data) mutable {
            // create synthesized pull_coroutine< T >
            typename pull_coroutine< T & >::control_block synthesized_cb{ this, ctx };
            pull_coroutine< T & > synthesized{ & synthesized_cb };
            other = & synthesized_cb;
            // set transferred value
            synthesized_cb.t = static_cast< T * >( data);
            try {
                auto fn = std::move( fn_);
                // call coroutine-fn with synthesized pull_coroutine as argument
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
}

template< typename T >
push_coroutine< T & >::control_block::control_block( typename pull_coroutine< T & >::control_block * cb,
                                                     boost::context::captured_context & ctx_) noexcept :
    other{ cb },
    ctx{ std::move( ctx_) },
    except{} {
}

template< typename T >
void
push_coroutine< T & >::control_block::resume( T & t) {
    // pass an pointer to other context
    auto result = ctx( const_cast< typename std::remove_const< T >::type * >( std::addressof( t) ) );
    ctx = std::move( std::get< 0 >( result) );
    if ( except) {
        std::rethrow_exception( except);
    }
}

template< typename T >
bool
push_coroutine< T & >::control_block::valid() const noexcept {
    return ctx ? true : false;
}


// push_coroutine< void >

template< typename StackAllocator, typename Fn >
push_coroutine< void >::control_block::control_block( context::preallocated palloc, StackAllocator salloc, Fn && fn) :
    other{ nullptr },
#if defined(BOOST_NO_CXX14_GENERIC_LAMBDAS)
    ctx{ std::allocator_arg, palloc, salloc,
        std::move( 
         std::bind(
             [this]( typename std::decay< Fn >::type & fn_, boost::context::captured_context ctx, void *) mutable {
                // create synthesized pull_coroutine< T >
                typename pull_coroutine< void >::control_block synthesized_cb{ this, ctx };
                pull_coroutine< void > synthesized{ & synthesized_cb };
                other = & synthesized_cb;
                try {
                    auto fn = std::move( fn_);
                    // call coroutine-fn with synthesized pull_coroutine as argument
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
            // create synthesized pull_coroutine< T >
            typename pull_coroutine< void >::control_block synthesized_cb{ this, ctx};
            pull_coroutine< void > synthesized{ & synthesized_cb };
            other = & synthesized_cb;
            try {
                auto fn = std::move( fn_);
                // call coroutine-fn with synthesized pull_coroutine as argument
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
}

inline
push_coroutine< void >::control_block::control_block( pull_coroutine< void >::control_block * cb,
                                                      boost::context::captured_context & ctx_) noexcept :
    other{ cb },
    ctx{ std::move( ctx_) },
    except{} {
}

inline
void
push_coroutine< void >::control_block::resume() {
    auto result = ctx();
    ctx = std::move( std::get< 0 >( result) );
    if ( except) {
        std::rethrow_exception( except);
    }
}

inline
bool
push_coroutine< void >::control_block::valid() const noexcept {
    return ctx ? true : false;
}

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES2_DETAIL_PUSH_CONTROL_BLOCK_IPP
