#pragma once

#include "menu.h"

namespace CHARACTERMENU {
	enum class eMenuMode {
		all,
		shape,
		cosmetic
	};

	struct Data {
		static bool creating;
		static eMenuMode mode;
	};

	void Initialize();
	void OnTick();
	bool isOpen();
	void open();
	void close();
}