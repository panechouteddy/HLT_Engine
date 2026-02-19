#pragma once
#include <windows.h>
#include <cstdio>
#include <fcntl.h>
#include <io.h>
#include <iostream>

// Console helper dans l'unité de traduction
namespace hlt_DebugTools
{
    namespace hlt_DebugConsole
    {
        void CreateDebugConsole();

        void DestroyDebugConsole();
    }
}