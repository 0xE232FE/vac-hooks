#include "Hooks.h"
#include "Utils.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        PBYTE toPatch = Utils_findPattern(L"steamservice", "\x74\x47\x6A\x01\x6A", 0);
        if (toPatch) {
            DWORD old;
            VirtualProtect(toPatch, 1, PAGE_EXECUTE_READWRITE, &old);
            *toPatch = 0xEB;
            VirtualProtect(toPatch, 1, old, &old);
            Utils_hookImport(L"steamservice", "kernel32.dll", "LoadLibraryExW", Hooks_LoadLibraryExW);
            MessageBoxW(NULL, L"Loaded successfully!", L"VAC hooks", MB_OK | MB_ICONINFORMATION);
        } else {
            MessageBoxW(NULL, L"Something went wrong...", L"VAC hooks", MB_OK | MB_ICONERROR);
        }
        DisableThreadLibraryCalls(hModule);
    }
    return TRUE;
}
