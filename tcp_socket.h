#ifndef __TCP_SOCKET_H__ 
#define __TCP_SOCKET_H__ 

#include <socket_base.h>

class tcpsvr : public socket_base
{
public:
    tcpsvr( int no_delay_f,
            int non_block_f,
            int buf_size )
        : socket_base( no_delay_f, non_block_f, buf_size ) {}

    int open( int port );
    int accept();
};


class tcpcli : public socket_base
{
public:
    tcpcli( int no_delay_f,
            int non_block_f,
            int buf_size )
        : socket_base( no_delay_f, non_block_f, buf_size ) {}

    int connect( char * addr, int port );
};


#endif
