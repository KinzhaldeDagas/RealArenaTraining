#include <Windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    (void)lpvReserved;

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        // Reduce loader noise and avoid unnecessary thread notifications.
        DisableThreadLibraryCalls((HMODULE)hinstDLL);
    }

    return TRUE;
}
