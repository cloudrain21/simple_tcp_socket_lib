#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <tcp_socket.h>

int main()
{
    int      i;
    int      ret;
    int      sock;
    char     buf[64];
    tcpcli * cli = new tcpcli(1,0,0);

    sock = cli->connect( (char *)"127.0.0.1", 11111 );
    if( sock < 0 ) exit(1);

    printf("open okay...[%d]\n", sock);

    memset(buf, 0x00, sizeof(buf));

    for(i=0; i<10; i++)
    {
        sprintf(buf, "this is [%d]th test message !!!!", i);

        ret = cli->send(buf, strlen(buf));
        printf("[%d] send... ret (%d)\n", i, ret);
    }

    sleep(1);

    return 0;
}
