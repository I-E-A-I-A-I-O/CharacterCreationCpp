#include "pch.h"
#include "blip.h"

void BLIPS::SetBlipName(Blip blip, const char* name)
{
	HUD::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(name);
	HUD::END_TEXT_COMMAND_SET_BLIP_NAME(blip);
}

Blip BLIPS::Create(float x, float y, float z, int sprite, eBlipColor color, const char* name) {
	Blip blipHandle = HUD::ADD_BLIP_FOR_COORD(x, y, z);
	HUD::SET_BLIP_SPRITE(blipHandle, sprite);
	HUD::SET_BLIP_COLOUR(blipHandle, color);
	SetBlipName(blipHandle, name);
	HUD::SET_BLIP_AS_SHORT_RANGE(blipHandle, true);
	return blipHandle;
}