#include "pch.h"
#include "outfitCreationMenu.h"
#include "natives.h"
#include "utils.h"
#include "mainScript.h"
#include "nlohmann/json.hpp"
#include "screen.h"
#include "characterData.h"
#include <filesystem>
#include <fstream>
#include <vector>

NativeMenu::Menu omenu;
bool OUTFITMENU::Data::creating = false;
bool OUTFITMENU::Data::reset = false;
CharacterData current_character = CharacterData();
Cam shoe_cam;
Cam face_cam;
Cam leg_cam;

void OnOutfitMain() {
	omenu.ReadSettings();
	face_cam = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 0);
	leg_cam = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 0);
	shoe_cam = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 0);
	AUDIO::PLAY_SOUND_FRONTEND(-1, (char*)"CONFIRM_BEEP", (char*)"HUD_MINI_GAME_SOUNDSET", true);
}

void reset_outfit_values() {
	current_character = CharacterData();
	OUTFITMENU::outfit_data.outfit_max_hairstyles = 0;
	OUTFITMENU::outfit_data.outfit_max_haircolors = 0;
}

void OnOutfitExit() {
	CAM::DESTROY_CAM(face_cam, 0);
	CAM::DESTROY_CAM(leg_cam, 0);
	CAM::DESTROY_CAM(shoe_cam, 0);
	reset_outfit_values();
	PLAYER::SET_PLAYER_CONTROL(0, 1, 0);
}

bool OUTFITMENU::isOpen() {
	return omenu.IsThisOpen();
}

void OUTFITMENU::open() {
	if (OUTFITMENU::Data::reset) reset_outfit_values();

	omenu.OpenMenu();
}

void OUTFITMENU::close() {
	omenu.CloseMenu();
}

void OUTFITMENU::Initialize() {
	omenu.RegisterOnMain(std::bind(OnOutfitMain));
	omenu.RegisterOnExit(std::bind(OnOutfitExit));
	omenu.ReadSettings();
}

void faceshot_start()
{
	Hash player_model = ENTITY::GET_ENTITY_MODEL(GlobalData::PLAYER_ID);
	Vector3 dimensions = Vector3();
	Vector3 ignore = Vector3();
	Vector3 cam_offset = Vector3();
	cam_offset.x = 0.1f;
	//cam_offset.y = 0.4f;
	cam_offset.y = 0.5f;
	Vector3 bone_coords = PED::GET_PED_BONE_COORDS(GlobalData::PLAYER_ID, eBone::SKEL_Head, cam_offset);
	Vector3 cam_look_coords = Vector3();
	MISC::GET_MODEL_DIMENSIONS(player_model, &dimensions, &ignore);
	dimensions.x = 0;
	dimensions.z = 0;
	Vector3 rear_position = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(GlobalData::PLAYER_ID, dimensions);
	cam_look_coords.x = rear_position.x;
	cam_look_coords.y = rear_position.y;
	cam_look_coords.z = rear_position.z + 0.7f;
	CAM::SET_CAM_COORD(face_cam, bone_coords);
	CAM::POINT_CAM_AT_COORD(face_cam, cam_look_coords);
	CAM::SET_CAM_ACTIVE(face_cam, true);
	CAM::RENDER_SCRIPT_CAMS(1, 1, 1000, 1, 0, 0);
}

void faceshot_end()
{
	if (!CAM::IS_CAM_ACTIVE(face_cam)) return;

	CAM::SET_CAM_ACTIVE(face_cam, false);
	CAM::RENDER_SCRIPT_CAMS(0, 1, 1000, 1, 0, 0);
}

void legcam_start() {
	Hash player_model = ENTITY::GET_ENTITY_MODEL(GlobalData::PLAYER_ID);
	Vector3 dimensions = Vector3();
	Vector3 ignore = Vector3();
	Vector3 cam_offset = Vector3();
	cam_offset.x = 0.35f;
	//cam_offset.y = 1.0f;
	cam_offset.y = 0.9f;
	Vector3 bone_coords = PED::GET_PED_BONE_COORDS(GlobalData::PLAYER_ID, eBone::SKEL_Pelvis, cam_offset);
	Vector3 cam_look_coords = Vector3();
	MISC::GET_MODEL_DIMENSIONS(player_model, &dimensions, &ignore);
	dimensions.x = 0;
	dimensions.z = 0;
	Vector3 rear_position = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(GlobalData::PLAYER_ID, dimensions);
	cam_look_coords.x = rear_position.x;
	cam_look_coords.y = rear_position.y;
	cam_look_coords.z = rear_position.z - 0.35f;
	CAM::SET_CAM_COORD(leg_cam, bone_coords);
	CAM::POINT_CAM_AT_COORD(leg_cam, cam_look_coords);
	CAM::SET_CAM_ACTIVE(leg_cam, true);
	CAM::RENDER_SCRIPT_CAMS(1, 1, 1000, 1, 0, 0);
}

