#pragma once

#include "TypeConfig.h"
#include <functional>

namespace Vfs  {

/**
  f( num_item, num_thread );

  TODO: use atomics
*/
class ThreadPool {
public:
    int    nb_available_threads (); ///<
    void   set_max_nb_threads   ( int max_nb_threads );
    void   execute              ( PI nb_jobs, const std::function<void( PI nb_threads, const std::function<void( const std::function<void( PI num_job, PI num_thread )> & )> & )> & ); ///< nb_jobs should be > something like 4*nb_thread, but not too high, too avoid overhead
    void   execute              ( const std::function<void( PI num_thread, PI nb_threads )> &f ); ///<
    void   init                 ( int nb_threads = 0 ); ///< call to init is not mandatory (checked during `execute` anymway) but is a way to define explicitly nb_threads

private:
    int    _nb_hardware_threads = 0;
    int    _max_nb_threads      = std::numeric_limits<int>::max();
};

extern ThreadPool thread_pool;

} // namespace Vfs
