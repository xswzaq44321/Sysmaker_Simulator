#include <cstdlib>
#include <iostream>

#include "socket_api.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc <= 1) {
        printf("usage: %s <unix-socket-path>\n", argv[0]);
        exit(1);
    }
    char buf[100'000 + 1];
    struct server obj = {
        .m_isUnix = true,
        .m_pathOrIp = argv[1],
    };

    server_create_socket(&obj);
    printf("Connecting to client...\n");
    server_bind_listen_socket(&obj);
    printf("Client connected\n");
    printf("Input file path to be transmitted: ");

    char fileName[256];
    while (scanf(" %s", fileName) != EOF) {
        FILE* fp = fopen(fileName, "r");
        if (fp == NULL) {
            perror("fopen");
        } else {
            int readLen = fread(buf, sizeof(char), 100'000, fp);
            int sentBytes = send_data(obj.client_fd, buf, readLen);
            printf("json file: %s sent, total %d bytes\n", fileName, sentBytes);
            fclose(fp);
        }
        printf("Input file path to be transmitted: ");
    }
    printf("\nClosing connection...\n");
    server_close(&obj);
}