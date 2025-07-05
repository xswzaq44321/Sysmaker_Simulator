#include "socket_api.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <array>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

int main(int argc, char* argv[])
{
    if (argc <= 1) {
        printf("usage: %s <unix-socket-path>\n", argv[0]);
        exit(1);
    }
    client client_obj = {
        .m_isUnix = true,
        .m_pathOrIp = argv[1],
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
            uint64_t size = ftell(fp) + 1;
            fseek(fp, 0L, SEEK_SET);
            vector<char> buf(size);

            fread(buf.data(), sizeof(char), buf.size(), fp);
            printf("netlist content: \n%s\n", buf.data());

            send_data(client_obj.server_fd, &size, 8);
            int sentBytes = send_data(client_obj.server_fd, buf.data(), size);
            printf("netlist.json sent, total %d bytes\n", sentBytes);
            fclose(fp);
        }

        vector<uint16_t> resp_msg(15 * 11);
        for (int i = 0; i < 11; ++i) {
            resp_msg[i * 15 + 2] = i;
        }
        int resp_msg_idx = -2;

        while (client_obj.server_fd != -1) {
            uint64_t incoming_size;
            int bytesRecv = recv_data_nowait(client_obj.server_fd, &incoming_size, 8);
            if (bytesRecv > 0) { /* TODO: deal with bytesRecv == 0*/
                bytesRecv = recv_data(client_obj.server_fd, buf, incoming_size);
                assert(incoming_size == bytesRecv && bytesRecv < buf_size);
                buf[bytesRecv] = '\0'; // append null-terminate just in case
                printf("Data size: %ld\n", incoming_size);
                printf("Data received: \"%s\"\n", buf);
                // datas are now in `buf`, start processing...

                json j = json::parse(buf);
                uint64_t duration = (j["Interface Configuration"]["Data Frame Size"].get<int>() + 2)
                    * (1e9 / j["Interface Configuration"]["Clock Frequency"].get<int>());
                printf("Probably gonna take %ld ns\n", duration);
                printf("BeginTime: %lld\n", stoll(j["BeginTime"].get<string>()));
                j["EndTime"] = to_string(stoll(j["BeginTime"].get<string>()) + duration) + " ns";
                if (resp_msg_idx < 0)
                    j["Data"]["MISO"] = json::array({ 0 });
                else
                    j["Data"]["MISO"] = json::array({ resp_msg[resp_msg_idx] });
                printf("SendData: [%d]=%d\n", resp_msg_idx, resp_msg[resp_msg_idx]);
                if (resp_msg_idx < 0)
                    ++resp_msg_idx;
                else
                    resp_msg_idx = (resp_msg_idx + 1) % resp_msg.size();

                strncpy(buf, j.dump(4).c_str(), buf_size);
                uint64_t outgoing_size = strnlen(buf, buf_size);
                printf("Respond Data size: %ld\n", outgoing_size);
                printf("Respond Data: \"%s\"\n", buf);
                send_data(client_obj.server_fd, &outgoing_size, 8);
                send_data(client_obj.server_fd, buf, outgoing_size);
            } else if (bytesRecv == -1) {
                printf("Server closed\n");
                client_obj.server_fd = -1;
                client_close(&client_obj);
                break;
            }
        }
    }
}
