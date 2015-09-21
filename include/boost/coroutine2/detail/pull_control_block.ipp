
//          Copyright Oliver Kowalke 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES2_DETAIL_PULL_CONTROL_BLOCK_IPP
#define BOOST_COROUTINES2_DETAIL_PULL_CONTROL_BLOCK_IPP

#include <exception>

#include <boost/assert.hpp>
#include <boost/config.hpp>

#include <boost/context/execution_context.hpp>

#include <boost/coroutine2/detail/config.hpp>
#include <boost/coroutine2/detail/forced_unwind.hpp>
#include <boost/coroutine2/detail/state.hpp>

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
                                                   Fn && fn_, bool preserve_fpu_) :
    other( nullptr),
    ctx( palloc, salloc,
            [=,fn=std::forward< Fn >( fn_),ctx=boost::context::execution_context::current()] () mutable -> void {
               // create synthesized push_coroutine< T >
               typename push_coroutine< T >::control_block synthesized_cb( this, ctx);
               push_coroutine< T > synthesized( & synthesized_cb);
               other = & synthesized_cb;
               try {
                   // call coroutine-fn with synthesized push_coroutine as argument
                   fn( synthesized);
               } catch ( forced_unwind const&) {
                   // do nothing for unwinding exception
               } catch (...) {
                   // store other exceptions in exception-pointer
                   except = std::current_exception();
               }
               // set termination flags
               state |= static_cast< int >( state_t::complete);
               // jump back to ctx
               other->ctx( preserve_fpu);
               BOOST_ASSERT_MSG( false, "pull_coroutine is complete");
            }),
    preserve_fpu( preserve_fpu_),
    state( static_cast< int >( state_t::unwind) ),
    except() {
    // enter coroutine-fn in order to have first value available after ctor returns
    ctx( preserve_fpu);
}

template< typename T >
pull_coroutine< T >::control_block::control_block( typename push_coroutine< T >::control_block * cb,
                                                   boost::context::execution_context const& ctx_) :
    other( cb),
    ctx( ctx_),
    preserve_fpu( other->preserve_fpu),
    state( 0),
    except() {
}

template< typename T >
pull_coroutine< T >::control_block::~control_block() {
    if ( 0 == ( state & static_cast< int >( state_t::complete ) ) &&
         0 != ( state & static_cast< int >( state_t::unwind) ) ) {
        // set early-exit flag
        state |= static_cast< int >( state_t::early_exit);
        ctx( preserve_fpu);
    }
}

template< typename T >
void
pull_coroutine< T >::control_block::resume() {
    other->ctx = boost::context::execution_context::current();
    ctx( preserve_fpu);
    if ( except) {
        std::rethrow_exception( except);
    }
    // test early-exit-flag
    if ( 0 != ( ( other->state) & static_cast< int >( state_t::early_exit) ) ) {
        throw forced_unwind();
    }
}

template< typename T >
bool
pull_coroutine< T >::control_block::valid() const noexcept {
    return nullptr != other && nullptr != other->t && 0 == ( state & static_cast< int >( state_t::complete) );
}


// pull_coroutine< T & >

template< typename T >
template< typename StackAllocator, typename Fn >
pull_coroutine< T & >::control_block::control_block( context::preallocated palloc, StackAllocator salloc,
                                                     Fn && fn_, bool preserve_fpu_) :
    other( nullptr),
    ctx( palloc, salloc,
            [=,fn=std::forward< Fn >( fn_),ctx=boost::context::execution_context::current()] () mutable -> void {
               // create synthesized push_coroutine< T >
               typename push_coroutine< T & >::control_block synthesized_cb( this, ctx);
               push_coroutine< T & > synthesized( & synthesized_cb);
               other = & synthesized_cb;
               try {
                   // call coroutine-fn with synthesized push_coroutine as argument
                   fn( synthesized);
               } catch ( forced_unwind const&) {
                   // do nothing for unwinding exception
               } catch (...) {
                   // store other exceptions in exception-pointer
                   except = std::current_exception();
               }
               // set termination flags
               state |= static_cast< int >( state_t::complete);
               // jump back to ctx
               other->ctx( preserve_fpu);
               BOOST_ASSERT_MSG( false, "pull_coroutine is complete");
            }),
    preserve_fpu( preserve_fpu_),
    state( static_cast< int >( state_t::unwind) ),
    except() {
    // enter coroutine-fn in order to have first value available after ctor returns
    ctx( preserve_fpu);
}