void legcam_end()
{
	if (!CAM::IS_CAM_ACTIVE(leg_cam)) return;

	CAM::SET_CAM_ACTIVE(leg_cam, false);
	CAM::RENDER_SCRIPT_CAMS(0, 1, 1000, 1, 0, 0);
}

void shoecam_start() {
	Hash player_model = ENTITY::GET_ENTITY_MODEL(GlobalData::PLAYER_ID);
	Vector3 dimensions = Vector3();
	Vector3 ignore = Vector3();
	Vector3 cam_offset = Vector3();
	cam_offset.x = 0.5f;
	//cam_offset.y = 1.0f;
	cam_offset.y = 0.8f;
	Vector3 bone_coords = PED::GET_PED_BONE_COORDS(GlobalData::PLAYER_ID, eBone::SKEL_Pelvis, cam_offset);
	Vector3 cam_look_coords = Vector3();
	MISC::GET_MODEL_DIMENSIONS(player_model, &dimensions, &ignore);
	dimensions.x = 0;
	dimensions.z = 0;
	Vector3 rear_position = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(GlobalData::PLAYER_ID, dimensions);
	cam_look_coords.x = rear_position.x;
	cam_look_coords.y = rear_position.y;
	cam_look_coords.z = rear_position.z - 0.8f;
	CAM::SET_CAM_COORD(shoe_cam, bone_coords);
	CAM::POINT_CAM_AT_COORD(shoe_cam, cam_look_coords);
	CAM::SET_CAM_ACTIVE(shoe_cam, true);
	CAM::RENDER_SCRIPT_CAMS(1, 1, 1000, 1, 0, 0);
}

void shoecam_end()
{
	if (!CAM::IS_CAM_ACTIVE(shoe_cam)) return;

	CAM::SET_CAM_ACTIVE(shoe_cam, false);
	CAM::RENDER_SCRIPT_CAMS(0, 1, 1000, 1, 0, 0);
}

void set_outfit_hair_values() {
	OUTFITMENU::outfit_data.outfit_max_hairstyles = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 2);
	OUTFITMENU::outfit_data.outfit_max_haircolors = PED::GET_NUM_HAIR_COLORS_();
}

void set_outfit_max_drawables() {
	OUTFITMENU::outfit_data.max_mask_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 1);
	OUTFITMENU::outfit_data.max_torso_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 3) - 1;
	OUTFITMENU::outfit_data.max_leg_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 4) - 2;
	OUTFITMENU::outfit_data.max_bag_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 5) - 1;
	OUTFITMENU::outfit_data.max_shoe_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 6) - 1;
	OUTFITMENU::outfit_data.max_accessory_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 7) - 1;
	OUTFITMENU::outfit_data.max_undershirt_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 8) - 2;
	OUTFITMENU::outfit_data.max_armor_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 9) - 1;
	OUTFITMENU::outfit_data.max_badge_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 10) - 1;
	OUTFITMENU::outfit_data.max_torso2_drawble = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 11) - 2;
	OUTFITMENU::outfit_data.max_hat_type = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 0);
	OUTFITMENU::outfit_data.max_glasses_type = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 1);
	OUTFITMENU::outfit_data.max_ear_type = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 2);
	OUTFITMENU::outfit_data.max_watch_type = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 6);
	OUTFITMENU::outfit_data.max_bracelet_type = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 7);
}

void update_outfit_hairmenu() {
	omenu.Title("Hair");
	omenu.Subtitle("Customize your hairstyle.");

	if (omenu.IntOption("Hairstyle", current_character.outfit_selected_hairstyle, 0, OUTFITMENU::outfit_data.outfit_max_hairstyles)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, current_character.outfit_selected_hairstyle, 0, 0);
			PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, current_character.outfit_selected_haircolor, current_character.outfit_selected_highlight);
		}
	}

	if (omenu.IntOption("Hair color", current_character.outfit_selected_haircolor, 0, OUTFITMENU::outfit_data.outfit_max_haircolors)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, current_character.outfit_selected_hairstyle, 0, 0);
			PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, current_character.outfit_selected_haircolor, current_character.outfit_selected_highlight);
		}
	}

	if (omenu.IntOption("Hightlight color", current_character.outfit_selected_highlight, 0, OUTFITMENU::outfit_data.outfit_max_haircolors)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, current_character.outfit_selected_hairstyle, 0, 0);
			PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, current_character.outfit_selected_haircolor, current_character.outfit_selected_highlight);
		}
	}
}

