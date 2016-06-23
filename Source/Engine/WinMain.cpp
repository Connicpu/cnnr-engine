#include <Common/Platform.h>

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
    int argc = __argc;
    char **argv = __argv;

    if (argc >= 2 && strcmp(argv[1], "-console") == 0)
    {
        AllocConsole();
        FILE *f;
        freopen_s(&f, "CONIN$", "r", stdin);
        freopen_s(&f, "CONOUT$", "w", stdout);
        freopen_s(&f, "CONOUT$", "w", stderr);
    }

    int main(int argc, char **argv);
    return main(argc, argv);
}

