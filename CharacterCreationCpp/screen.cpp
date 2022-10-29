#include "pch.h"
#include "screen.h"
#include "natives.hpp"
#include <string>

void SCREEN::ShowHelpTextThisFrame(const char* text, bool beep)
{
	HUD::BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	HUD::END_TEXT_COMMAND_DISPLAY_HELP(0, 0, beep, 0);
}

void SCREEN::ShowHelpTextThisFrame_long(const char* text1, const char* text2, bool beep)
{
	HUD::BEGIN_TEXT_COMMAND_DISPLAY_HELP("TWOSTRINGS");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text1);
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text2);
	HUD::END_TEXT_COMMAND_DISPLAY_HELP(0, 0, beep, 0);
}

void SCREEN::ShowSubtitle(const char* text, int time)
{
	HUD::BEGIN_TEXT_COMMAND_PRINT("STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	HUD::END_TEXT_COMMAND_PRINT(time, true);
}

/*void SCREEN::ShowNotification(const char* text, const char* subject)
{
	HUD::BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)text);
	HUD::SET_NOTIFICATION_MESSAGE_((char*)"CHAR_LESTER", "CHAR_LESTER", false, 1, false, "Lester", subject);
	HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(false, true);
}*/

void SCREEN::ShowNotification(const char* text)
{
	HUD::BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(false, true);
}