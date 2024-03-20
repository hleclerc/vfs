#pragma once

#include "vfs_namespace.h"
#include <cstdint>
#include <utility>

BEG_VFS_NAMESPACE

/**
  Simple object pool


*/
class BumpPointerPool {
public:
    /* */       BumpPointerPool( const BumpPointerPool &that ) = delete;
    /* */       BumpPointerPool( BumpPointerPool &&that );
    /* */       BumpPointerPool();
    /* */      ~BumpPointerPool();

    void        operator=      ( const BumpPointerPool &that ) = delete;

    auto        allocate_max   ( std::size_t min_size, std::size_t max_size = 2048ul, std::size_t alig = 8 ) -> std::pair<char *,std::size_t>; ///< take end of the current buffer, up to `max_size`. If the current buffer does not contain `min_size`, get room from a new buffer.
    char*       allocate       ( std::size_t size, std::size_t alig );
    char*       allocate       ( std::size_t size );

    template                   <class T,class... Args>
    T*          create         ( Args &&...args );

    void        clear          ();
    void        free           ();

    static auto include_for    () { return "vfs/support/BumpPointerPool.h"; }
    static auto type_name      () { return "BumpPointerPool"; }

private:
    struct      Frame          { Frame *prev_frame; char *ending_ptr; char content[ 8 ]; };
    struct      Item           { virtual ~Item() {} Item *prev; };
    union       Exof           { char *cp; std::size_t vp; };

    template    <class T>
    struct      Inst : Item    { template<class... Args> Inst( Args &&...args ) : object{ std::forward<Args>( args )... } {} virtual ~Inst() {} T object; };

    Exof        current_ptr;   ///<
    char*       ending_ptr;    ///<
    Frame*      last_frame;    ///<
    Item*       last_item;     ///<
};

END_VFS_NAMESPACE

#include "BumpPointerPool.tcc" // IWYU pragma: export
