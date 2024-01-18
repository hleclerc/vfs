#include <vfs/support/display/Displayer.h>
#include "catch_main.h"
#include <iostream>

void test_display( DisplayItem *di ) {
    di->write_to( std::cout, { .single_line = true } );
    std::cout << std::endl;

    di->write_to( std::cout, { .single_line = false } );
    std::cout << std::endl;
}

TEST_CASE( "Displayer", "" ) {
    Displayer ds;
    // test_display( ds.number( 17.5 ) );
    // test_display( ds.array( {
    //     ds.array( { ds.number( 17.5 ), ds.number( 18.5 ) } ),
    //     ds.array( { ds.number( 270.5 ), ds.number( 28.5 ) } ),
    // } ) );
    test_display( ds.array( {
        ds.object( "Yo", { { "a", ds.number( 1 ) }, { "b", ds.number( 2 ) } } ),
        ds.object( "Yo", { { "a", ds.number( 3 ) }, { "b", ds.number( 4 ) } } ),
    } ) );
}
