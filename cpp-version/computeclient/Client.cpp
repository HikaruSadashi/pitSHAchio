#include <iostream>
#include <unistd.h>
#include <string>
#include "socket.h"
#include <openssl/sha.h>

std::string incrementStr(std::string string)
{
    int i = string.length() - 1;
    for (; i >= 0; i--)
    {
        string[i] = ((char)(string[i])) + 1;
        switch (string[i])
        {
        case 0x3A:            // if char == 9
            string[i] = 0x41; // char = A
            i = -2;
            break;
        case 0x5B:            // if char == Z
            string[i] = 0x61; // char = a
            i = -2;
            break;
        case 0x7A:            // if char == z
            string[i] = 0x30; // char = 0
            break;            // increment next char
        default:
            i = -2;
            break;
        }
        if (i < 0)
        {
            break;
        }
    }
    if (i == -1)
    {
        string = string.append("0");
    }
    return string;
}

char intToHex(int x)
{
    char y = x + 48;
    if (y > 57)
        y += 39;
    return y;
}

int main(int argc, char* argv[])
{
    std::string ip = "127.0.0.1";
    if(argc > 1) {
        ip = argv[1];
    } 
    try
    {
        for (;;)
        {
            Sync::Socket socket(ip, 3010);
            Sync::ByteArray message;
            socket.Open();

            int succ = socket.Read(message);
            if (message.ToString()[0] == 18 || succ == 0)
            {
                socket.Close();
                sleep(3);
                continue;
            }
            std::string pwHash = message.ToString();
            if(pwHash.length() < 64) {
                std::cout << "Server Closed" <<std::endl;
                socket.Close();
                break;;
            }
            std::cout << "cracking " << pwHash << std::endl;

            bool complete = false;
            Sync::ByteArray readyArr ({(char)20});
            socket.Write(readyArr);
            for (;;)
            {
                int status = socket.Read(message);

                if (status == 0)
                {
                    complete = true;
                    break;
                }
                std::string incoming = message.ToString();
                if (incoming[0] == 22)
                {
                    incoming = "";
                }
                std::cout << "attempting strings of class \"" <<incoming<<"\""<<std::endl;

                std::string component = "";
                std::string sum = "";
                while (component.length() < 4)
                {
                    unsigned char buffer[32];
                    unsigned char *buffer_ptr = &buffer[0];
                    sum = "";
                    sum.append(incoming);
                    sum.append(component);
                    SHA256((unsigned char *)&sum[0], sum.length(), buffer_ptr);
                    std::string hash(64, 0);

                    for (int i = 0; i < 32; i++)
                    {
                        hash[i * 2] = intToHex((buffer[i] & 0xF0) >> 4);
                        hash[i * 2 + 1] = intToHex(buffer[i] & 0x0F);
                    }
                    if (hash.compare(pwHash) == 0)
                    {
                        Sync::ByteArray answer(sum);
                        socket.Write(answer);
                        complete = true;
                        break;
                    }
                    component = incrementStr(component);
                }
                if (complete)
                {
                    break;
                }

                Sync::ByteArray response({(char)10});
                socket.Write(response);
            }
            socket.Close();
            sleep(5);
        }
        return 0;
    }
    catch (std::basic_string<char, std::char_traits<char>, std::allocator<char>> e )
    {
        std::cout << "Server Closed "<< std::endl;
        return 0;
    }
}
