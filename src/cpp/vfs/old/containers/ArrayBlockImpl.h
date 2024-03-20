#pragma once

#include "Memory/Memory_Cpu.h"

BEG_VFS_NAMESPACE

///
template<class ItemPtr,
         class CtCoords    = CtIntList<-1>, // position of this block
         class CtStrides   = CtIntList<sizeof(ItemPtr)>,
         class CtSizes     = CtIntList<-1>,
         class CtCapas     = CtIntList<-1>,
         class CtCapaAligs = CtIntList<1>,
         int   global_alig = -1,
         class Memory      = Memory_Cpu>
class ArrayBlockImpl;

#define DTP template<class ItemPtr,class CtSizes>
#define UTP ArrayBlockImpl<ItemPtr,CtSizes>

DTP void for_each_template_arg( auto &&f ) { f( CtType<ItemPtr>() ); f( CtType<CtSizes>() ); }
DTP void get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/support/containers/ArrayImpl.h" ); }
DTP auto template_type_name( CtType<UTP> ) { return "ArrayImpl"; }

auto block_type_after_operation( auto op_name, auto block_type ) { return block_type; }

#undef DTP
#undef UTP

END_VFS_NAMESPACE
