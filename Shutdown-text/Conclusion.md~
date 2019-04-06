* shutdown分为shutdown(connfd,SHUT_WR) 和 shutdown(connfd,SHUT_RD)
* 1.当关闭server读一端时 即shutdown(connfd,SHUT_RD),server还能不停地调用write传输数据
* 2.当client关闭读一端的情况: 假设client的逻辑是不断地read(),且每次只read 1字节后睡眠1s,此时TCP接收缓冲区肯定会被占满,因为client应用层没有即时读走tcp缓冲区数据，这也使得不停发送数据的server的blocking write发生堵塞,然后我们应用层在client read一半时shutdown client的读一端，即shutdown(connfd,SHUT_RD)，此时client调用read还是正常返回1(正常读取),而server一端的处于堵塞的write返回已写入tcp发送缓冲区的字节数,再次write返回-1(SIGPIPE),而client的read在读取完自己接受缓冲区的内容之后也返回-1
* 3.如果read一半是不调用shutdown转而close,那么对端write立刻返回已写入字节数,read也返回-1;
* 4.抓包情况待补充,是否发送FIN待验证,如果发送了FIN,且对端无法识别FIN是close发出还是shutdown发出,那么又是如何在shutdown之后仍能保持通信?
