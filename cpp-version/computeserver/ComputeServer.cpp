#include "socket.h"
#include "socketserver.h"
#include "IPCServer.h"
#include "Blockable.h"
#include <iostream>
#include <unistd.h>
#include <thread>

std::string clientCount = "";
bool newHashReady = false;
Sync::ThreadSem readSem(1);
Sync::ThreadSem clientCountSem(1);
Sync::ByteArray hash("");
Sync::ByteArray storedAnswer;
Sync::ByteArray *ans = &storedAnswer;

void incrementClient()
{
    int i = clientCount.length() - 1;
    for (; i >= 0; i--)
    {
        clientCount[i] = ((char)(clientCount[i])) + 1;
        switch (clientCount[i])
        {
        case 0x3A:                 // if char == 9
            clientCount[i] = 0x41; // char = A
            i = -2;
            break;
        case 0x5B:                 // if char == Z
            clientCount[i] = 0x61; // char = a
            i = -2;
            break;
        case 0x7A:                 // if char == z
            clientCount[i] = 0x30; // char = 0
            break;                 // increment next char
        default:
            i = -2;
            break;
        }
        if (i < 0)
        {
            return;
        }
    }
    if (i == -1)
    {
        clientCount = clientCount.append("0");
    }
}

void endClient(Sync::Socket s)
{
    Sync::ByteArray t({(char)18}); // send cancel if nothing ready
    s.Write(t);                    // cancel
    s.Close();
}

void socketReply(Sync::Socket s)
{
    try
    {
        if (!newHashReady)
        {
            endClient(s);
            return;
        }
        s.Write(hash);
        Sync::ByteArray ready;
        s.Read(ready);
        if (ready.ToString()[0] != 20)
        {
            s.Close();
            return;
        }
        for (;;)
        {
            clientCountSem.Wait();
            Sync::ByteArray re;
            if (clientCount.length() == 0)
            {
                re = Sync::ByteArray({(char)22});
            }
            else
            {
                re = Sync::ByteArray(clientCount);
            }

            incrementClient();
            clientCountSem.Signal();

            int success = s.Write(re);
            if (success < 1)
            {
                s.Close();
                return;
            }

            int cl_status = s.Read(re);
            if (cl_status == 0)
            {
                s.Close();
                return;
            }

            std::string reply = re.ToString();

            if (reply[0] != 10)
            {
                storedAnswer = Sync::ByteArray(reply);
                std::cout << std::endl;
                std::cout << "pw cracked: " << (*ans).ToString() << std::endl;
                clientCount = std::string(100, '0');
            }

            if (clientCount.length() > 1 || !newHashReady)
            {
                // computations complete
                readSem.Wait();
                clientCount = "";
                newHashReady = false;
                readSem.Signal();
                s.Close();
                return;
            }
        }
    }
    catch (int e)
    {
        return;
    }
    return;
}

int main()
{
    bool socketBound = false;
    try
    {
        Sync::SocketServer server(3010);
        socketBound = true;

        std::thread ipcServer_thread(run_IPC, &hash, &readSem, &server, &newHashReady, ans);

        ipcServer_thread.detach();
        for (;;)
        {
            try
            {
                Sync::Socket s = server.Accept();
                std::thread th(socketReply, s);
                th.detach();
            }
            catch (int e)
            {
                if (e == 2)
                {
                    std::cout << "Shutting down" << std::endl;
                }
                break;
            }
        }
    }
    catch (std::basic_string<char, std::char_traits<char>, std::allocator<char>> e)
    {
        if (!socketBound)
        {
            std::cout << e << ", ";
        }
        std::cout << "shutting down..." << std::endl;
    }
}