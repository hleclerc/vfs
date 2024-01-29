// #include "string/va_string.h"
// #include <iostream>

#include "ThreadPool.h"
#include "wsq.hpp"
#include <thread>

BEG_VFS_NAMESPACE

//// nsmake lib_name pthread
ThreadPool thread_pool;

void ThreadPool::set_max_nb_threads( int max_nb_threads ) {
    _max_nb_threads = max_nb_threads;
}

int ThreadPool::nb_available_threads() {
    if ( _nb_hardware_threads == 0 )
        _nb_hardware_threads = std::thread::hardware_concurrency();
    return std::min( _max_nb_threads, _nb_hardware_threads );
}

void ThreadPool::execute( const std::function<void( PI num_thread, PI nb_threads )> &f ) {
    // launch
    PI nb_threads = nb_available_threads();
    std::vector<std::thread> thiefs;
    thiefs.reserve( nb_threads );
    for( PI num_thread = 0; num_thread < nb_threads; ++num_thread ) {
        thiefs.push_back( std::thread{ [&f,num_thread,nb_threads]() {
            f( num_thread, nb_threads );
        } } );
    }

    // join
    for( std::thread &thief : thiefs )
        thief.join();
}

void ThreadPool::execute( PI nb_jobs, const std::function<void( PI nb_threads, const std::function<void( const std::function<void( PI num_job, PI num_thread )> & )> & )> &cb ) {
    PI nb_threads = nb_available_threads();
    cb( nb_threads, [&]( const std::function<void( PI num_job, PI num_thread )> &f ) {
        // work-stealing queue of integer items
        WorkStealingQueue<PI> queue;

        // only one thread can push and pop
        std::thread owner( [&queue,&f,nb_jobs]() {
            for( PI i = 0; i < nb_jobs; ++i )
                queue.push( i );
            while ( ! queue.empty() ) {
                // std::cout << va_string( "queue.size: {}\n", queue.size() ) << std::flush;
                std::optional<int> num_job = queue.pop();
                if ( num_job.has_value() )
                    f( num_job.value(), 0 );
            }
        });

        // multiple thives can steal
        std::vector<std::thread> thiefs;
        thiefs.reserve( nb_threads - 1 );
        for( PI n = 1; n < nb_threads; ++n ) {
            thiefs.push_back( std::thread{ [&queue,&f,n]() {
                while( ! queue.empty() ) {
                    // std::cout << va_string( "queue.size: {}\n", queue.size() ) << std::flush;
                    std::optional<int> num_job = queue.steal();
                    if ( num_job.has_value() )
                        f( num_job.value(), n );
                }
            } } );
        }

        owner.join();
        for( std::thread &thief : thiefs )
            thief.join();
    } );
}

END_VFS_NAMESPACE
