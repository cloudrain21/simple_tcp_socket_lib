#include <socket_base.h>


int socket_base::set_sock_option()
{
    int     ret = 0;
    int     flag;

    if( m_no_delay_f > 0 )
    {
        ret = setsockopt( m_sock_fd,
                          IPPROTO_TCP,
                          TCP_NODELAY,
                          (void *)&m_no_delay_f,
                          sizeof(m_no_delay_f) );
        if( ret < 0 ) return -1;
    }

    if( m_send_buf_size > 0 )
    {
        ret = setsockopt( m_sock_fd,
                          SOL_SOCKET,
                          SO_SNDBUF,
                          (void *)&m_send_buf_size,
                          sizeof(m_send_buf_size) );
        if( ret < 0 ) return -2;
    }

    if( m_recv_buf_size > 0 )
    {
        ret = setsockopt( m_sock_fd,
                          SOL_SOCKET,
                          SO_RCVBUF,
                          (void *)&m_recv_buf_size,
                          sizeof(m_recv_buf_size) );
        if( ret < 0 ) return -3;
    }

    if( m_non_block_f > 0 )
    {
        flag = fcntl( m_sock_fd, F_GETFL, 0 );
        ret = fcntl(m_sock_fd, F_SETFL, flag | O_NDELAY );
        if( ret < 0 ) return -4;
    }

    return ret;
}

int _send( int fd, char * data, int size)
{
    int     ret;
    int     remain = size;
    char *  ptr    = data;

    do
    {
        ret = ::send(fd, ptr, remain, 0 );
        if( ret > 0 )
        {
            remain -= ret;
            ptr    += ret;
        }
        else if( ret == 0 )
        {
            continue;
        }
        else
        {
            if( (errno == EWOULDBLOCK) || (errno == EAGAIN) )
            {
                continue;
            }
            else
            {
                return -1;
            }
        }
    }
    while(remain > 0);
    
    return (size - remain);
}

int socket_base::send( char * data, int size )
{
    int     remain = 0;
    int     ret;
    char *  ptr;
    int     size_of_data = size;

    /*
     * size 를 담은 4 byte 부터 보낸다.
     */
    remain = sizeof(int);
    ptr    = (char *)&size_of_data;

    ret = _send( m_sock_fd, ptr, remain );
    if( ret != remain )
    {
        return -1;
    }

    /*
     * 사용자 data 를 보낸다.
     */
    remain = size;
    ptr    = data;

    return _send( m_sock_fd, ptr, remain );
}

int _recv( int fd, char * data, int size )
{
    int     ret;
    int     remain;
    char *  ptr;

    remain  = size;
    ptr     = data;

    do
    {
        ret = ::recv(fd, ptr, remain, 0);
        if( ret > 0 )
        {
            remain  -= ret;
            ptr     += ret;
        }
        else if( ret == 0 )
        {
            return -1;
        }
        else
        {
            if( (errno == EWOULDBLOCK) || (errno == EAGAIN) )
            {
                continue;
            }
            else
            {
                return -2;
            }
        }
    }while(remain > 0);

    return (size - remain);
}

int socket_base::recv(char * data)
{
    int     ret;
    char    buf[8];
    int     remain;

    memset(buf, 0x00, sizeof(buf));

    /*
     * size 를 담은 4 byte 부터 수신 
     */
    remain  = sizeof(int);
    ret = _recv( m_accept_sock_fd, buf, remain );
    if( ret != remain )
    {
        return -1;
    }

    /*
     * 사용자 data 를 수신 
     */
    remain = *(int *)buf;
    return _recv( m_accept_sock_fd, data, remain );
}

int socket_base::close(int fd)
{
    return ::close(fd);
}