void update_outfit_blushmenu() {
	omenu.Title("Blush");
	omenu.Subtitle("Customize your blush.");

	if (omenu.BoolOption("Has blush", current_character.outfit_has_blush)) {
		if (OUTFITMENU::Data::creating) {
			if (!current_character.outfit_has_blush) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, 255, 1);
			}
		}
	}

	if (omenu.IntOption("Type", current_character.outfit_selected_blush_type, 0, 6)) {
		if (OUTFITMENU::Data::creating) {
			current_character.outfit_has_blush = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, current_character.outfit_selected_blush_type, current_character.outfit_selected_blush_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 5, 2, current_character.outfit_selected_blush_color, current_character.outfit_selected_blush_color);
		}
	}

	if (omenu.IntOption("Color", current_character.outfit_selected_blush_color, 0, 63)) {
		if (OUTFITMENU::Data::creating) {
			current_character.outfit_has_blush = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, current_character.outfit_selected_blush_type, current_character.outfit_selected_blush_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 5, 2, current_character.outfit_selected_blush_color, current_character.outfit_selected_blush_color);
		}
	}

	if (omenu.FloatOption("Opacity", current_character.outfit_selected_blush_opacity, 0.0f, 1.0f)) {
		if (OUTFITMENU::Data::creating) {
			current_character.outfit_has_blush = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, current_character.outfit_selected_blush_type, current_character.outfit_selected_blush_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 5, 2, current_character.outfit_selected_blush_color, current_character.outfit_selected_blush_color);
		}
	}
}

void update_outfit_lipstickmenu() {
	omenu.Title("Lipstick");
	omenu.Subtitle("Customize your lipstick.");

	if (omenu.BoolOption("Has lipstick", current_character.outfit_has_lipstick)) {
		if (OUTFITMENU::Data::creating) {
			if (!current_character.outfit_has_lipstick) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, 255, 1);
			}
		}
	}

	if (omenu.IntOption("Type", current_character.outfit_selected_lipstick_type, 0, 9)) {
		if (OUTFITMENU::Data::creating) {
			current_character.outfit_has_lipstick = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, current_character.outfit_selected_lipstick_type, current_character.outfit_selected_lipstick_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 8, 2, current_character.outfit_selected_lipstick_color, current_character.outfit_selected_lipstick_color);
		}
	}

	if (omenu.IntOption("Color", current_character.outfit_selected_lipstick_color, 0, 63)) {
		if (OUTFITMENU::Data::creating) {
			current_character.outfit_has_lipstick = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, current_character.outfit_selected_lipstick_type, current_character.outfit_selected_lipstick_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 8, 2, current_character.outfit_selected_lipstick_color, current_character.outfit_selected_lipstick_color);
		}
	}

	if (omenu.FloatOption("Opacity", current_character.outfit_selected_lipstick_opacity, 0.0f, 1.0f)) {
		if (OUTFITMENU::Data::creating) {
			current_character.outfit_has_lipstick = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, current_character.outfit_selected_lipstick_type, current_character.outfit_selected_lipstick_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 8, 2, current_character.outfit_selected_lipstick_color, current_character.outfit_selected_lipstick_color);
		}
	}
}

void update_outfit_makeupmenu() {
	omenu.Title("Makeup");
	omenu.Subtitle("Customize your makeup.");

	if (omenu.BoolOption("Has makeup", current_character.outfit_has_makeup)) {
		if (OUTFITMENU::Data::creating) {
			if (!current_character.outfit_has_makeup) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, 255, 1);
			}
		}
	}

	if (omenu.IntOption("Style", current_character.outfit_selected_makeup_type, 0, 74)) {
		if (OUTFITMENU::Data::creating) {
			current_character.outfit_has_makeup = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, current_character.outfit_selected_makeup_type, current_character.outfit_selected_makeup_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 4, 2, current_character.outfit_selected_makeup_color, current_character.outfit_selected_makeup_color);
		}
	}

	if (omenu.IntOption("Color", current_character.outfit_selected_makeup_color, 0, 63)) {
		if (OUTFITMENU::Data::creating) {
			current_character.outfit_has_makeup = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, current_character.outfit_selected_makeup_type, current_character.outfit_selected_makeup_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 4, 2, current_character.outfit_selected_makeup_color, current_character.outfit_selected_makeup_color);
		}
	}

	if (omenu.FloatOption("Opacity", current_character.outfit_selected_makeup_opacity, 0.0f, 1.0f)) {
		if (OUTFITMENU::Data::creating) {
			current_character.outfit_has_makeup = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, current_character.outfit_selected_makeup_type, current_character.outfit_selected_makeup_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 4, 2, current_character.outfit_selected_makeup_color, current_character.outfit_selected_makeup_color);
		}
	}
}

