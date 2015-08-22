#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <tcp_socket.h>

int main()
{
    int             ret;
    int             sock = -1;
    int             accept_sock = -1;
    struct timeval  timeout;
    fd_set          fd;
    char            buf[64];

    tcpsvr *        svr = new tcpsvr(1,1,0);

    sock = svr->open( 11111 );
    if( sock < 0 ) exit(1);

    printf("open okay...[%d]\n", sock);

    memset(buf, 0x00, sizeof(buf));

    while(1)
    {
        timeout.tv_sec  = 1;
        timeout.tv_usec = 0;

        FD_ZERO(&fd);
        if( sock > 0 ) FD_SET( sock, &fd );
        if( accept_sock > 0 ) FD_SET( accept_sock, &fd );

        if( (ret = select( FD_SETSIZE, &fd, NULL, NULL, &timeout)) <= 0 )
        {
            usleep(40);
            continue;
        }

        if( FD_ISSET( sock, &fd ) )
        {
            if( accept_sock > 0 )
            {
                close(accept_sock);
                accept_sock = -1;
            }

            accept_sock = svr->accept();
            if( accept_sock < 0 )
            {
                printf("accept fail...[%d] [%s]\n", accept_sock, strerror(errno));
                usleep(40);
                continue;
            }
            printf("accept okay...[%d]\n", accept_sock );
        }

        if( FD_ISSET( accept_sock, &fd ) )
        {
            ret = svr->recv(buf);
            if( ret < 0 )
            {
                svr->close(accept_sock);
                continue;
            }
            printf("recv... ret[%d] message[%s]\n", ret, buf );
        }
    }

    return 0;
}
