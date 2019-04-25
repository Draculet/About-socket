#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <signal.h>

#define SERV_ADDR "127.0.0.1"
#define SERV_PORT 9999
int main(void)
{
    struct sockaddr_in serverAdd;
    bzero(&serverAdd, sizeof(serverAdd));
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_addr.s_addr = inet_addr(SERV_ADDR);
    serverAdd.sin_port = htons(SERV_PORT);
    
    int connfd = socket(AF_INET, SOCK_STREAM, 0);
    
    int connResult = connect(connfd, (struct sockaddr *)&serverAdd, sizeof(serverAdd));
    if (connResult < 0) {
        printf("连接失败\n");
        close(connfd);
        return -1;
    }
    else
    {
        printf("连接成功\n");
    }
    
    ssize_t readLen;
 
    char sendMsg[200000] = {0};
    unsigned long long totalSize = 0;
    signal(SIGPIPE, SIG_IGN);
    //shutdown(connfd,SHUT_WR);
    shutdown(connfd,SHUT_RD);
    int count = 0;
    while (1) {
 
        readLen = read(connfd, sendMsg, 1);
        count++;
        if (readLen < 0) {
            printf("读取失败 errno = %d\n",errno);
            return -1;
        }
        else
        {
            totalSize += readLen;
            printf("读取成功 totalSize = %zd\n",totalSize);
        }
        sleep(1);
        if (totalSize == 1)
            shutdown(connfd,SHUT_RD);
       	if (count == 10)
       	{
       		shutdown(connfd,SHUT_WR);
       		ssize_t writeLen = write(connfd, sendMsg, 10);
        	if (writeLen < 0) {
            	printf("write return -1 ,errno = %d\n",errno);
            	close(connfd);
            return -1;
        	}
        	else
        	{
            	printf("writeLen:%ld\n",writeLen);
        	}
       	}     
            //close(connfd);
    }
}
