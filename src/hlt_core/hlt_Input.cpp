#include "pch.h"
#include "hlt_Input.h"
#include <Windowsx.h>

hlt_Input::KeyboardInput* hlt_Input::KeyboardInput::s_pInstance = nullptr;

hlt_Input::KeyboardInput& hlt_Input::KeyboardInput::GetInstance()
{
    if (s_pInstance == nullptr)
    {
        s_pInstance = new hlt_Input::KeyboardInput();
    }
    return *s_pInstance;
}

void hlt_Input::KeyboardInput::Update()
{
    for (int i = 1; i < 255; i++)
    {
        if (GetAsyncKeyState(i) < 0)
        {
            switch (m_KeyStates[i])
            {
            case keyState::NONE:
            case keyState::UP:
                m_KeyStates[i] = keyState::DOWN; // On commence par DOWN
                break;
            case keyState::DOWN:
                m_KeyStates[i] = keyState::PUSH; // Puis on maintient en PUSH
                break;
            case keyState::PUSH:
                // On reste en PUSH tant que c'est enfoncé
                break;
            }
        }
        else
        {
            switch (m_KeyStates[i])
            {
            case keyState::DOWN:
            case keyState::PUSH:
                m_KeyStates[i] = keyState::UP; // On vient de relâcher
                break;
            case keyState::UP:
                m_KeyStates[i] = keyState::NONE; // Retour au repos
                break;
            case keyState::NONE:
                break;
            }
        }
    }
}

void hlt_Input::KeyboardInput::Reset()
{
    memset(&m_KeyStates, 0, 256);
}

bool hlt_Input::KeyboardInput::IsKey(int key)
{
    return m_KeyStates[key] == DOWN || m_KeyStates[key] == PUSH;
}

bool hlt_Input::KeyboardInput::IsKeyUp(int key)
{
    return m_KeyStates[key] == UP;
}

bool hlt_Input::KeyboardInput::IsKeyDown(int key)
{
    return m_KeyStates[key] == DOWN;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

hlt_Input::MouseInput* hlt_Input::MouseInput::s_pInstance = nullptr;

hlt_Input::MouseInput& hlt_Input::MouseInput::GetInstance()
{
    if (s_pInstance == nullptr)
    {
        s_pInstance = new hlt_Input::MouseInput();
    }
    return *s_pInstance;
}

void hlt_Input::MouseInput::Update()
{
    for (int i = 0; i < 5; i++)
    {
        if (GetAsyncKeyState(buttons[i]) < 0)
        {
            switch (m_ButtonStates[i])
            {
            case keyState::NONE:
            case keyState::UP:
                m_ButtonStates[i] = keyState::DOWN; // On commence par DOWN
                break;
            case keyState::DOWN:
                m_ButtonStates[i] = keyState::PUSH; // Puis on maintient en PUSH
                break;
            case keyState::PUSH:
                // On reste en PUSH tant que c'est enfoncé
                break;
            }
        }
        else
        {
            switch (m_ButtonStates[i])
            {
            case keyState::DOWN:
            case keyState::PUSH:
                m_ButtonStates[i] = keyState::UP; // On vient de relâcher
                break;
            case keyState::UP:
                m_ButtonStates[i] = keyState::NONE; // Retour au repos
                break;
            case keyState::NONE:
                break;
            }
        }
    }
}

void hlt_Input::MouseInput::Reset()
{
    memset(&m_ButtonStates, 0, 5);
}

bool hlt_Input::MouseInput::IsKey(int key)
{
    return m_ButtonStates[key] == DOWN || m_ButtonStates[key] == PUSH;
}

bool hlt_Input::MouseInput::IsKeyDown(int key)
{
    return m_ButtonStates[key] == UP;
}

bool hlt_Input::MouseInput::IsKeyUp(int key)
{
    return m_ButtonStates[key] == DOWN;
}

void hlt_Input::MouseInput::MouseMove(LPARAM lParam)
{
    m_LastPos = m_Pos;
    m_Pos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
}
