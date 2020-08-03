#include "thread.hpp"
#include "interrupt.hpp"
#include "socket.hpp"
#include <thread>
#include <future>

namespace thread{


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

   // Create a new thread that calls ListenConnections() and send future_obj as argument
    std::thread listen_connections_thread(&socket::ListenConnections, std::move(future_obj));

   // Create a new thread that calls GetUserInterrumption()
    std::thread get_user_interruption_thread(&utils::GetUserInterruption);

  // Both threads are executing concurrently
  
  // When user_interruption is catched 
  get_user_interruption_thread.join();
  // Send signal to end listen connection thread
  exit_signal.set_value(); 
  listen_connections_thread.join(); 

  }

}