template< typename T >
pull_coroutine< T & >::control_block::control_block( typename push_coroutine< T & >::control_block * cb,
                                                     boost::context::execution_context const& ctx_) :
    other( cb),
    ctx( ctx_),
    preserve_fpu( other->preserve_fpu),
    state( 0),
    except() {
}

template< typename T >
pull_coroutine< T & >::control_block::~control_block() {
    if ( 0 == ( state & static_cast< int >( state_t::complete ) ) &&
         0 != ( state & static_cast< int >( state_t::unwind) ) ) {
        // set early-exit flag
        state |= static_cast< int >( state_t::early_exit);
        ctx( preserve_fpu);
    }
}

template< typename T >
void
pull_coroutine< T & >::control_block::resume() {
    other->ctx = boost::context::execution_context::current();
    ctx( preserve_fpu);
    if ( except) {
        std::rethrow_exception( except);
    }
    // test early-exit-flag
    if ( 0 != ( ( other->state) & static_cast< int >( state_t::early_exit) ) ) {
        throw forced_unwind();
    }
}

template< typename T >
bool
pull_coroutine< T & >::control_block::valid() const noexcept {
    return nullptr != other && nullptr != other->t && 0 == ( state & static_cast< int >( state_t::complete) );
}


// pull_coroutine< void >

template< typename StackAllocator, typename Fn >
pull_coroutine< void >::control_block::control_block( context::preallocated palloc, StackAllocator salloc,
                                                      Fn && fn_, bool preserve_fpu_) :
    other( nullptr),
    ctx( palloc, salloc,
            [=,fn=std::forward< Fn >( fn_),ctx=boost::context::execution_context::current()] () mutable -> void {
               // create synthesized push_coroutine< T >
               typename push_coroutine< void >::control_block synthesized_cb( this, ctx);
               push_coroutine< void > synthesized( & synthesized_cb);
               other = & synthesized_cb;
               try {
                   // call coroutine-fn with synthesized push_coroutine as argument
                   fn( synthesized);
               } catch ( forced_unwind const&) {
                   // do nothing for unwinding exception
               } catch (...) {
                   // store other exceptions in exception-pointer
                   except = std::current_exception();
               }
               // set termination flags
               state |= static_cast< int >( state_t::complete);
               // jump back to ctx
               other->ctx( preserve_fpu);
               BOOST_ASSERT_MSG( false, "pull_coroutine is complete");
            }),
    preserve_fpu( preserve_fpu_),
    state( static_cast< int >( state_t::unwind) ),
    except() {
    // enter coroutine-fn in order to have first value available after ctor returns
    ctx( preserve_fpu);
}

inline
pull_coroutine< void >::control_block::control_block( push_coroutine< void >::control_block * cb,
                                                      boost::context::execution_context const& ctx_) :
    other( cb),
    ctx( ctx_),
    preserve_fpu( other->preserve_fpu),
    state( 0),
    except() {
}

inline
pull_coroutine< void >::control_block::~control_block() {
    if ( 0 == ( state & static_cast< int >( state_t::complete ) ) &&
         0 != ( state & static_cast< int >( state_t::unwind) ) ) {
        // set early-exit flag
        state |= static_cast< int >( state_t::early_exit);
        ctx( preserve_fpu);
    }
}

inline
void
pull_coroutine< void >::control_block::resume() {
    other->ctx = boost::context::execution_context::current();
    ctx( preserve_fpu);
    if ( except) {
        std::rethrow_exception( except);
    }
    // test early-exit-flag
    if ( 0 != ( ( other->state) & static_cast< int >( state_t::early_exit) ) ) {
        throw forced_unwind();
    }
}

inline
bool
pull_coroutine< void >::control_block::valid() const noexcept {
    return nullptr != other && 0 == ( state & static_cast< int >( state_t::complete) );
}

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES2_DETAIL_PULL_CONTROL_BLOCK_IPP
