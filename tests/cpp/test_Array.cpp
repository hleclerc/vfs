// #include <vfs/Vector.h>
#include <vfs/Scalar.h>
#include <vfs/List.h>

#include "catch_main.h"

// TEST_CASE( "Empty array", "" ) {
//     Vector b;
//     P( b );
// }
<<<<<<< HEAD
using ArraySize = Array<Int,ArrayTag::WantCtSizeInAllDims>;
=======
using ArrayShape = Array<Int,ArrayTag::ForceKnownSizes>;
>>>>>>> 4970fbdf272868c6ca582882b8e7603276cea972

TEST_CASE( "Rt dim array", "" ) {
    // using Vol = Array<Scalar>;
    ArrayShape a{ 1, 2, 3 };
    P( a );

<<<<<<< HEAD
    using Func = FuncInfo<CtString<"PT">>;
    vfs_call<void>( Func(), ensure_ct_known( 17 ) );

    //Vol v( FromSizesAndItemValue(), ArraySize{ 2, 2, 2 }, CtInt<0>() );
    // for( size_t i = 0; i < 4; i++ )
    //     v << i;
    //    P( v );
=======
    //P(  )
    // Vol v( FromSizesAndItemValue(), ArraySize{ 2, 2, 2 }, CtInt<0>() );
    // for( size_t i = 0; i < 4; i++ )
    //     v << i;
    //P( v );
>>>>>>> 4970fbdf272868c6ca582882b8e7603276cea972
 
    // Vol v{ 1, 2, 3, 4 };
}

// TEST_CASE( "Array", "" ) {
//     // // Point a{ 1, 2, 3 };
//     // // P( a.type() );
//     // // P( a );

//     // // Vector b{ 1, 2, 3 };
//     // // b << 6;
//     // // P( b );

//     // // List<Point> c{ { 1, 2, 3 }, { 4, 5, 6 } };
//     // // P( c.type() );
//     // // P( c );

//     using Vol = Array<Scalar>;

//     Vol vol = Vol::fill( { 3 }, 5 );
//     // P( vol( 0 ) );
//     // // vol( 0 ) = 10;
//     P( vol );

//     // // vol( 1 ) = 1.5;
//     // // P( vol );

//     // // P( type_promote( CtString<"">(), CtType<SI32>(), CtType<FP32>() ) );
// }
