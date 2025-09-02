#include <OS/OSError.h>
#include <OS/OSThread.h>
#include <VI/vi.h>
#include <cstdio>
#include <hook.hpp>
#include <memory.h>
#include <modules.h>
#include <nt/network.h>
#include <plugin.hpp>
#include <stdarg.h>
#include <string.h>

#include "debug.h"
#include "net_log.h"

#define AF_INET 0x2
#define SOCK_DGRAM 0x2
#define INADDR_ANY 0x0

#define MSG_WAITALL 0x100

namespace NetLog {
    static int srv_socket = -1;
    static struct sockaddr_in srvaddr, cliaddr;
    OSThread thread;
    char stack[0x1000];

    static void* listen(void* param)
    {
        while (1)
        {
            u32 len = sizeof(cliaddr);
            char buffer[256];

            while (!CanReceiveOnSocket(srv_socket))
            {
                VIWaitForRetrace();
                continue;
            }

            if (recvfrom(srv_socket, &buffer, 256, 0, (struct sockaddr*)&cliaddr, &len) > 0)
            {
                buffer[255] = '\0';

                if (strncmp(buffer, "Hello", sizeof(buffer)))
                {
                    debug_log("Stranger Danger (%s), skipping..\n", SOInetNtoA(cliaddr.sin_addr));
                    VIWaitForRetrace();
                    continue;
                }

                debug_log("Client Connected: %s\n", SOInetNtoA(cliaddr.sin_addr));
                if (sendto(srv_socket, "Hello Friend", strlen("Hello Friend"), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0)
                {
                    debug_log("Failed to send reply\n");
                    return NULL;
                }
            }

            return NULL;
        }
    }

    void (*_OSReport)();
    void send(const char* format, ...)
    {
        // immediate call to original.
        // This will implicitely use original args
        // THIS IS HACKY
        _OSReport();

        if (CanSendOnSocket(srv_socket) && cliaddr.sin_addr.s_addr != 0)
        {
            char buffer[256];

            va_list args;
            va_start(args, format);
            vsnprintf(buffer, 256, format, args);
            va_end(args);

            if (sendto(srv_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0)
            {
                debug_log("Failed to send message\n");
                return;
            }
        }

        return;
    }

    int Init(Plugin* plg)
    {
        srv_socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (srv_socket == -1)
        {
            debug_log("Failed to create socket!\n");
            closesocket(srv_socket);
            return 1;
        }

        memset(&srvaddr, 0, sizeof(srvaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        srvaddr.sin_family = AF_INET;
        srvaddr.sin_addr.s_addr = INADDR_ANY;
        srvaddr.sin_port = SOHtoNs(6969);

        if (bind(srv_socket, (const struct sockaddr*)&srvaddr, sizeof(srvaddr)) < 0)
        {
            debug_log("Failed to bind socket\n");
            closesocket(srv_socket);
            return 1;
        }

        OSCreateThread(&thread, listen, NULL, stack + sizeof(stack), sizeof(stack), 31, 0);
        OSResumeThread(&thread);

        SyringeCore::Hook* hook = plg->addHookEx(0x801d8600, reinterpret_cast<void*>(send), SyringeCore::OPT_DIRECT);
        hook->getTrampoline(reinterpret_cast<void**>(&_OSReport));
        return 0;
    }

} // namespace NetLog
