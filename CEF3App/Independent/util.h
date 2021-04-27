#pragma once

#include <string>

#include "include/base/cef_build.h"

namespace cef_util
{
#if defined(OS_LINUX)
    #include <stdlib.h>
    #include <unistd.h>

    std::string GetAppPath()
    {
        std::string path = "";
        const pid_t pid = getpid();
        char buf[20] = {0};

        sprintf(buf, "%d", pid);
        std::string _link = "/proc/";
        _link.append(buf);
        _link.append("/exe");
        char proc[512];
        const int ch = readlink(_link.c_str(), proc, 512);
        if (ch != -1) 
        {
            proc[ch] = 0;
            path = proc;
            const std::string::size_type t = path.find_last_of("/");
            path = path.substr(0, t);
        }

        return path;
    }
#elif defined(OS_WIN)
    #include <windows.h>

    std::string GetAppPath()
    {
        HMODULE hModule = GetModuleHandleW(NULL);
        WCHAR   wpath[MAX_PATH];

        GetModuleFileNameW(hModule, wpath, MAX_PATH);
        std::wstring wide(wpath);

        std::string path = CefString(wide);
        path = path.substr(0, path.find_last_of("\\/"));
        return path;
    }
#endif
}