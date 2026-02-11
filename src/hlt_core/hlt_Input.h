#pragma once

#define VK_Z 0x5A
#define VK_Q 0x51
#define VK_S 0x53
#define VK_D 0x44

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
		float m_ScrollDelta = 0.f;
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

		void MouseMove(UINT uMsg, LPARAM lParam);
		XMINT2* GetPos() { return &m_Pos; }
		XMINT2* GetLastPos() { return &m_LastPos; }
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