void update_maskmenu() {
	omenu.Title("Mask");
	omenu.Subtitle("Change your mask.");
	
	if (omenu.IntOption("Type", current_character.mask_drawable, 0, OUTFITMENU::outfit_data.max_mask_drawable)) {
		current_character.mask_texture = 0;
		OUTFITMENU::outfit_data.max_mask_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 1, current_character.mask_drawable);

		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 1, current_character.mask_drawable, 0, 0);
		}
	}

	if (omenu.IntOption("Color", current_character.mask_texture, 0, OUTFITMENU::outfit_data.max_mask_texture)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 1, current_character.mask_drawable, current_character.mask_texture, 0);
		}
	}
}

void update_legmenu() {
	omenu.Title("Lower Body");
	omenu.Subtitle("Change lower body.");

	if (omenu.IntOption("Type", current_character.leg_drawable, 0, OUTFITMENU::outfit_data.max_leg_drawable)) {
		current_character.leg_texture = 0;
		OUTFITMENU::outfit_data.max_leg_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 4, current_character.leg_drawable);

		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 4, current_character.leg_drawable, 0, 0);
		}
	}

	if (omenu.IntOption("Color", current_character.leg_texture, 0, OUTFITMENU::outfit_data.max_leg_texture)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 4, current_character.leg_drawable, current_character.leg_texture, 0);
		}
	}
}

void update_bagmenu() {
	omenu.Title("Bags & Parachute");
	omenu.Subtitle("Change Bags & Parachute.");

	if (omenu.IntOption("Type", current_character.bag_drawable, 0, OUTFITMENU::outfit_data.max_bag_drawable)) {
		current_character.bag_texture = 0;
		OUTFITMENU::outfit_data.max_bag_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 5, current_character.bag_drawable);

		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 5, current_character.bag_drawable, 0, 0);
		}
	}

	if (omenu.IntOption("Color", current_character.bag_texture, 0, OUTFITMENU::outfit_data.max_bag_texture)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 5, current_character.bag_drawable, current_character.bag_texture, 0);
		}
	}
}

void update_shoemenu() {
	omenu.Title("Shoes");
	omenu.Subtitle("Change your shoes.");

	if (omenu.IntOption("Type", current_character.shoe_drawable, 0, OUTFITMENU::outfit_data.max_shoe_drawable)) {
		current_character.shoe_texture = 0;
		OUTFITMENU::outfit_data.max_shoe_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 6, current_character.shoe_drawable);

		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 6, current_character.shoe_drawable, 0, 0);
		}
	}

	if (omenu.IntOption("Color", current_character.shoe_texture, 0, OUTFITMENU::outfit_data.max_shoe_texture)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 6, current_character.shoe_drawable, current_character.shoe_texture, 0);
		}
	}
}

void update_accessorymenu() {
	omenu.Title("Accessory");
	omenu.Subtitle("Change your accessory.");

	if (omenu.IntOption("Type", current_character.accessory_drawable, 0, OUTFITMENU::outfit_data.max_accessory_drawable)) {
		current_character.accessory_texture = 0;
		OUTFITMENU::outfit_data.max_accessory_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 7, current_character.accessory_drawable);

		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 7, current_character.accessory_drawable, 0, 0);
		}
	}

	if (omenu.IntOption("Color", current_character.accessory_texture, 0, OUTFITMENU::outfit_data.max_accessory_texture)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 7, current_character.accessory_drawable, current_character.accessory_texture, 0);
		}
	}
}

