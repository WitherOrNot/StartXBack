/*

Developed in collaboration with MAS & ASDCORP

*/

#include "pch.h"
#include <bcrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include "MinHook.h"

#pragma comment(linker,"/export:GetFileVersionInfoA=C:\\Windows\\System32\\version.dll.GetFileVersionInfoA")
#pragma comment(linker,"/export:GetFileVersionInfoW=C:\\Windows\\System32\\version.dll.GetFileVersionInfoW")
#pragma comment(linker,"/export:GetFileVersionInfoByHandle=C:\\Windows\\System32\\version.dll.GetFileVersionInfoByHandle")
#pragma comment(linker,"/export:GetFileVersionInfoExA=C:\\Windows\\System32\\version.dll.GetFileVersionInfoExA")
#pragma comment(linker,"/export:GetFileVersionInfoExW=C:\\Windows\\System32\\version.dll.GetFileVersionInfoExW")
#pragma comment(linker,"/export:GetFileVersionInfoSizeA=C:\\Windows\\System32\\version.dll.GetFileVersionInfoSizeA")
#pragma comment(linker,"/export:GetFileVersionInfoSizeW=C:\\Windows\\System32\\version.dll.GetFileVersionInfoSizeW")
#pragma comment(linker,"/export:GetFileVersionInfoSizeExA=C:\\Windows\\System32\\version.dll.GetFileVersionInfoSizeExA")
#pragma comment(linker,"/export:GetFileVersionInfoSizeExW=C:\\Windows\\System32\\version.dll.GetFileVersionInfoSizeExW")
#pragma comment(linker,"/export:VerFindFileA=C:\\Windows\\System32\\version.dll.VerFindFileA")
#pragma comment(linker,"/export:VerFindFileW=C:\\Windows\\System32\\version.dll.VerFindFileW")
#pragma comment(linker,"/export:VerInstallFileA=C:\\Windows\\System32\\version.dll.VerInstallFileA")
#pragma comment(linker,"/export:VerInstallFileW=C:\\Windows\\System32\\version.dll.VerInstallFileW")
#pragma comment(linker,"/export:VerLanguageNameA=C:\\Windows\\System32\\version.dll.VerLanguageNameA")
#pragma comment(linker,"/export:VerLanguageNameW=C:\\Windows\\System32\\version.dll.VerLanguageNameW")
#pragma comment(linker,"/export:VerQueryValueA=C:\\Windows\\System32\\version.dll.VerQueryValueA")
#pragma comment(linker,"/export:VerQueryValueW=C:\\Windows\\System32\\version.dll.VerQueryValueW")

DWORD WINAPI MainThread(HMODULE m);


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

typedef LONG(*_WinVerifyTrust)(
    HWND   hwnd,
    GUID* pgActionID,
    LPVOID pWVTData
    );

LONG DetourWinVerifyTrust(
    HWND   hwnd,
    GUID* pgActionID,
    LPVOID pWVTData
);

_WinVerifyTrust fpWinVerifyTrust;

DWORD WINAPI MainThread(HMODULE m)
{
    MH_STATUS error;
    if ((error = MH_Initialize()) != MH_OK)
    {
        return FALSE;
    }

    if ((error = MH_CreateHookApi(L"wintrust", "WinVerifyTrust", &DetourWinVerifyTrust, (LPVOID*)(&fpWinVerifyTrust))) != MH_OK)
    {
        return FALSE;
    }

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
    {
        return FALSE;
    }

    return TRUE;
}

LONG DetourWinVerifyTrust(
    HWND   hwnd,
    GUID* pgActionID,
    LPVOID pWVTData
) {
    MH_DisableHook(MH_ALL_HOOKS);
    return 0;
}