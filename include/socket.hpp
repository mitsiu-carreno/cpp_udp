#ifndef SOCKET_H
#define SOCKET_H

#include <future>

namespace socket{
  void ListenConnections(std::future<void> future_obj);
}

#endif