void update_undermenu() {
	omenu.Title("Undershirt");
	omenu.Subtitle("Change your undershirt.");

	if (omenu.IntOption("Type", current_character.undershirt_drawable, 0, OUTFITMENU::outfit_data.max_undershirt_drawable)) {
		current_character.undershirt_texture = 0;
		OUTFITMENU::outfit_data.max_undershirt_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 8, current_character.undershirt_drawable);

		if (OUTFITMENU::Data::creating) PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 8, current_character.undershirt_drawable, 0, 0);
	}

	if (omenu.IntOption("Color", current_character.undershirt_texture, 0, OUTFITMENU::outfit_data.max_undershirt_texture)) {
		if (OUTFITMENU::Data::creating) PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 8, current_character.undershirt_drawable, current_character.undershirt_texture, 0);
	}
}

void update_armormenu() {
	omenu.Title("Armor");
	omenu.Subtitle("Change your armor.");

	if (omenu.IntOption("Type", current_character.armor_drawable, 0, OUTFITMENU::outfit_data.max_armor_drawable)) {
		current_character.armor_texture = 0;
		OUTFITMENU::outfit_data.max_armor_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 9, current_character.armor_drawable);

		if (OUTFITMENU::Data::creating) PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 9, current_character.armor_drawable, 0, 0);
	}

	if (omenu.IntOption("Color", current_character.armor_texture, 0, OUTFITMENU::outfit_data.max_armor_texture)) {
		if (OUTFITMENU::Data::creating) PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 9, current_character.armor_drawable, current_character.armor_texture, 0);
	}
}

void update_uppermenu() {
	omenu.Title("Upper Body");
	omenu.Subtitle("Change upper body.");

	if (omenu.IntOption("Type", current_character.torso2_drawable, 0, OUTFITMENU::outfit_data.max_torso2_drawble)) {
		current_character.torso2_texture = 0;
		OUTFITMENU::outfit_data.max_torso2_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 11, current_character.torso2_drawable);

		if (OUTFITMENU::Data::creating) PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 11, current_character.torso2_drawable, 0, 0);
	}

	if (omenu.IntOption("Color", current_character.torso2_texture, 0, OUTFITMENU::outfit_data.max_torso2_texture)) {
		if (OUTFITMENU::Data::creating) PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 11, current_character.torso2_drawable, current_character.torso2_texture, 0);
	}
}

void update_hatmenu() {
	omenu.Title("Hat");
	omenu.Subtitle("Change your hat.");

	if (omenu.BoolOption("Has hat", current_character.has_hat)) {
		if (OUTFITMENU::Data::creating && !current_character.has_hat) PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 0);
	}

	if (omenu.IntOption("Type", current_character.hat_type, 0, OUTFITMENU::outfit_data.max_hat_type)) {
		current_character.hat_color = 0;
		OUTFITMENU::outfit_data.max_hat_color = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 0, current_character.hat_type);

		if (OUTFITMENU::Data::creating) {
			current_character.has_hat = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 0, current_character.hat_type, 0, 1);
		}
	}

	if (omenu.IntOption("Color", current_character.hat_color, 0, OUTFITMENU::outfit_data.max_hat_color)) {
		if (OUTFITMENU::Data::creating) {
			current_character.has_hat = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 0, current_character.hat_type, current_character.hat_color, 1);
		}
	}
}

void update_glassmenu() {
	omenu.Title("Glasses");
	omenu.Subtitle("Change your glasses");

	if (omenu.BoolOption("Has glasses", current_character.has_glasses)) {
		if (OUTFITMENU::Data::creating && !current_character.has_glasses) PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 1);
	}

	if (omenu.IntOption("Type", current_character.glasses_type, 0, OUTFITMENU::outfit_data.max_glasses_type)) {
		current_character.glasses_color = 0;
		OUTFITMENU::outfit_data.max_glasses_color = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 1, current_character.glasses_type);

		if (OUTFITMENU::Data::creating) {
			current_character.has_glasses = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 1, current_character.glasses_type, 0, 1);
		}
	}

	if (omenu.IntOption("Color", current_character.glasses_color, 0, OUTFITMENU::outfit_data.max_glasses_color)) {
		if (OUTFITMENU::Data::creating) {
			current_character.has_glasses = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 1, current_character.glasses_type, current_character.glasses_color, 1);
		}
	}
}

void update_earmenu() {
	omenu.Title("Ear Accessory");
	omenu.Subtitle("Change your ear accessory");

	if (omenu.BoolOption("Has ear accessory", current_character.has_ear)) {
		if (OUTFITMENU::Data::creating && !current_character.has_ear) PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 2);
	}

	if (omenu.IntOption("Type", current_character.ear_type, 0, OUTFITMENU::outfit_data.max_ear_type)) {
		current_character.ear_color = 0;
		OUTFITMENU::outfit_data.max_ear_color = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 2, current_character.ear_type);

		if (OUTFITMENU::Data::creating) {
			current_character.has_ear = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 2, current_character.ear_type, 0, 1);
		}
	}

	if (omenu.IntOption("Color", current_character.ear_color, 0, OUTFITMENU::outfit_data.max_ear_color)) {
		if (OUTFITMENU::Data::creating) {
			current_character.has_ear = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 2, current_character.ear_type, current_character.ear_color, 1);
		}
	}
}

