#pragma once

#include "menu.h"

namespace LOADINGMENU {
	void Initialize();
	void OnTick();
	bool isOpen();
	void open();
	void close();
}