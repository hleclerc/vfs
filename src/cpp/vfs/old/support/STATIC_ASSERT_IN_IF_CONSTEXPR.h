#pragma once

#define STATIC_ASSERT_WITH_RETURN_IN_IF_CONSTEXPR( RET, MSG ) do { static_assert( 0, MSG ); return RET; } while( false )
#define STATIC_ASSERT_IN_IF_CONSTEXPR( COND, MSG ) static_assert( COND, MSG )
