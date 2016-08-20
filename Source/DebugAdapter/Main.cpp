#include <Common/String.h>
#include <connorlib/messageipc.h>
#include <thread>
#include <iostream>
#include <iomanip>

char defaultcmd[] = "Game.exe --console";
char pausedcmd[] = "Game.exe --console --startpaused";

int main(int argc, const char *const *argv)
{
    using namespace MessageIpc;

    if (argc < 3)
        return 1;

    char *cmdline = defaultcmd;
    if (argc > 3 && argv[3] == "start-paused")
    {
        cmdline = pausedcmd;
    }

    unsigned pid;
    if (argv[1] == "launch"_s)
    {
        STARTUPINFO startinfo = { sizeof(startinfo) };
        PROCESS_INFORMATION procinfo;
        CreateProcessA(
            argv[2],
            cmdline,
            nullptr,
            nullptr,
            false,
            DETACHED_PROCESS,
            nullptr,
            nullptr,
            &startinfo,
            &procinfo
        );

        pid = procinfo.dwProcessId;
    }
    else if (argv[1] == "attach"_s)
    {
        pid = std::stoul(argv[1]);
    }

    std::cout << std::setiosflags(std::ios::binary);
    std::cin >> std::setiosflags(std::ios::binary);

    Sleep(200);

    std::cout << "21\n{\"event\":\"ready\"}";
    std::cout.flush();

    IpcClient client = std::move(*IpcClient::OpenClient("cnnr-lua-debugger", pid));

    std::thread write_thread([&]()
    {
        std::vector<char> data;
        for (;;)
        {
            unsigned len = 0;
            std::cin.read((char *)&len, 4);

            data.resize(len);
            if (!std::cin.read(data.data(), len))
                return;

            if (!client.Send((const uint8_t *)data.data(), len))
                return;
        }
    });

    while (auto msg = client.Recv())
    {
        std::cout << msg->len() << std::endl;
        std::cout.write((const char *)msg->data(), msg->len());
        std::cout.flush();
    }

    _exit(0);
}
