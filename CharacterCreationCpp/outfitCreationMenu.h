#pragma once

#include "menu.h"

namespace OUTFITMENU {
	struct Data {
		static bool creating;
	};

	void Initialize();
	void OnTick();
	bool isOpen();
	void open();
	void close();
}