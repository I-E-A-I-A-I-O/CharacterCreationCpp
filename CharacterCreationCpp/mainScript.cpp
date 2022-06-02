#include "pch.h"
#include "mainScript.h"
#include "json.hpp"
#include "natives.h"
#include "characterCreationMenu.h"
#include "loadingMenu.h"
#include "outfitCreationMenu.h"
#include "utils.h"
#include "screen.h"
#include "enums.h"
#include "blip.h"

int GlobalData::PLAYER_ID;
bool GlobalData::swapped = false;
bool transisioning = false;
bool entering = false;
bool died = false;
Hash to_model = 0x705E61F2;
Blip creation_blip;
Blip outfit_blip;

void lock_player() {
	TASK::TASK_ACHIEVE_HEADING(GlobalData::PLAYER_ID, 270, 0);
	PLAYER::SET_PLAYER_CONTROL(0, 0, 1 << 8);
}

void unlock_player() {
	PLAYER::SET_PLAYER_CONTROL(0, 1, 0);
}

void prepare_creation(bool male) {
	lock_player();
	transisioning = true;

	if (male) to_model = 0x705E61F2;
	else to_model = 0x9C9EFFD8;
}

void leave_creation() {
	unlock_player();
	CHARACTERMENU::Data::creating = false;
	CHARACTERMENU::close();
}

void handle_creation_menu_opening() {
	if (!UTILS::can_open_creationmenu()) {

		if (CHARACTERMENU::isOpen()) {
			leave_creation();
		}

		return;
	}

	SCREEN::ShowHelpTextThisFrame_long("Press ~INPUT_CONTEXT~ to enter creation mode with MP Female.", "\nPress ~INPUT_CONTEXT_SECONDARY~ to enter creation mode with MP Male.", false);

	if (PAD::IS_CONTROL_JUST_PRESSED(0, 51)) {
		prepare_creation(false);
	}

	if (PAD::IS_CONTROL_JUST_PRESSED(0, 52)) {
		prepare_creation(true);
	}
}

void handle_loading_menu_opening() {
	if (!UTILS::can_open_loadingmenu()) {
		if (LOADINGMENU::isOpen()) {
			unlock_player();
			LOADINGMENU::close();
		}

		return;
	}

	if (PAD::IS_CONTROL_JUST_PRESSED(0, 316)) {
		lock_player();
		LOADINGMENU::open();
	}
}

void handle_outfitmenu_open() {
	if (!UTILS::can_open_outfitmenu() || !UTILS::is_freemode_character()) return;

	SCREEN::ShowHelpTextThisFrame_long("Press ~INPUT_CONTEXT~ to enter outfit creation.",  "\nPress ~INPUT_CONTEXT_SECONDARY~ to edit the current outfit.", false);

	if (PAD::IS_CONTROL_JUST_PRESSED(0, 51)) {
		lock_player();
		entering = true;
		OUTFITMENU::Data::reset = true;
	}

	if (PAD::IS_CONTROL_JUST_PRESSED(0, 52)) {
		lock_player();
		entering = true;
		OUTFITMENU::Data::reset = false;
	}
}

void creation_tick() {
	if (transisioning) {
		CAM::DO_SCREEN_FADE_OUT(2000);
		WAIT(4000);
		UTILS::loadModel(to_model);
		PLAYER::SET_PLAYER_MODEL(0, to_model);
		UTILS::unloadModel(to_model);
		GlobalData::PLAYER_ID = PLAYER::PLAYER_PED_ID();
		PED::SET_PED_DEFAULT_COMPONENT_VARIATION(GlobalData::PLAYER_ID);
		PED::SET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, true);
		CAM::DO_SCREEN_FADE_IN(2000);
		transisioning = false;
		GlobalData::swapped = true;

		if (LOADINGMENU::Data::loaded) LOADINGMENU::Data::loaded = false;

		CHARACTERMENU::Data::creating = true;
		CHARACTERMENU::open();
	}
	else if (CHARACTERMENU::Data::creating) {
		if (!CHARACTERMENU::isOpen()) {
			leave_creation();
		}
	}
}

