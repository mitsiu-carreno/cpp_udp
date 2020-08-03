#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include <future>

namespace sockethandler{
  void ListenConnections(std::future<void> future_obj);
}

#endif
