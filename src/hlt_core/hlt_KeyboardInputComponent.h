#pragma once
#include <vector>

namespace hlt_Component
{
	struct hlt_KeyboardInputComponent
	{
	public:
		static const int ID = 2;

		std::vector<int> keys;

		std::vector<bool> isDown;
		std::vector<bool> isUp;
		std::vector<bool> isKey;
	};
}