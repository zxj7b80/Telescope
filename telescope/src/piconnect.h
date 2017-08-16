#ifndef PICONNECT_H
#define PICONNECT_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
void connect_pi()
{
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(50000);
    serv_addr.sin_addr.s_addr = inet_addr("192.6.94.26"); //enter ip adress of the hexapod controller

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("sockfd = %d \n", sockfd);
    //    qDebug("654321");
    //qDebug()<<sockfd;
    if (sockfd < 0)
    {
        printf("ERROR opening socket\n");
    }
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        printf("connection to M2 error!\n");
        throw QString(strerror(errno))+"   connection to M2 error!";
        exit(0);
    }
}
#endif // PICONNECT_H
