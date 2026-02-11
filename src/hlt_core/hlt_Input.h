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
		KeyboardInput() = default;
		~KeyboardInput() = default;
		static KeyboardInput& GetInstance();

		virtual void OnStart() { Reset(); }
		void OnUpdate(float& dt);

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
		XMFLOAT2 m_Pos = { 0.f, 0.f };

	public:
		MouseInput() = default;
		~MouseInput() = default;
		static MouseInput& GetInstance();

		virtual void OnStart() { Reset(); }
		void OnUpdate(float& dt);

		void Reset();
		bool IsKey(int key);
		bool IsKeyDown(int key);
		bool IsKeyUp(int key);
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

