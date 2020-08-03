#ifndef THREAD_H
#define THREAD_H

#include <string>

namespace thread{
  void PrintSafe(const std::string &s);
  void HandleThreads();
}

#endif