void update_watchmenu() {
	omenu.Title("Watch");
	omenu.Subtitle("Change your watch");

	if (omenu.BoolOption("Has watch", current_character.has_watch)) {
		if (OUTFITMENU::Data::creating && !current_character.has_watch) PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 6);
	}

	if (omenu.IntOption("Type", current_character.watch_type, 0, OUTFITMENU::outfit_data.max_watch_type)) {
		current_character.watch_color = 0;
		OUTFITMENU::outfit_data.max_watch_color = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 6, current_character.watch_type);

		if (OUTFITMENU::Data::creating) {
			current_character.has_watch = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 6, current_character.watch_type, 0, 1);
		}
	}

	if (omenu.IntOption("Color", current_character.watch_color, 0, OUTFITMENU::outfit_data.max_watch_color)) {
		if (OUTFITMENU::Data::creating) {
			current_character.has_watch = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 6, current_character.watch_type, current_character.watch_color, 1);
		}
	}
}

void update_braceletmenu() {
	omenu.Title("Bracelet");
	omenu.Subtitle("Change your bracelet");

	if (omenu.BoolOption("Has bracelet", current_character.has_bracelet)) {
		if (OUTFITMENU::Data::creating && !current_character.has_bracelet) PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 7);
	}

	if (omenu.IntOption("Type", current_character.bracelet_type, 0, OUTFITMENU::outfit_data.max_bracelet_type)) {
		current_character.bracelet_color = 0;
		OUTFITMENU::outfit_data.max_bracelet_color = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 7, current_character.bracelet_type);

		if (OUTFITMENU::Data::creating) {
			current_character.has_bracelet = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 7, current_character.bracelet_type, 0, 1);
		}
	}

	if (omenu.IntOption("Color", current_character.bracelet_color, 0, OUTFITMENU::outfit_data.max_bracelet_color)) {
		if (OUTFITMENU::Data::creating) {
			current_character.has_bracelet = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 7, current_character.bracelet_type, current_character.bracelet_color, 1);
		}
	}
}

void update_eyemenu() {
	omenu.Title("Eyes");
	omenu.Subtitle("Customize your eyes");

	if (omenu.IntOption("Eye color", current_character.eye_color, 0, 29)) {
		if (OUTFITMENU::Data::creating) PED::SET_PED_EYE_COLOR_(GlobalData::PLAYER_ID, current_character.eye_color);
	}
}

void update_clothesmenu() {
	omenu.Title("Outfit Customization");
	omenu.Subtitle("Customize your outfit.");

	if (omenu.MenuOption("Eyes", "eyemenu", { "Customize your eyes." })) {
		faceshot_start();
		OUTFITMENU::outfit_data.face_cam_lock = true;
	}

	if (omenu.MenuOption("Hair", "hairmenu", { "Change your hairstyle." })) {
		set_outfit_hair_values();
	}

	if (omenu.MenuOption("Blush", "blushmenu", { "Change your blush." })) {
		faceshot_start();
		OUTFITMENU::outfit_data.face_cam_lock = true;
	}

	if (omenu.MenuOption("Lipstick", "lipstickmenu", { "Change your lipstick." })) {
		faceshot_start();
		OUTFITMENU::outfit_data.face_cam_lock = true;
	}

	if (omenu.MenuOption("Makeup", "makeupmenu", { "Change your makeup." })) {
		faceshot_start();
		OUTFITMENU::outfit_data.face_cam_lock = true;
	}

	if (omenu.MenuOption("Mask", "maskmenu", { "Change your mask." })) {
		faceshot_start();
		OUTFITMENU::outfit_data.face_cam_lock = true;
	}
	
	if (omenu.IntOption("Torso", current_character.torso_drawable, 0, OUTFITMENU::outfit_data.max_torso_drawable, 1, { "Change your torso." })) {
		if (OUTFITMENU::Data::creating) PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 3, current_character.torso_drawable, 0, 0);
	}

	if (omenu.MenuOption("Lower body", "legmenu", { "Change lower body." })) {
		legcam_start();
		OUTFITMENU::outfit_data.leg_cam_lock = true;
	}

	omenu.MenuOption("Bags & Parachute", "bagmenu", { "Change Bags & Parachute." });

	if (omenu.MenuOption("Shoes", "shoemenu", { "Change your shoes." })) {
		shoecam_start();
		OUTFITMENU::outfit_data.shoe_cam_lock = true;
	}

	omenu.MenuOption("Accessory", "accessorymenu", { "Change your accessory." });
	omenu.MenuOption("Undershirt", "undermenu", { "Change your undershirt." });
	omenu.MenuOption("Armor", "armormenu", { "Change your armor." });

	if (omenu.IntOption("Badge", current_character.badge_drawable, 0, OUTFITMENU::outfit_data.max_badge_drawable, 1, { "Change your badge." })) {
		if (OUTFITMENU::Data::creating) PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 10, current_character.badge_drawable, 0, 0);
	}

	omenu.MenuOption("Upper body", "uppermenu", { "Change upper body." });
	omenu.MenuOption("Hat", "hatmenu", { "Change your hat." });

	if (omenu.MenuOption("Glasess", "glassmenu", { "Change your glasess." })) {
		faceshot_start();
		OUTFITMENU::outfit_data.face_cam_lock = true;
	}

	omenu.MenuOption("Ear accessory", "earmenu", { "Change your ear accessory." });
	omenu.MenuOption("Watch", "watchmenu", { "Change your watch." });
	omenu.MenuOption("Bracelet", "braceletmenu", { "Change your bracelet." });
}

