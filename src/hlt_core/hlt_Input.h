#pragma once

namespace hlt_Input
{
	class KeyboardInput
	{
	private:
		enum keyState
		{
			NONE,
			DOWN,
			UP,
			PUSH
		};

	private:
		static KeyboardInput* s_pInstance;

		unsigned char m_KeyStates[256];

	public:
		KeyboardInput() { Reset(); }
		~KeyboardInput() = default;
		static KeyboardInput& GetInstance();

		virtual void Start() { Reset(); }
		void Update();

		void Reset();
		bool IsKey(int key);
		bool IsKeyDown(int key);
		bool IsKeyUp(int key);
	};

	class MouseInput
	{
	private:
		enum keyState
		{
			NONE,
			DOWN,
			UP,
			PUSH
		};

	private:
		static MouseInput* s_pInstance;

		static constexpr int buttons[5] = {
			VK_LBUTTON,
			VK_RBUTTON,
			VK_MBUTTON,
			VK_XBUTTON1,
			VK_XBUTTON2
		};
		unsigned char m_ButtonStates[5];
		float m_MouseWheelDelta = 0.f;
		XMINT2 m_Pos = { 0, 0 };
		XMINT2 m_LastPos = { 0, 0 };
		
	public:
		MouseInput() { Reset(); }
		~MouseInput() = default;
		static MouseInput& GetInstance();

		virtual void Start() { Reset(); }
		void Update();

		void Reset();
		bool IsKey(int key);
		bool IsKeyDown(int key);
		bool IsKeyUp(int key);

		void SetMouseMove(LPARAM& lParam);
		void SetMouseWheel(WPARAM& wParam);
		XMINT2* GetPos() { return &m_Pos; }
		XMINT2* GetLastPos() { return &m_LastPos; }
		XMINT2 GetDeltaPos() { return XMINT2(m_Pos.x - m_LastPos.x, m_Pos.y - m_LastPos.y); }
	};

	class ControllerInput
	{
	private:
		enum keyState
		{
			NONE,
			DOWN,
			UP,
			PUSH
		};
	};
}