void outfit_tick() {
	if (entering) {
		CAM::DO_SCREEN_FADE_OUT(2000);
		WAIT(4000);
		ENTITY::SET_ENTITY_COORDS(GlobalData::PLAYER_ID, -1188.65857f, -765.6327f, 16.3201351f, 1, 0, 0, 1);
		
		if (OUTFITMENU::Data::reset) {
			PED::SET_PED_DEFAULT_COMPONENT_VARIATION(GlobalData::PLAYER_ID);
			PED::CLEAR_ALL_PED_PROPS(GlobalData::PLAYER_ID);
			PED::SET_PED_EYE_COLOR_(GlobalData::PLAYER_ID, 1);
		}

		WAIT(2000);
		CAM::DO_SCREEN_FADE_IN(2000);
		WAIT(1500);
		entering = false;
		OUTFITMENU::Data::creating = true;
		OUTFITMENU::open();
	}
	else if (OUTFITMENU::Data::creating) {
		if (!OUTFITMENU::isOpen()) {
			CAM::DO_SCREEN_FADE_OUT(2000);
			WAIT(4000);
			ENTITY::SET_ENTITY_COORDS(GlobalData::PLAYER_ID, -1204.60474f, -780.3441f, 16.3322849f, 1, 0, 0, 1);
			WAIT(2000);
			CAM::DO_SCREEN_FADE_IN(2000);
			WAIT(1500);
			unlock_player();
			OUTFITMENU::Data::creating = false;
		}
	}
}

void model_watch_tick() {
	if (GlobalData::swapped) {
		if (!UTILS::is_freemode_character()) {
			GlobalData::swapped = false;
			LOADINGMENU::Data::loaded = false;
			return;
		}

		if (ENTITY::IS_ENTITY_DEAD(GlobalData::PLAYER_ID, 1)) {
			if (!UTILS::is_main_character_2()) {
				UTILS::loadModel(0xD7114C9);
				WAIT(2000);
				PLAYER::SET_PLAYER_MODEL(0, 0xD7114C9);
				UTILS::unloadModel(0xD7114C9);
				GlobalData::PLAYER_ID = PLAYER::PLAYER_PED_ID();
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(GlobalData::PLAYER_ID);
			}
			else died = true;
		}
		else if (died) {
			while (CAM::IS_SCREEN_FADED_OUT() || CAM::IS_SCREEN_FADING_OUT() || !CAM::IS_GAMEPLAY_CAM_RENDERING()) WAIT(0);
			if (LOADINGMENU::Data::loaded) LOADINGMENU::load_outfit(LOADINGMENU::Data::last_loaded);
			died = false;
		}
	}
}

int main() {
	LOADINGMENU::Initialize();
	CHARACTERMENU::Initialize();
	OUTFITMENU::Initialize();
	creation_blip = BLIPS::Create(-1042.3564f, -2745.46924f, 20.36439f, eBlipSprite::BlipSpriteClothes, eBlipColor::BlipColorGreen, "Character Creation");
	outfit_blip = BLIPS::Create(-1204.60474f, -780.3441f, 16.3322849f, eBlipSprite::BlipSpriteClothes, eBlipColor::BlipColorBlue, "Outfit Creation");

	for (;;) {
		GlobalData::PLAYER_ID = PLAYER::PLAYER_PED_ID();
		model_watch_tick();
		LOADINGMENU::OnTick();
		CHARACTERMENU::OnTick();
		OUTFITMENU::OnTick();
		handle_outfitmenu_open();
		handle_loading_menu_opening();
		handle_creation_menu_opening();
		creation_tick();
		outfit_tick();
		WAIT(0);
	}
}

void MainScriptMain() {
	srand(GetTickCount64());
	main();
}

void MainScriptAbort() {
	HUD::REMOVE_BLIP(&creation_blip);
	HUD::REMOVE_BLIP(&creation_blip);

	if (CHARACTERMENU::Data::creating) {
		unlock_player();
		CHARACTERMENU::Data::creating = false;
	}

	if (OUTFITMENU::Data::creating) {
		CAM::DO_SCREEN_FADE_OUT(2000);
		WAIT(4000);
		ENTITY::SET_ENTITY_COORDS(GlobalData::PLAYER_ID, -1204.60474f, -780.3441f, 16.3322849f, 1, 0, 0, 1);
		WAIT(2000);
		CAM::DO_SCREEN_FADE_IN(2000);
		WAIT(1500);
		unlock_player();
		OUTFITMENU::Data::creating = false;
		CAM::RENDER_SCRIPT_CAMS(0, 0, 3000, 0, 0, 0);
	}
}