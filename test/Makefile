OP_MODUO_SRC_DIR = ../Op-Muduo/network
OP_MODUO_LIB_DIR = lib/

OP_MODUO_SRC = $(wildcard $(OP_MODUO_SRC_DIR)/*.cc)
OP_MODUO_LIB = $(wildcard $(OP_MODUO_LIB_DIR)/*.o)

test_buffer : $(OP_MODUO_LIB) test_buffer.cc
	g++ -g test_buffer.cc $(OP_MODUO_LIB) -o test_buffer.out -pthread

test_tcp_server : $(OP_MODUO_LIB) test_tcp_server.cc
	g++ -g test_tcp_server.cc $(OP_MODUO_LIB) -o test_tcp_server.out -pthread

test_tcp_acceptor : $(OP_MODUO_LIB) test_tcp_acceptor.cc
	g++ test_tcp_acceptor.cc $(OP_MODUO_LIB) -o test_tcp_acceptor.out -pthread

test_timer_loop_thread : $(OP_MODUO_LIB) test_timer_loop_thread.cc
	g++ test_timer_loop_thread.cc $(OP_MODUO_LIB) -o test_timer_loop_thread.out -pthread

test_timer_loop : $(OP_MODUO_LIB) test_timer_loop.cc
	g++ test_timer_loop.cc $(OP_MODUO_LIB) -o test_timer_loop.out -pthread

test_channel : $(OP_MODUO_LIB) test_channel.cc
	g++ test_channel.cc $(OP_MODUO_LIB) -o test_channel.out -pthread

op_muduo_lib : $(OP_MODUO_LIB)
	g++ -g -c $(OP_MODUO_SRC)
	mv *.o $(OP_MODUO_LIB_DIR)
