
#include "simple_daemon.hpp"

#include <cstring>
#include <unistd.h>

int main() {
  SimpleDaemon simpleDaemon;
  simpleDaemon.run();
  return 0;
}
