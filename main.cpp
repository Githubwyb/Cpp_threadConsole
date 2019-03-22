/*
 * @Author WangYubo
 * @Date 09/17/2018
 * @Description
 */

#include "log.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define	UDP_TEST_PORT		80

int main(int argC, char* arg[]) {
    /* 建立socket*/
    int socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketFd < 0) {
        LOG_ERROR("Socket create failed, code %d", socketFd);
        return -1;
    }

    /* 填写sockaddr_in 结构 */
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");// 接收任意IP发来的数据

    /* 绑定socket */
    int rc = bind(socketFd, (struct sockaddr *)&addr, sizeof(addr));
    if (rc < 0) {
        LOG_ERROR("Bind failed, code %d", rc);
        return -1;
    }

    rc = listen(socketFd, 10);
    if (rc != 0) {
        LOG_ERROR("Listen failed, code %d", rc);
        return -1;
    }

    while (true) {
        struct sockaddr_in addrClient = {0};
        socklen_t addrLen = 0;
        int socketClient = accept(socketFd, (struct sockaddr *)&addrClient, &addrLen);
        LOG_DEBUG("Recv connect from %s:%d", inet_ntoa(addrClient.sin_addr), addrClient.sin_port);

        while (true) {
            char buff[255];
            ssize_t recvLen = recv(socketClient, buff, 255, 0);
            LOG_DEBUG("Recv data:");
            LOG_HEX(buff, recvLen);
            if (buff[0] == 'd') {
                close(socketClient);
                break;
            }
            if (buff[0] == 'r') {
                close(socketClient);
                close(socketFd);
                return 0;
            }
        }
    }
}