void save_outfit() {
	MISC::DISPLAY_ONSCREEN_KEYBOARD(1, "FMMC_KEY_TIP9", nullptr, "Name", nullptr, nullptr, nullptr, 21);

	while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);

	const char* name = MISC::GET_ONSCREEN_KEYBOARD_RESULT();

	if (name == NULL) {
		SCREEN::ShowNotification("~r~Save canceled.");
		return;
	}

	std::string filepath = "CharacterCreationData\\Outfits\\";
	filepath = filepath.append(name) + ".json";
	nlohmann::json j;
	j["eyecolor"] = current_character.eye_color;
	j["hair"]["style"] = current_character.outfit_selected_hairstyle;
	j["hair"]["color"] = current_character.outfit_selected_haircolor;
	j["hair"]["highlight"] = current_character.outfit_selected_highlight;
	j["blush"]["enabled"] = current_character.outfit_has_blush;
	j["blush"]["type"] = current_character.outfit_selected_blush_type;
	j["blush"]["color"] = current_character.outfit_selected_blush_color;
	j["blush"]["opacity"] = current_character.outfit_selected_blush_opacity;
	j["lipstick"]["enabled"] = current_character.outfit_has_lipstick;
	j["lipstick"]["type"] = current_character.outfit_selected_lipstick_type;
	j["lipstick"]["color"] = current_character.outfit_selected_lipstick_color;
	j["lipstick"]["opacity"] = current_character.outfit_selected_lipstick_opacity;
	j["makeup"]["enabled"] = current_character.outfit_has_makeup;
	j["makeup"]["type"] = current_character.outfit_selected_makeup_type;
	j["makeup"]["color"] = current_character.outfit_selected_makeup_color;
	j["makeup"]["opacity"] = current_character.outfit_selected_makeup_opacity;
	j["mask"]["drawable"] = current_character.mask_drawable;
	j["mask"]["texture"] = current_character.mask_texture;
	j["leg"]["drawable"] = current_character.leg_drawable;
	j["leg"]["texture"] = current_character.leg_texture;
	j["bag"]["drawable"] = current_character.bag_drawable;
	j["bag"]["texture"] = current_character.bag_texture;
	j["shoe"]["drawable"] = current_character.shoe_drawable;
	j["shoe"]["texture"] = current_character.shoe_texture;
	j["accessory"]["drawable"] = current_character.accessory_drawable;
	j["accessory"]["texture"] = current_character.accessory_texture;
	j["torso"]["drawable"] = current_character.torso_drawable;
	j["torso2"]["drawable"] = current_character.torso2_drawable;
	j["torso2"]["texture"] = current_character.torso2_texture;
	j["undershirt"]["drawable"] = current_character.undershirt_drawable;
	j["undershirt"]["texture"] = current_character.undershirt_texture;
	j["armor"]["drawable"] = current_character.armor_drawable;
	j["armor"]["texture"] = current_character.armor_texture;
	j["hat"]["enabled"] = current_character.has_hat;
	j["hat"]["drawable"] = current_character.hat_type;
	j["hat"]["texture"] = current_character.hat_color;
	j["glasses"]["enabled"] = current_character.has_glasses;
	j["glasses"]["drawable"] = current_character.glasses_type;
	j["glasses"]["texture"] = current_character.glasses_color;
	j["ear"]["enabled"] = current_character.has_ear;
	j["ear"]["drawable"] = current_character.ear_type;
	j["ear"]["texture"] = current_character.ear_color;
	j["watch"]["enabled"] = current_character.has_watch;
	j["watch"]["drawable"] = current_character.watch_type;
	j["watch"]["texture"] = current_character.watch_color;
	j["bracelet"]["enabled"] = current_character.has_bracelet;
	j["bracelet"]["drawable"] = current_character.bracelet_type;
	j["bracelet"]["texture"] = current_character.bracelet_color;
	j["badge"] = current_character.badge_drawable;
	std::ofstream o(filepath);
	o << std::setw(4) << j << std::endl;
	SCREEN::ShowNotification("~g~Outfit saved!");
}

