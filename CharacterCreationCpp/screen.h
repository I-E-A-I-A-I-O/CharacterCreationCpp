#pragma once

#include <string>
#include <vector>

namespace SCREEN
{
	void ShowSubtitle(const char* text, int time);
	void ShowHelpTextThisFrame(const char* text, bool beep);
	void ShowHelpTextThisFrame_long(const char* text1, const char* text2, bool beep);
	void ShowNotification(const char* text, const char* subject);
	void ShowNotification(const char* text);
}