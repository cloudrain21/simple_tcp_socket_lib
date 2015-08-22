
# Targets
BIN= tcpsvr tcpcli

CC = g++
INCLUDE = -I..
CFLAGS = -g -W -Wall -O0 \
		 -fno-strict-aliasing -fno-inline -rdynamic -m64 -mtune=k8 \
		 -fno-rtti  -fPIC -D_POSIX_PTHREAD_SEMANTICS -D_POSIX_THREADS \
		 -D_POSIX_THREAD_SAFE_FUNCTIONS \
		 -D_REENTRANT -D_THREAD_SAFE -D_GNU_SOURCE $(INCLUDE)
LDFLAGS =
LIBS = -L.. -lmysocket
LDENV = LD_RUN_PATH=/lib:/usr/lib:/usr/local/lib:.:..
POSTCMD = true

.SUFFIXES :
.SUFFIXES : .cpp .o

.cpp.o :
	$(CC) -c $(CFLAGS) $<

all : $(BIN)

clean :
	rm -rf $(BIN) core.* *.o a.out *~ 

.PHONY : all clean

tcpsvr: tcpsvr.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LIBOBJFILES) $(LDFLAGS) $(LIBS)

tcpcli: tcpcli.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LIBOBJFILES) $(LDFLAGS) $(LIBS)


# END OF FILE
