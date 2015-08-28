#ifndef __SOCKET_BASE_H__ 
#define __SOCKET_BASE_H__ 

#include <sys/types.h>
#include <sys/socket.h>
#if __APPLE__
#include <sys/socket.h>
#else
#include <linux/socket.h>
#endif
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

class socket_base
{
protected:
    int     m_sock_fd;
    int     m_accept_sock_fd;
    char    m_addr[16];
    int     m_port;
    int     m_recv_buf_size;
    int     m_send_buf_size;
    int     m_non_block_f;
    int     m_no_delay_f;

public:
    socket_base( int   no_delay_f,
                 int   non_block_f,
                 int   buf_size )
    {
        m_no_delay_f    = no_delay_f;
        m_non_block_f   = non_block_f;

        m_recv_buf_size = buf_size;
        m_send_buf_size = buf_size;
    }

    int set_sock_option();
    int send(char * data, int size);
    int recv(char * data);
    int close(int fd);
};

#endif
