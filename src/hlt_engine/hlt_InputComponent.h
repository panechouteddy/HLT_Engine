#pragma once
#include <vector>
#include <hlt_core/hlt_Input.h>
#include <DirectXMath.h>

namespace hlt_Component
{
	namespace hlt_Input
	{
		struct hlt_Keyboard
		{
		public:
			static const int ID = 2;

			std::vector<int> keys;

			std::vector<bool> isDown;
			std::vector<bool> isUp;
			std::vector<bool> isKey;
		};

		struct hlt_Mouse
		{
		public:
			static const int ID = 3;

			std::vector<int> keys;

			std::vector<bool> isDown;
			std::vector<bool> isUp;
			std::vector<bool> isKey;
			
			DirectX::XMINT2* m_Pos = ::hlt_Input::MouseInput::GetInstance().GetPos();
			DirectX::XMINT2* m_LastPos = ::hlt_Input::MouseInput::GetInstance().GetLastPos();
		};
	}
}