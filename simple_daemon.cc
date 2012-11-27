#include "simple_daemon.hpp"

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

namespace {

const int kBuffer = 512;
char buf[kBuffer];

void init_sockaddr_in_struct(struct sockaddr_in& me, const unsigned port) {
  memset((char *)&me, 0, sizeof me);
  me.sin_family = AF_INET;
  me.sin_addr.s_addr = htonl(INADDR_ANY);
  me.sin_port = htons(port);  
}

inline bool make_socket(int& waiting_socket) {
  return (waiting_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ? false : true;
}

inline bool binding(int& waiting_socket, struct sockaddr_in& me) {
  return bind(waiting_socket, (sockaddr *)&me, sizeof me) == -1 ? false : true;
}

}; // anonymous namespace

void SimpleDaemon::init() {
  switch (fork()) {
    case 0: break;
    case -1: assert(false);
    default: exit(0);
  }

  if (setsid() < 0) assert(false);

  switch (fork()) {
    case 0: break;
    case -1: assert(false);
    default: exit(0);
  }

  chdir("/");
  umask(0);
  signal(SIGCHLD, SIG_IGN);

  int fd_limit = sysconf(_SC_OPEN_MAX);
  for (int fd = 0; fd < fd_limit; ++fd)
    close(fd);

}

void SimpleDaemon::run() {
  ready();
  
  struct sockaddr_in me;
  init_sockaddr_in_struct(me, port_);

  int waiting_socket;
  assert(make_socket(waiting_socket));
  assert(binding(waiting_socket, me));

  listen(waiting_socket, 1);

  while (1) {
    int data_socket = accept(waiting_socket, NULL, NULL);
    pid_t pid;
    if ((pid = fork()) == 0) {
      close(waiting_socket);
      proc(data_socket);
      close(data_socket);
      return;
    } else {
      close(data_socket);
    }
  }
  close(waiting_socket);
}

void SimpleDaemon::proc(const int kSocket) {
  char *temp_buf = new char[512];
  do {
    int n = read(kSocket, temp_buf, sizeof(temp_buf));
    write(kSocket, temp_buf, n);
  } while (strncmp(temp_buf, "quit", 4) != 0);
  delete[] temp_buf;
}
