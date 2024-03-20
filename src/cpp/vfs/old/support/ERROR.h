#pragma once

//#include "../string/va_string.h" // IWYU pragma: export
#include <iostream>
#include <assert.h>

#define ERROR( TXT ) \
    do { std::cerr << __FILE__ << ":" << __LINE__ << ": error: " << TXT << std::endl; assert( 0 ); } while ( 0 )
