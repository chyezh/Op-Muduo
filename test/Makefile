HTTPACK_SRC_DIR = ../httpack/network
HTTPACK_LIB_DIR = lib/

HTTPACK_SRC = $(wildcard $(HTTPACK_SRC_DIR)/*.cc)
HTTPACK_LIB = $(wildcard $(HTTPACK_LIB_DIR)/*.o)

test_buffer : $(HTTPACK_LIB) test_buffer.cc
	g++ -g test_buffer.cc $(HTTPACK_LIB) -o test_buffer.out -pthread

test_tcp_server : $(HTTPACK_LIB) test_tcp_server.cc
	g++ -g test_tcp_server.cc $(HTTPACK_LIB) -o test_tcp_server.out -pthread

test_tcp_acceptor : $(HTTPACK_LIB) test_tcp_acceptor.cc
	g++ test_tcp_acceptor.cc $(HTTPACK_LIB) -o test_tcp_acceptor.out -pthread

test_timer_loop_thread : $(HTTPACK_LIB) test_timer_loop_thread.cc
	g++ test_timer_loop_thread.cc $(HTTPACK_LIB) -o test_timer_loop_thread.out -pthread

test_timer_loop : $(HTTPACK_LIB) test_timer_loop.cc
	g++ test_timer_loop.cc $(HTTPACK_LIB) -o test_timer_loop.out -pthread

test_channel : $(HTTPACK_LIB) test_channel.cc
	g++ test_channel.cc $(HTTPACK_LIB) -o test_channel.out -pthread

httpack_lib : $(HTTPACK_LIB)
	g++ -g -c $(HTTPACK_SRC)
	mv *.o $(HTTPACK_LIB_DIR)
