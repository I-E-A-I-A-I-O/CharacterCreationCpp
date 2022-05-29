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
Hash to_model = 0x705E61F2;
Hash playerzero = 0xD7114C9;
Hash playerone = 0x9B22DBAF;
Hash playertwo = 0x9B810FA2;
//Cam creation_camera;
Blip creation_blip;
Blip outfit_blip;

bool is_main_character() {
	return PED::IS_PED_MODEL(GlobalData::PLAYER_ID, 0xD7114C9) || PED::IS_PED_MODEL(GlobalData::PLAYER_ID, 0x9B22DBAF) || PED::IS_PED_MODEL(GlobalData::PLAYER_ID, 0x9B810FA2);
}

bool is_main_character_2() {
	Hash player_model = ENTITY::GET_ENTITY_MODEL(GlobalData::PLAYER_ID);
	return player_model == playerzero || player_model == playerone || player_model == playertwo;
}

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
	/*CAM::RENDER_SCRIPT_CAMS(0, 0, 3000, 0, 0, 0);
	CAM::DESTROY_CAM(creation_camera, 1);*/
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
	if (!UTILS::can_open_outfitmenu()) {
		/*if (OUTFITMENU::isOpen()) {
			unlock_player();
			OUTFITMENU::close();
		}*/
		return;
	}

	SCREEN::ShowHelpTextThisFrame("Press ~INPUT_CONTEXT~ to enter outfit creation.", false);

	if (PAD::IS_CONTROL_JUST_PRESSED(0, 51)) {
		lock_player();
		entering = true;
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
		/*creation_camera = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 0);
		Vector3 dimensions = Vector3();
		Vector3 ignore = Vector3();
		MISC::GET_MODEL_DIMENSIONS(to_model, &dimensions, &ignore);
		dimensions.x = 0;
		dimensions.z = 0;
		Vector3 rear_position = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(GlobalData::PLAYER_ID, dimensions);
		rear_position.z -= 0.8;
		Vector3 camera_offset = Vector3();
		camera_offset.x = 0;
		camera_offset.y = 0.5;
		camera_offset.z = 0.1;
		rear_position.z += 0.7;
		CAM::ATTACH_CAM_TO_PED_BONE(creation_camera, GlobalData::PLAYER_ID, eBone::SKEL_Head, camera_offset, 1);
		CAM::POINT_CAM_AT_COORD(creation_camera, rear_position);
		CAM::SET_CAM_ACTIVE(creation_camera, 1);
		CAM::RENDER_SCRIPT_CAMS(1, 1, 3000, 0, 0, 0);*/
		CAM::DO_SCREEN_FADE_IN(2000);
		transisioning = false;
		GlobalData::swapped = true;
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

}

int main() {
	LOADINGMENU::Initialize();
	CHARACTERMENU::Initialize();
	OUTFITMENU::Initialize();
	creation_blip = BLIPS::Create(-1042.3564f, -2745.46924f, 20.36439f, eBlipSprite::BlipSpriteClothes, eBlipColor::BlipColorGreen, "Character Creation");
	outfit_blip = BLIPS::Create(-1204.60474f, -780.3441f, 16.3322849f, eBlipSprite::BlipSpriteClothes, eBlipColor::BlipColorBlue, "Outfit Creation");

	for (;;) {
		GlobalData::PLAYER_ID = PLAYER::PLAYER_PED_ID();
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
		//CAM::RENDER_SCRIPT_CAMS(0, 0, 3000, 0, 0, 0);
		//CAM::DESTROY_CAM(creation_camera, 0);
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
	}
}