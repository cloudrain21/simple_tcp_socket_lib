# generic settings
SHELL = /bin/sh

INCLUDE = -I.

# Targets

ARLIBFILE = libmysocket.a
SOLIBFILE = libmysocket.so
LIBRARYFILES = $(ARLIBFILE) $(SOLIBFILE)
LIBOBJFILES = socket_base.o tcp_socket.o

# Building binaries
CC = g++
#CFLAGS = -g -W -Wall
CFLAGS = -g -W -Wall -O0 \
		 -fno-strict-aliasing -rdynamic -fno-inline -m64 -mtune=k8 \
		 -fno-rtti  -fPIC -D_POSIX_PTHREAD_SEMANTICS -D_POSIX_THREADS \
		 -D_POSIX_THREAD_SAFE_FUNCTIONS -D_REENTRANT -D_THREAD_SAFE \
		 -D_GNU_SOURCE $(INCLUDE)
#C99FLAGS = -I./include -I.. -std=c99 -Wall -pedantic -fsigned-char -O2
LDFLAGS =
LIBS = -lm -lpthread -ldl -rdynamic -lrt
LDENV = LD_RUN_PATH=/lib:/usr/lib:/usr/local/lib:.:..
POSTCMD = true

#================================================================
# Suffix rules
#================================================================

.SUFFIXES :
.SUFFIXES : .cpp .o

.cpp.o :
	$(CC) -c $(CFLAGS) $<

#================================================================
# Actions
#================================================================

all : $(LIBRARYFILES)
	@$(POSTCMD)
	@printf '\n'
	@printf '#================================================================\n'
	@printf '# Ready to install.\n'
	@printf '#================================================================\n'


clean :
	rm -rf $(LIBRARYFILES) $(LIBOBJFILES) *.o a.out *~

install :
	@printf '#================================================================\n'
	@printf '# do something\n'
	@printf '#================================================================\n'

.PHONY : all clean install


#================================================================
# Building binaries
#================================================================

$(ARLIBFILE) : $(LIBOBJFILES)
	$(AR) $(ARFLAGS) $@ $(LIBOBJFILES)


$(SOLIBFILE) : $(LIBOBJFILES)
	$(CC) $(CFLAGS) -shared -Wl,-soname,libmysocket.so -o $@ $(LIBOBJFILES) $(LDFLAGS) $(LIBS)
	#$(CC) -shared -static-libgcc -rdynamic -m64 -mtune=k8 -o $@ $(LIBOBJFILES) -L. $(LIBS)


# END OF FILE
