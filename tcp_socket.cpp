#include <tcp_socket.h>


int tcpsvr::open( int   port )
{
    int                 ret;
    struct sockaddr_in  in_sock;
    int                 sock;
    int                 optval;

    memset((char *)&in_sock, 0x00, sizeof(struct sockaddr_in));

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if( sock < 0 ) return -1;

    m_port    = port;
    m_sock_fd = sock;

    in_sock.sin_family = AF_INET;
    in_sock.sin_addr.s_addr = htonl(INADDR_ANY);
    in_sock.sin_port = htons(port);

    ret = bind( sock, (struct sockaddr*)&in_sock, sizeof(in_sock));
    if( ret < 0 ) return -2;

    optval = 1;
    ret = setsockopt( sock,
                      SOL_SOCKET,
                      SO_REUSEADDR,
                      (void *)&optval,
                      sizeof(optval) );
    if( ret < 0 ) return -3;

    ret = set_sock_option();
    if( ret < 0 ) return ret;

    ret = listen( sock, 5 );
    if( ret < 0 ) return -4;

    return sock;
}


int tcpcli::connect( char*     addr,
                     int       port )
{
    int                 ret;
    struct sockaddr_in  serv;
    int                 sock;

    if( addr == NULL ) return -1;

    memset((char *)&serv, 0x00, sizeof(struct sockaddr_in));

    sock = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
    if( sock < 0 ) return -2;

    m_sock_fd = sock;
    m_port    = port;
    strcpy(m_addr, addr);

    ret = set_sock_option();
    if( ret < 0 ) return -3;

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(addr);
    serv.sin_port = htons(port);

    ret = ::connect( sock,
                     (struct sockaddr *)&serv,
                     sizeof(struct sockaddr_in) );
    if( ret < 0 ) return -4;


    return sock;
}

int tcpsvr::accept()
{
    struct sockaddr_in  cli;
    int                 addr_len;
    int                 new_sock;
    int                 flag;
    int                 ret;

    memset((char *)&cli, 0x00, sizeof(struct sockaddr_in) );

    addr_len = sizeof(struct sockaddr_in);

    new_sock = ::accept( m_sock_fd,
                        (struct sockaddr *)&cli,
                        (socklen_t *)&addr_len );
    if( new_sock < 0 ) return -1;

    if( m_non_block_f > 0 )
    {
        flag = fcntl( new_sock, F_GETFL, 0 );
        ret = fcntl( new_sock, F_SETFL, flag | O_NDELAY );
        if( ret < 0 ) return -2;
    }

    m_accept_sock_fd = new_sock;

    return new_sock;
}
