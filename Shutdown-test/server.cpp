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

#define SERV_PORT 9999
 
int main(int argc, const char * argv[])
{
 
    struct sockaddr_in serverAdd;
    struct sockaddr_in clientAdd;
    
    bzero(&serverAdd, sizeof(serverAdd));
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAdd.sin_port = htons(SERV_PORT);
    
    socklen_t clientAddrLen;
    
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    setsockopt(listenfd,
               SOL_SOCKET, SO_REUSEADDR,
               (void *)&yes, sizeof(yes));
    
    if (listenfd < 0) {
        printf("创建socket失败\n");
        return -1;
    }
    
    int bindResult = bind(listenfd, (struct sockaddr *)&serverAdd, sizeof(serverAdd));
    if (bindResult < 0) {
        printf("绑定端口失败\n");
        close(listenfd);
        return -1;
    }
    
    listen(listenfd, 20);
    
    int connfd;
    unsigned char recvMsg[2469880];
    unsigned long long totalSize = 0;
    
    clientAddrLen = sizeof(clientAdd);
    connfd = accept(listenfd,(struct sockaddr *)&clientAdd,&clientAddrLen);
    if (connfd < 0) {
        printf("连接失败\n");
        return -1;
    }
    else{
//        这里我们用于测试，只接收一个连接
        close(listenfd);
    }
    signal(SIGPIPE, SIG_IGN);
    //shutdown(connfd,SHUT_RD);
    
    ssize_t readLen = read(connfd, recvMsg, 10);
        if (readLen < 0) {
            printf("璇诲彇澶辫触 errno = %d\n",errno);
            return -1;
        }
        else if (readLen == 0)
        {
        	printf("read return 0\n");
        }
        else
        {
            totalSize += readLen;
            printf("璇诲彇鎴愬姛 totalSize = %zd\n",totalSize);
        }
    while (1) {
        ssize_t writeLen = write(connfd, recvMsg, sizeof(recvMsg));
        
        if (writeLen < 0) {
            printf("写入失败 errno = %d\n",errno);
            close(connfd);
            return -1;
        }
        else
        {
            printf("writeLen:%ld\n",writeLen);
        }
 
    }
    
    return 0;
}

