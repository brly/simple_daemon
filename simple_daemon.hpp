#ifndef SIMPLE_DAEMON_HPP_
#define SIMPLE_DAEMON_HPP_

#include <cassert>

class SimpleDaemon {

  unsigned port_;

 public:
  
  SimpleDaemon(unsigned port = 50000) : port_(port) {
    assert(port < 65535);
    init();
  }

  ~SimpleDaemon() {}

  // to be "daemon process"
  void init();

  void run();

 private:

  // This function called before call proc() function.
  virtual void ready(){}

  // This function called by run() function.
  // argument "kSocket" is communication (Client <-> Server) sockets.
  // This is worked as 'Echo Server'
  // Try to do telnet command.
  virtual void proc(const int kSocket);
  
};

#endif // SIMPLE_DAEMON_HPP_
