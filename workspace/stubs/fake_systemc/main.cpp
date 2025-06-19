#include "socket_api.h"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    client client_obj = {
        .m_isUnix = true,
        .m_pathOrIp = "/tmp/fake_qemu.sock"
    };
    char buf[100'000];
    int buf_size = 100'000;

    while (true) {
        client_create_socket(&client_obj);

        int waitFor = 3600;
        printf("Waiting server(QEMU) connection on path \"%s\" for %d secs\n", client_obj.m_pathOrIp, waitFor);
        if (client_connect_timeout(&client_obj, waitFor) == -1) {
            printf("Failed to connect to server, closing...\n");
            exit(-1);
        } else {
            printf("Server(QEMU) connected\n");
        }

        // sending netlist.json
        {
            FILE* fp = fopen("netlist.json", "r");
            if (fp == NULL)
                perror("fopen");
            fseek(fp, 0L, SEEK_END);
            int64_t size = ftell(fp) + 1;
            fseek(fp, 0L, SEEK_SET);
            vector<char> buf(size);

            fread(buf.data(), sizeof(char), buf.size(), fp);
            printf("netlist content: \n%s\n", buf.data());

            send_data(client_obj.server_fd, &size, 8);
            int sentBytes = send_data(client_obj.server_fd, buf.data(), size);
            printf("netlist.json sent, total %d bytes\n", sentBytes);
            fclose(fp);
        }

        while (client_obj.server_fd != -1) {
            int64_t incoming_size;
            int bytesRecv = recv_data_nowait(client_obj.server_fd, &incoming_size, 8);
            if (bytesRecv > 0) {
                bytesRecv = recv_data(client_obj.server_fd, buf, incoming_size);
                // datas are now in `buf`, start processing...

                printf("Data size: %ld\n", incoming_size);
                printf("Data received: \"%.*s\"\n", bytesRecv, buf);
            } else if (bytesRecv == -1) {
                printf("Server closed\n");
                client_obj.server_fd = -1;
                client_close(&client_obj);
            }
        }
    }
}