void update_outfitmenu() {
	omenu.Title("OUTFIT Creation");
	omenu.Subtitle("Create a new outfit.");

	if (omenu.MenuOption("Customization", "clothesmenu", { "Customize your outfit." })) {
		set_outfit_max_drawables();
	}

	if (omenu.Option("Save outfit")) {
		save_outfit();
	}
}

void OUTFITMENU::OnTick() {
	omenu.CheckKeys();
	OUTFITMENU::outfit_data.face_cam_lock = false;
	OUTFITMENU::outfit_data.leg_cam_lock = false;
	OUTFITMENU::outfit_data.shoe_cam_lock = false;

	if (omenu.CurrentMenu("mainmenu")) {
		update_outfitmenu();
	}
	else if (omenu.CurrentMenu("clothesmenu")) {
		update_clothesmenu();
	}
	else if (omenu.CurrentMenu("hairmenu")) {
		update_outfit_hairmenu();
	}
	else if (omenu.CurrentMenu("blushmenu")) {
		update_outfit_blushmenu();
		OUTFITMENU::outfit_data.face_cam_lock = true;
	}
	else if (omenu.CurrentMenu("lipstickmenu")) {
		update_outfit_lipstickmenu();
		OUTFITMENU::outfit_data.face_cam_lock = true;
	}
	else if (omenu.CurrentMenu("makeupmenu")) {
		update_outfit_makeupmenu();
		OUTFITMENU::outfit_data.face_cam_lock = true;
	}
	else if (omenu.CurrentMenu("maskmenu")) {
		update_maskmenu();
		OUTFITMENU::outfit_data.face_cam_lock = true;
	}
	else if (omenu.CurrentMenu("legmenu")) {
		update_legmenu();
		OUTFITMENU::outfit_data.leg_cam_lock = true;
	}
	else if (omenu.CurrentMenu("bagmenu")) {
		update_bagmenu();
	}
	else if (omenu.CurrentMenu("shoemenu")) {
		update_shoemenu();
		OUTFITMENU::outfit_data.shoe_cam_lock = true;
	}
	else if (omenu.CurrentMenu("accessorymenu")) {
		update_accessorymenu();
	}
	else if (omenu.CurrentMenu("undermenu")) {
		update_undermenu();
	}
	else if (omenu.CurrentMenu("armormenu")) {
		update_armormenu();
	}
	else if (omenu.CurrentMenu("uppermenu")) {
		update_uppermenu();
	}
	else if (omenu.CurrentMenu("hatmenu")) {
		update_hatmenu();
	}
	else if (omenu.CurrentMenu("glassmenu")) {
		update_glassmenu();
		OUTFITMENU::outfit_data.face_cam_lock = true;
	}
	else if (omenu.CurrentMenu("earmenu")) {
		update_earmenu();
	}
	else if (omenu.CurrentMenu("watchmenu")) {
		update_watchmenu();
	}
	else if (omenu.CurrentMenu("braceletmenu")) {
		update_braceletmenu();
	}
	else if (omenu.CurrentMenu("eyemenu")) {
		update_eyemenu();
		OUTFITMENU::outfit_data.face_cam_lock = true;
	}

	if (!OUTFITMENU::outfit_data.face_cam_lock) faceshot_end();
	if (!OUTFITMENU::outfit_data.leg_cam_lock) legcam_end();
	if (!OUTFITMENU::outfit_data.shoe_cam_lock) shoecam_end();

	omenu.EndMenu();
}