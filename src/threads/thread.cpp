#include "thread.hpp"
#include "interrupt.hpp"
#include "socketHandler.hpp"
#include <thread>
#include <future>
#include <mutex>
#include <iostream>

namespace thread{

  // As we have multiple threads executing at the same time, we 
  // gotta handle the std::cout calls so messages don't get mixed up
  void PrintSafe(std::string const &s){
    // Our mutex will flag and therefore protect that each std::cout 
    // call is handled one at the time, static is needed so that the 
    // var prevail though several calls 
    static std::mutex my_mutex;
  
    // Is a mutex wrapper that provides a mechanism for owning 
    // a mutex for the duration of a scoped block
    // at the end of the block lock_guard is destructed and 
    // the mutex is released 
    const std::lock_guard<std::mutex> l(my_mutex);
    std::cout << s;
    std::cout << std::flush;
  }

  void HandleThreads(){
    // We need a way to signal the connection thread when to finish
    // We pass a std::future<void> object to the thread and it will exit when 
    // a value in the future is available.
    // As we want only to signal the thread and not acutally pass any value in that signal
    // we can use future object of type void
    
    // Create a std::promise object
    std::promise<void> exit_signal;

   // Fetch the associated future object from this promise
    std::future<void> future_obj = exit_signal.get_future();


   // Create a new thread that calls GetUserInterrumption()
    std::thread get_user_interruption_thread(&utils::GetUserInterruption);

   // Create a new thread that calls ListenConnections() and send future_obj as argument
    std::thread listen_connections_thread(&sockethandler::ListenConnections, std::move(future_obj));

  // Both threads are executing concurrently
  
  // When user_interruption is catched 
  get_user_interruption_thread.join();
  thread::PrintSafe("Signaling socket to close\n");
  // Send signal to end listen connection thread
  exit_signal.set_value(); 
  listen_connections_thread.join(); 

  }

}
