#include "display.h"

BEG_VFS_NAMESPACE

DisplayItem *display( Displayer &ds, const std::string& str ) { return ds.string( str ); }
DisplayItem *display( Displayer &ds, std::string_view   str ) { return ds.string( str ); }
DisplayItem *display( Displayer &ds, const char*        str ) { return ds.string( str ); }
DisplayItem *display( Displayer &ds, char               str ) { return ds.string( { &str, 1 } ); }

DisplayItem *display( Displayer &ds, std::uint64_t      val ) { return ds.number( val ); }
DisplayItem *display( Displayer &ds, std::uint32_t      val ) { return ds.number( val ); }
DisplayItem *display( Displayer &ds, std::uint16_t      val ) { return ds.number( val ); }
DisplayItem *display( Displayer &ds, std::uint8_t       val ) { return ds.number( val ); }
DisplayItem *display( Displayer &ds, std::int64_t       val ) { return ds.number( val ); }
DisplayItem *display( Displayer &ds, std::int32_t       val ) { return ds.number( val ); }
DisplayItem *display( Displayer &ds, std::int16_t       val ) { return ds.number( val ); }
DisplayItem *display( Displayer &ds, std::int8_t        val ) { return ds.number( val ); }
DisplayItem *display( Displayer &ds, bool               val ) { return ds.number( val ); }
DisplayItem *display( Displayer &ds, double             val ) { return ds.number( val ); }
DisplayItem *display( Displayer &ds, float              val ) { return ds.number( val ); }

DisplayItem *display( Displayer &ds, const void*        val ) { return ds.number( std::size_t( val ) ); }
DisplayItem *display( Displayer &ds, void*              val ) { return ds.number( std::size_t( val ) ); }

END_VFS_NAMESPACE
