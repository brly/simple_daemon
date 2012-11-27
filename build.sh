COMPILER=g++
FLAGS="-O3"
NAME="example_echo_server_daemon"
$COMPILER $FLAGS -o $NAME main.cc simple_daemon.cc

