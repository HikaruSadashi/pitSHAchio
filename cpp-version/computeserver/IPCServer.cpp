#include "socket.h"
#include "socketserver.h"
#include "Blockable.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <thread>

void handleResponse(Sync::Socket socket, Sync::ByteArray *hex, Sync::ThreadSem *sem, bool *ready,
                    Sync::SocketServer *mainServer, Sync::SocketServer *ipcServer, Sync::ByteArray *ans)
{
    try
    {
        Sync::ThreadSem hashReadySem = *sem;
        Sync::ByteArray in;

        hashReadySem.Wait();
        int err = socket.Read(in);

        if (in.ToString()[0] == 't')
        {
            std::cout << "terminating server" << std::endl;
            socket.Close();
            (*mainServer).Shutdown();
            (*ipcServer).Shutdown();
            hashReadySem.Signal();
            return;
        }

        *ans = Sync::ByteArray(".");
        *hex = Sync::ByteArray(in.ToString());
        *ready = true;
        std::cout << "new hash received: ";
        std::cout << (*hex).ToString() << std::endl;

        hashReadySem.Signal();

        while (*ready)
        {
            sleep(1);
        }

        if ((*ans).ToString().compare(".") != 0)
        {
            socket.Write(*ans);
        }
        else
        {
            Sync::ByteArray response(".");
            socket.Write(response);
        }

        // std::cout << "closing!" << std::endl;
    }
    catch (std::basic_string<char, std::char_traits<char>, std::allocator<char>> e)
    {
        std::cout << "Shutting down..." << std::endl;
    }

    socket.Close();
}

void run_IPC(Sync::ByteArray *hex, Sync::ThreadSem *sem, Sync::SocketServer *mainServer, bool *ready, Sync::ByteArray *ans)
{
    Sync::SocketServer main = *mainServer;
    try
    {
        Sync::SocketServer server(3030);

        for (;;)
        {
            try
            {
                Sync::Socket sock = server.Accept(); // issue here?
                std::thread th(handleResponse, sock, hex, sem, ready, mainServer, &server, ans);
                th.detach();
            }
            catch (int e)
            {
                if (e == 2)
                {
                    std::cout << "Shutdown called " << std::endl;
                    break;
                }
                std::cout << "error with server accept: " << e << std::endl;
                break;
            }
        }
    }
    catch (std::basic_string<char, std::char_traits<char>, std::allocator<char>> e)
    {
        //std::cout << e << std::endl;
    }
   
}