#include "socket.hpp"
#include <future>
#include <iostream>

namespace socket{
  void ListenConnections(std::future<void> future_obj){
    std::cout << "Test thread 1\n";
    while(future_obj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout){
      std::cout << "Test thread 1, working\n";
    }
  }
}
