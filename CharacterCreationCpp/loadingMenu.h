#pragma once

#include "menu.h"

namespace LOADINGMENU {
	struct Data {
		static bool loaded;
		static std::string last_loaded;
	};

	void load_outfit(const std::string& outfit_name);
	void Initialize();
	void OnTick();
	bool isOpen();
	void open();
	void close();
}