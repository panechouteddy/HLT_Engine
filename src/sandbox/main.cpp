#include "pch.h"
#include "main.h"


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
    Test_Inputs();
    
    return 0;
}

// La fameuse WndProc que vous n'aviez pas
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_MOUSEMOVE:
        // On envoie le mouvement à votre instance existante
        hlt_Input::MouseInput::GetInstance().MouseMove(message);
        break;

    case WM_MOUSEWHEEL:
        // On capture le scroll (à ajouter dans votre classe MouseInput)
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        // hlt_Input::MouseInput::GetInstance().SetScrollDelta(delta); 
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
    // 1. Enregistrement de la classe de fenêtre
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"HLT_Engine", NULL };
    RegisterClassEx(&wc);

    // 2. Création de la fenêtre (nécessaire pour recevoir les messages souris/scroll)
    HWND hwnd = CreateWindow(wc.lpszClassName, L"HLT Engine Test", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 720, NULL, NULL, wc.hInstance, NULL);

    ShowWindow(hwnd, cmdShow);
    UpdateWindow(hwnd);

    // 3. Lancement de votre test
    Test_Inputs();

    UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 0;
}