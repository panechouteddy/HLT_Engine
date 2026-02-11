#include "pch.h"

void hlt_DebugTools::hlt_DebugConsole::CreateDebugConsole()
{
    if (AllocConsole())
    {
        // Redirige les flux C vers la console
        FILE* fp = nullptr;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$", "r", stdin);

        std::cin.clear();  // Efface les flags d'erreur (EOF/FAIL)
        std::cout.clear();
        std::cerr.clear();

        // Synchronise les iostream C++ avec les flux C
        std::ios::sync_with_stdio(true);

        // UTF-8 si besoin
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleTitleA("Debug Console");
    }
}

void hlt_DebugTools::hlt_DebugConsole::DestroyDebugConsole()
{
    // Vide et ferme les flux avant de libérer la console
    std::fflush(stdout);
    std::fflush(stderr);
    // Note : fclose peut échouer si les flux ont déjà été fermés, mais c'est en général acceptable ici.
    std::fclose(stdout);
    std::fclose(stderr);
    std::fclose(stdin);
    FreeConsole();
}
