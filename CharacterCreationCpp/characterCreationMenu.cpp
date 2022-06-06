#include "pch.h"
#include "characterCreationMenu.h"
#include "natives.h"
#include "utils.h"
#include "nlohmann/json.hpp"
#include "mainScript.h"
#include "screen.h"
#include "characterData.h"
#include <fstream>
#include <map>

bool CHARACTERMENU::Data::creating = false;
CHARACTERMENU::eMenuMode CHARACTERMENU::Data::mode = CHARACTERMENU::eMenuMode::all;
NativeMenu::Menu menu;
ShapeData current_shape = ShapeData();
Cam face_camera;
int max_hairstyles;
int max_haircolors;
bool face_lock = false;
int selected_head_tat;
int selected_torso_tat;
int selected_larm_tat;
int selected_rarm_tat;
int selected_lleg_tat;
int selected_rleg_tat;
bool tattoo_lock_out;
bool tattoo_lock_in;

std::map<std::string, std::vector<Tattoo>> tattoos = {
	{ "ZONE_HEAD", std::vector<Tattoo>() },
	{ "ZONE_LEFT_ARM", std::vector<Tattoo>() },
	{ "ZONE_RIGHT_ARM", std::vector<Tattoo>() },
	{ "ZONE_LEFT_ARM", std::vector<Tattoo>() },
	{ "ZONE_LEFT_LEG", std::vector<Tattoo>() },
	{ "ZONE_TORSO", std::vector<Tattoo>() }
};

void OnMain() {
	face_camera = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 0);
	menu.ReadSettings();
	AUDIO::PLAY_SOUND_FRONTEND(-1, (char*)"CONFIRM_BEEP", (char*)"HUD_MINI_GAME_SOUNDSET", true);
}

void reset_items() {
	max_hairstyles = 0;
	max_haircolors = 0;
	selected_head_tat = 0;
	selected_torso_tat = 0;
	selected_larm_tat = 0;
	selected_rarm_tat = 0;
	selected_lleg_tat = 0;
	selected_rleg_tat = 0;

	if (CHARACTERMENU::Data::mode == CHARACTERMENU::eMenuMode::all) current_shape = ShapeData();
}

void reset_tattoo_lists() {
	tattoos["ZONE_HEAD"].clear();
	tattoos["ZONE_LEFT_ARM"].clear();
	tattoos["ZONE_RIGHT_ARM"].clear();
	tattoos["ZONE_RIGHT_LEG"].clear();
	tattoos["ZONE_LEFT_LEG"].clear();
	tattoos["ZONE_TORSO"].clear();
}

void refresh_tattoos() {
	PED::CLEAR_PED_DECORATIONS_LEAVE_SCARS(GlobalData::PLAYER_ID);

	if (current_shape.head_tattoo.collection != 0) {
		PED::ADD_PED_DECORATION_FROM_HASHES(GlobalData::PLAYER_ID, current_shape.head_tattoo.collection, current_shape.head_tattoo.overlay);
	}

	if (current_shape.torso_tattoo.collection != 0) {
		PED::ADD_PED_DECORATION_FROM_HASHES(GlobalData::PLAYER_ID, current_shape.torso_tattoo.collection, current_shape.torso_tattoo.overlay);
	}

	if (current_shape.larm_tattoo.collection != 0) {
		PED::ADD_PED_DECORATION_FROM_HASHES(GlobalData::PLAYER_ID, current_shape.larm_tattoo.collection, current_shape.larm_tattoo.overlay);
	}

	if (current_shape.rarm_tattoo.collection != 0) {
		PED::ADD_PED_DECORATION_FROM_HASHES(GlobalData::PLAYER_ID, current_shape.rarm_tattoo.collection, current_shape.rarm_tattoo.overlay);
	}

	if (current_shape.rleg_tattoo.collection != 0) {
		PED::ADD_PED_DECORATION_FROM_HASHES(GlobalData::PLAYER_ID, current_shape.rleg_tattoo.collection, current_shape.rleg_tattoo.overlay);
	}

	if (current_shape.lleg_tattoo.collection != 0) {
		PED::ADD_PED_DECORATION_FROM_HASHES(GlobalData::PLAYER_ID, current_shape.lleg_tattoo.collection, current_shape.lleg_tattoo.overlay);
	}
}

void load_tattoos() {
	std::ifstream i("CharacterCreationData\\pedOverlayCollections.json");
	nlohmann::json j;
	i >> j;
	i.close();

	reset_tattoo_lists();

	for (auto& collection : j.items()) {
		nlohmann::json col = collection.value();
		Hash collection_hash = col.at("CollectionHash");

		for (auto& overlay : col.at("Overlays").items()) {
			nlohmann::json ov = overlay.value();
			std::string type = ov.at("Type");

			if (type != "TYPE_TATTOO") continue;

			eGender character_gender = UTILS::get_gender();
			std::string gender = ov.at("Gender");

			if ((gender == "GENDER_FEMALE" && character_gender != eGender::GenderFemale) || (gender == "GENDER_MALE" && character_gender != eGender::GenderMale) || gender == "GENDER_DONTCARE") continue;

			std::string zone = ov.at("ZoneName");

			if (ov.find("UpdateGroup") == ov.end()) continue;

			std::string updateGroup = ov.at("UpdateGroup");

			if (updateGroup == "hairOverlay") continue;

			Hash overlay_hash = ov.at("OverlayHash");
			Tattoo tat = Tattoo();
			tat.collection = collection_hash;
			tat.overlay = overlay_hash;
			tat.zone = zone;
			tat.update_group = updateGroup;
			tattoos[zone].push_back(tat);
		}
	}
}

void OnExit() {
	CAM::DESTROY_CAM(face_camera, 0);
	reset_items();
}

bool CHARACTERMENU::isOpen() {
	return menu.IsThisOpen();
}

void CHARACTERMENU::open() {
	load_tattoos();
	menu.OpenMenu();
}

void CHARACTERMENU::close() {
	menu.CloseMenu();
}

void CHARACTERMENU::Initialize() {
	menu.RegisterOnMain(std::bind(OnMain));
	menu.RegisterOnExit(std::bind(OnExit));
	menu.ReadSettings();
}

void save_values() {
	MISC::DISPLAY_ONSCREEN_KEYBOARD(1, "FMMC_KEY_TIP9", nullptr, "Name", nullptr, nullptr, nullptr, 21);

	while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);

	const char* name = MISC::GET_ONSCREEN_KEYBOARD_RESULT();

	if (name == NULL) {
		SCREEN::ShowNotification("~r~Save canceled.");
		return;
	}

	std::string filepath = "CharacterCreationData\\Characters\\";
	filepath = filepath.append(name) + ".json";
	nlohmann::json j;
	j["model"] = ENTITY::GET_ENTITY_MODEL(GlobalData::PLAYER_ID);
	j["features"]["shape_mother"] = current_shape.shape_mother;
	j["features"]["shape_father"] = current_shape.shape_father;
	j["features"]["skin_mother"] = current_shape.skin_mother;
	j["features"]["skin_father"] = current_shape.skin_father;
	j["features"]["shape_mix"] = current_shape.shape_mix;
	j["features"]["skin_mix"] = current_shape.skin_mix;
	j["features"]["nose_width"] = current_shape.nose_width;
	j["features"]["nose_peak_height"] = current_shape.nose_peak_height;
	j["features"]["nose_peak_length"] = current_shape.nose_peak_length;
	j["features"]["nose_bone_height"] = current_shape.nose_bone_height;
	j["features"]["nose_peak_lowering"] = current_shape.nose_peak_lowering;
	j["features"]["nose_bone_twist"] = current_shape.nose_bone_twist;
	j["features"]["eyebrow_height"] = current_shape.eyebrow_height;
	j["features"]["eyebrow_forward"] = current_shape.eyebrow_forward;
	j["features"]["cheekbone_height"] = current_shape.cheekbone_height;
	j["features"]["cheekbone_width"] = current_shape.cheekbone_width;
	j["features"]["cheeks_width"] = current_shape.cheeks_width;
	j["features"]["eyes_opening"] = current_shape.eyes_opening;
	j["features"]["lips_thickness"] = current_shape.lips_thickness;
	j["features"]["jawbone_width"] = current_shape.jawbone_width;
	j["features"]["jawbone_back_length"] = current_shape.jawbone_back_length;
	j["features"]["chinbone_lowering"] = current_shape.chinbone_lowering;
	j["features"]["chinbone_length"] = current_shape.chinbone_length;
	j["features"]["chinbone_width"] = current_shape.chinbone_width;
	j["features"]["chin_hole"] = current_shape.chinbone_hole;
	j["features"]["neck_thickness"] = current_shape.neck_thickness;
	j["hair"]["style"] = current_character.outfit_selected_hairstyle;
	j["hair"]["color"] = current_character.outfit_selected_haircolor;
	j["hair"]["highlight"] = current_character.outfit_selected_highlight;
	j["eyebrows"]["enabled"] = current_shape.has_eyebrows;
	j["eyebrows"]["type"] = current_shape.selected_eyebrow_type;
	j["eyebrows"]["color"] = current_shape.selected_eyebrow_color;
	j["beard"]["enabled"] = current_shape.has_beard;
	j["beard"]["type"] = current_shape.selected_beard_type;
	j["beard"]["color"] = current_shape.selected_beard_color;
	j["chesthair"]["enabled"] = current_shape.has_chesthair;
	j["chesthair"]["type"] = current_shape.selected_chesthair_type;
	j["chesthair"]["color"] = current_shape.selected_chesthair_color;
	j["blush"]["enabled"] = current_character.outfit_has_blush;
	j["blush"]["type"] = current_character.outfit_selected_blush_type;
	j["blush"]["color"] = current_character.outfit_selected_blush_color;
	j["blush"]["opacity"] = current_character.outfit_selected_blush_opacity;
	j["lipstick"]["enabled"] = current_character.outfit_has_lipstick;
	j["lipstick"]["type"] = current_character.outfit_selected_lipstick_type;
	j["lipstick"]["color"] = current_character.outfit_selected_lipstick_color;
	j["lipstick"]["opacity"] = current_character.outfit_selected_lipstick_opacity;
	j["blemish"]["enabled"] = current_shape.has_blemish;
	j["blemish"]["type"] = current_shape.selected_blemish;
	j["aging"]["enabled"] = current_shape.has_aging;
	j["aging"]["age"] = current_shape.selected_age;
	j["makeup"]["enabled"] = current_character.outfit_has_makeup;
	j["makeup"]["type"] = current_character.outfit_selected_makeup_type;
	j["makeup"]["color"] = current_character.outfit_selected_makeup_color;
	j["makeup"]["opacity"] = current_character.outfit_selected_makeup_opacity;
	j["complexion"]["enabled"] = current_shape.has_complexion;
	j["complexion"]["type"] = current_shape.selected_complexion;
	j["sundamage"]["enabled"] = current_shape.has_sundamage;
	j["sundamage"]["type"] = current_shape.selected_sundamage;
	j["molesfreckles"]["enabled"] = current_shape.has_moles;
	j["molesfreckles"]["type"] = current_shape.selected_moles;
	j["tattoos"]["head"]["collection"] = current_shape.head_tattoo.collection;
	j["tattoos"]["head"]["overlay"] = current_shape.head_tattoo.overlay;
	j["tattoos"]["head"]["zone"] = current_shape.head_tattoo.zone;
	j["tattoos"]["head"]["update_group"] = current_shape.head_tattoo.update_group;
	j["tattoos"]["torso"]["collection"] = current_shape.torso_tattoo.collection;
	j["tattoos"]["torso"]["overlay"] = current_shape.torso_tattoo.overlay;
	j["tattoos"]["torso"]["zone"] = current_shape.torso_tattoo.zone;
	j["tattoos"]["torso"]["update_group"] = current_shape.torso_tattoo.update_group;
	j["tattoos"]["leftArm"]["collection"] = current_shape.larm_tattoo.collection;
	j["tattoos"]["leftArm"]["overlay"] = current_shape.larm_tattoo.overlay;
	j["tattoos"]["leftArm"]["zone"] = current_shape.larm_tattoo.zone;
	j["tattoos"]["leftArm"]["update_group"] = current_shape.larm_tattoo.update_group;
	j["tattoos"]["rightArm"]["collection"] = current_shape.rarm_tattoo.collection;
	j["tattoos"]["rightArm"]["overlay"] = current_shape.rarm_tattoo.overlay;
	j["tattoos"]["rightArm"]["zone"] = current_shape.rarm_tattoo.zone;
	j["tattoos"]["rightArm"]["update_group"] = current_shape.rarm_tattoo.update_group;
	j["tattoos"]["rightLeg"]["collection"] = current_shape.rleg_tattoo.collection;
	j["tattoos"]["rightLeg"]["overlay"] = current_shape.rleg_tattoo.overlay;
	j["tattoos"]["rightLeg"]["zone"] = current_shape.rleg_tattoo.zone;
	j["tattoos"]["rightLeg"]["update_group"] = current_shape.rleg_tattoo.update_group;
	j["tattoos"]["leftLeg"]["collection"] = current_shape.lleg_tattoo.collection;
	j["tattoos"]["leftLeg"]["overlay"] = current_shape.lleg_tattoo.overlay;
	j["tattoos"]["leftLeg"]["zone"] = current_shape.lleg_tattoo.zone;
	j["tattoos"]["leftLeg"]["update_group"] = current_shape.lleg_tattoo.update_group;
	std::ofstream o(filepath);
	o << std::setw(4) << j << std::endl;
	SCREEN::ShowNotification("~g~Character saved!");
}

void facecam_start()
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
	CAM::SET_CAM_COORD(face_camera, bone_coords);
	CAM::POINT_CAM_AT_COORD(face_camera, cam_look_coords);
	CAM::SET_CAM_ACTIVE(face_camera, true);
	CAM::RENDER_SCRIPT_CAMS(1, 1, 1000, 1, 0, 0);
}

void facecam_end()
{
	if (!CAM::IS_CAM_ACTIVE(face_camera)) return;

	CAM::SET_CAM_ACTIVE(face_camera, false);
	CAM::RENDER_SCRIPT_CAMS(0, 1, 1000, 1, 0, 0);
}

void set_hair_values() {
	max_hairstyles = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 2);
	max_haircolors = PED::GET_NUM_HAIR_COLORS_();
}

void update_mainmenu() {
	menu.Title("Character creation");

	menu.MenuOption("Customization", "customization", { "Customize your character." });

	if (menu.Option("Save character")) {
		save_values();
	}
}

void update_shapemenu() {
	menu.Title("Shape");
	menu.Subtitle("Customize your face features.");

	if (menu.IntOption("Shape mother", current_shape.shape_mother, 0, 45)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, current_shape.shape_mother, current_shape.shape_father, 0, current_shape.skin_mother, current_shape.skin_father, 0, current_shape.shape_mix, current_shape.skin_mix, 0, 0);
		}
	}

	if (menu.IntOption("Shape father", current_shape.shape_father, 0, 45)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, current_shape.shape_mother, current_shape.shape_father, 0, current_shape.skin_mother, current_shape.skin_father, 0, current_shape.shape_mix, current_shape.skin_mix, 0, 0);
		}
	}

	if (menu.IntOption("Skin mother", current_shape.skin_mother, 0, 45)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, current_shape.shape_mother, current_shape.shape_father, 0, current_shape.skin_mother, current_shape.skin_father, 0, current_shape.shape_mix, current_shape.skin_mix, 0, 0);
		}
	}

	if (menu.IntOption("Skin father", current_shape.skin_father, 0, 45)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, current_shape.shape_mother, current_shape.shape_father, 0, current_shape.skin_mother, current_shape.skin_father, 0, current_shape.shape_mix, current_shape.skin_mix, 0, 0);
		}
	}

	if (menu.FloatOption("Shape mix", current_shape.shape_mix, 0.00f, 1.00f, 0.01f, { "Set which parent head shape has more influence over your ped's head shape. Values go from Mother to Father." })) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, current_shape.shape_mother, current_shape.shape_father, 0, current_shape.skin_mother, current_shape.skin_father, 0, current_shape.shape_mix, current_shape.skin_mix, 0, 0);
		}
	}

	if (menu.FloatOption("Skin mix", current_shape.skin_mix, 0.00f, 1.00f, 0.01f, { "Set which parent skin color has more influence over your ped's skin color. Values go from Mother to Father." })) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, current_shape.shape_mother, current_shape.shape_father, 0, current_shape.skin_mother, current_shape.skin_father, 0, current_shape.shape_mix, current_shape.skin_mix, 0, 0);
		}
	}

	if (menu.FloatOption("Nose width", current_shape.nose_width, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 0, current_shape.nose_width);
		}
	}

	if (menu.FloatOption("Nose peak height", current_shape.nose_peak_height, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 1, current_shape.nose_peak_height);
		}
	}

	if (menu.FloatOption("Nose peak length", current_shape.nose_peak_length, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 2, current_shape.nose_peak_length);
		}
	}

	if (menu.FloatOption("Nose bone height", current_shape.nose_bone_height, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 3, current_shape.nose_bone_height);
		}
	}

	if (menu.FloatOption("Nose peak lowering", current_shape.nose_peak_lowering, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 4, current_shape.nose_peak_lowering);
		}
	}

	if (menu.FloatOption("Nose bone twist", current_shape.nose_bone_twist, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 5, current_shape.nose_bone_twist);
		}
	}

	if (menu.FloatOption("Eyebrow height", current_shape.eyebrow_height, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 6, current_shape.eyebrow_height);
		}
	}

	if (menu.FloatOption("Eyebrow forward", current_shape.eyebrow_forward, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 7, current_shape.eyebrow_forward);
		}
	}

	if (menu.FloatOption("Cheekbone height", current_shape.cheekbone_height, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 8, current_shape.cheekbone_height);
		}
	}

	if (menu.FloatOption("Cheekbone width", current_shape.cheekbone_width, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 9, current_shape.cheekbone_width);
		}
	}

	if (menu.FloatOption("Cheeks width", current_shape.cheeks_width, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 10, current_shape.cheeks_width);
		}
	}

	if (menu.FloatOption("Eye opening", current_shape.eyes_opening, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 11, current_shape.eyes_opening);
		}
	}

	if (menu.FloatOption("Lip thickness", current_shape.lips_thickness, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 12, current_shape.lips_thickness);
		}
	}

	if (menu.FloatOption("Jawbone width", current_shape.jawbone_width, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 13, current_shape.jawbone_width);
		}
	}

	if (menu.FloatOption("Jawbone back length", current_shape.jawbone_back_length, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 14, current_shape.jawbone_back_length);
		}
	}

	if (menu.FloatOption("Chinbone lowering", current_shape.chinbone_lowering, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 15, current_shape.chinbone_lowering);
		}
	}

	if (menu.FloatOption("Chinbone length", current_shape.chinbone_length, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 16, current_shape.chinbone_length);
		}
	}

	if (menu.FloatOption("Chinbone width", current_shape.chinbone_width, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 17, current_shape.chinbone_width);
		}
	}

	if (menu.FloatOption("Chin hole", current_shape.chinbone_hole, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 18, current_shape.chinbone_hole);
		}
	}

	if (menu.FloatOption("Neck thickness", current_shape.neck_thickness, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 19, current_shape.neck_thickness);
		}
	}
}

void update_hairmenu() {
	menu.Title("Hair");
	menu.Subtitle("Customize your hairstyle.");

	if (menu.IntOption("Hairstyle", current_character.outfit_selected_hairstyle, 0, max_hairstyles)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, current_character.outfit_selected_hairstyle, 0, 0);
			PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, current_character.outfit_selected_haircolor, current_character.outfit_selected_highlight);
		}
	}

	if (menu.IntOption("Hair color", current_character.outfit_selected_haircolor, 0, max_haircolors)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, current_character.outfit_selected_hairstyle, 0, 0);
			PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, current_character.outfit_selected_haircolor, current_character.outfit_selected_highlight);
		}
	}

	if (menu.IntOption("Hightlight color", current_character.outfit_selected_highlight, 0, max_haircolors)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, current_character.outfit_selected_hairstyle, 0, 0);
			PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, current_character.outfit_selected_haircolor, current_character.outfit_selected_highlight);
		}
	}
}

void update_eyebrowmenu() {
	menu.Title("Eyebrows");
	menu.Subtitle("Customize your eyebrows.");

	if (menu.BoolOption("Has eyebrows", current_shape.has_eyebrows)) {
		if (CHARACTERMENU::Data::creating) {
			if (!current_shape.has_eyebrows) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 2, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", current_shape.selected_eyebrow_type, 0, 33)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.has_eyebrows = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 2, current_shape.selected_eyebrow_type, 1);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 2, 1, current_shape.selected_eyebrow_color, current_shape.selected_eyebrow_color);
		}
	}

	if (menu.IntOption("Color", current_shape.selected_eyebrow_color, 0, 63)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.has_eyebrows = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 2, current_shape.selected_eyebrow_type, 1);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 2, 1, current_shape.selected_eyebrow_color, current_shape.selected_eyebrow_color);
		}
	}
}

void update_beardmenu() {
	menu.Title("Beard");
	menu.Subtitle("Customize your beard.");

	if (menu.BoolOption("Has beard", current_shape.has_beard)) {
		if (CHARACTERMENU::Data::creating) {
			if (!current_shape.has_beard) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 1, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", current_shape.selected_beard_type, 0, 28)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.has_beard = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 1, current_shape.selected_beard_type, 1);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 1, 1, current_shape.selected_beard_color, current_shape.selected_beard_color);
		}
	}

	if (menu.IntOption("Color", current_shape.selected_beard_color, 0, 63)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.has_beard = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 1, current_shape.selected_beard_type, 1);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 1, 1, current_shape.selected_beard_color, current_shape.selected_beard_color);
		}
	}
}

void update_chestmenu() {
	menu.Title("Chest Hair");
	menu.Subtitle("Customize your chest hair.");

	if (menu.BoolOption("Has chest hair", current_shape.has_chesthair)) {
		if (CHARACTERMENU::Data::creating) {
			if (!current_shape.has_chesthair) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 10, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", current_shape.selected_chesthair_type, 0, 16)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.has_chesthair = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 10, current_shape.selected_chesthair_type, 1);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 10, 1, current_shape.selected_chesthair_color, current_shape.selected_chesthair_color);
		}
	}

	if (menu.IntOption("Color", current_shape.selected_chesthair_color, 0, 63)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.has_chesthair = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 10, current_shape.selected_chesthair_type, 1);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 10, 1, current_shape.selected_chesthair_color, current_shape.selected_chesthair_color);
		}
	}
}

void update_blushmenu() {
	menu.Title("Blush");
	menu.Subtitle("Customize your blush.");

	if (menu.BoolOption("Has blush", current_character.outfit_has_blush)) {
		if (CHARACTERMENU::Data::creating) {
			if (!current_character.outfit_has_blush) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", current_character.outfit_selected_blush_type, 0, 6)) {
		if (CHARACTERMENU::Data::creating) {
			current_character.outfit_has_blush = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, current_character.outfit_selected_blush_type, current_character.outfit_selected_blush_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 5, 2, current_character.outfit_selected_blush_color, current_character.outfit_selected_blush_color);
		}
	}

	if (menu.IntOption("Color", current_character.outfit_selected_blush_color, 0, 63)) {
		if (CHARACTERMENU::Data::creating) {
			current_character.outfit_has_blush = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, current_character.outfit_selected_blush_type, current_character.outfit_selected_blush_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 5, 2, current_character.outfit_selected_blush_color, current_character.outfit_selected_blush_color);
		}
	}

	if (menu.FloatOption("Opacity", current_character.outfit_selected_blush_opacity, 0.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			current_character.outfit_has_blush = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, current_character.outfit_selected_blush_type, current_character.outfit_selected_blush_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 5, 2, current_character.outfit_selected_blush_color, current_character.outfit_selected_blush_color);
		}
	}
}

void update_lipstickmenu() {
	menu.Title("Lipstick");
	menu.Subtitle("Customize your lipstick.");

	if (menu.BoolOption("Has lipstick", current_character.outfit_has_lipstick)) {
		if (CHARACTERMENU::Data::creating) {
			if (!current_character.outfit_has_lipstick) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", current_character.outfit_selected_lipstick_type, 0, 9)) {
		if (CHARACTERMENU::Data::creating) {
			current_character.outfit_has_lipstick = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, current_character.outfit_selected_lipstick_type, current_character.outfit_selected_lipstick_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 8, 2, current_character.outfit_selected_lipstick_color, current_character.outfit_selected_lipstick_color);
		}
	}

	if (menu.IntOption("Color", current_character.outfit_selected_lipstick_color, 0, 63)) {
		if (CHARACTERMENU::Data::creating) {
			current_character.outfit_has_lipstick = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, current_character.outfit_selected_lipstick_type, current_character.outfit_selected_lipstick_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 8, 2, current_character.outfit_selected_lipstick_color, current_character.outfit_selected_lipstick_color);
		}
	}

	if (menu.FloatOption("Opacity", current_character.outfit_selected_lipstick_opacity, 0.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			current_character.outfit_has_lipstick = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, current_character.outfit_selected_lipstick_type, current_character.outfit_selected_lipstick_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 8, 2, current_character.outfit_selected_lipstick_color, current_character.outfit_selected_lipstick_color);
		}
	}
}

void update_blemishmenu() {
	menu.Title("Blemish");
	menu.Subtitle("Customize your blemish.");

	if (menu.BoolOption("Has blemish", current_shape.has_blemish)) {
		if (CHARACTERMENU::Data::creating) {
			if (!current_shape.has_blemish) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 0, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", current_shape.selected_blemish, 0, 23)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.has_blemish = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 0, current_shape.selected_blemish, 1);
		}
	}
}

void update_agemenu() {
	menu.Title("Age");
	menu.Subtitle("Customize your age.");

	if (menu.BoolOption("Aging", current_shape.has_aging)) {
		if (CHARACTERMENU::Data::creating) {
			if (!current_shape.has_aging) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 3, 255, 1);
			}
		}
	}

	if (menu.IntOption("Age", current_shape.selected_age, 0, 14)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.has_aging = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 3, current_shape.selected_age, 1);
		}
	}
}

void update_makeupmenu() {
	menu.Title("Makeup");
	menu.Subtitle("Customize your makeup.");

	if (menu.BoolOption("Has makeup", current_character.outfit_has_makeup)) {
		if (CHARACTERMENU::Data::creating) {
			if (!current_character.outfit_has_makeup) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, 255, 1);
			}
		}
	}

	if (menu.IntOption("Style", current_character.outfit_selected_makeup_type, 0, 74)) {
		if (CHARACTERMENU::Data::creating) {
			current_character.outfit_has_makeup = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, current_character.outfit_selected_makeup_type, current_character.outfit_selected_makeup_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 4, 2, current_character.outfit_selected_makeup_color, current_character.outfit_selected_makeup_color);

		}
	}

	if (menu.IntOption("Color", current_character.outfit_selected_makeup_color, 0, 63)) {
		if (CHARACTERMENU::Data::creating) {
			current_character.outfit_has_makeup = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, current_character.outfit_selected_makeup_type, current_character.outfit_selected_makeup_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 4, 2, current_character.outfit_selected_makeup_color, current_character.outfit_selected_makeup_color);

		}
	}

	if (menu.FloatOption("Opacity", current_character.outfit_selected_makeup_opacity, 0.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			current_character.outfit_has_makeup = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, current_character.outfit_selected_makeup_type, current_character.outfit_selected_makeup_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 4, 2, current_character.outfit_selected_makeup_color, current_character.outfit_selected_makeup_color);

		}
	}
}

void update_complexmenu() {
	menu.Title("Complexion");
	menu.Subtitle("Customize your complexion.");

	if (menu.BoolOption("Complexion", current_shape.has_complexion)) {
		if (CHARACTERMENU::Data::creating) {
			if (!current_shape.has_complexion) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 6, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", current_shape.selected_complexion, 0, 11)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.has_complexion = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 6, current_shape.selected_complexion, 1);
		}
	}
}

void update_sunmenu() {
	menu.Title("Sun Damage");
	menu.Subtitle("Customize your sun damage.");

	if (menu.BoolOption("Has sun damage", current_shape.has_sundamage)) {
		if (CHARACTERMENU::Data::creating) {
			if (!current_shape.has_sundamage) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 7, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", current_shape.selected_sundamage, 0, 10)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.has_sundamage = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 7, current_shape.selected_sundamage, 1);
		}
	}
}

void update_molesmenu() {
	menu.Title("Moles & Freckles");
	menu.Subtitle("Customize your moles & freckles.");

	if (menu.BoolOption("Has moles & freckles", current_shape.has_moles)) {
		if (CHARACTERMENU::Data::creating) {
			if (!current_shape.has_moles) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 9, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", current_shape.selected_moles, 0, 17)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.has_moles = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 9, current_shape.selected_moles, 1);
		}
	}
}

void update_tattoomenu() {
	menu.Title("Tattoos");

	if (menu.IntOption("Head", selected_head_tat, 0, tattoos["ZONE_HEAD"].size() - 1)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.head_tattoo = tattoos["ZONE_HEAD"].at(selected_head_tat);
			refresh_tattoos();
		}
	}

	if (menu.IntOption("Torso", selected_torso_tat, 0, tattoos["ZONE_TORSO"].size() - 1)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.torso_tattoo = tattoos["ZONE_TORSO"].at(selected_torso_tat);
			refresh_tattoos();
		}
	}

	if (menu.IntOption("Left arm", selected_larm_tat, 0, tattoos["ZONE_LEFT_ARM"].size() - 1)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.larm_tattoo = tattoos["ZONE_LEFT_ARM"].at(selected_larm_tat);
			refresh_tattoos();
		}
	}

	if (menu.IntOption("Right arm", selected_rarm_tat, 0, tattoos["ZONE_RIGHT_ARM"].size() - 1)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.rarm_tattoo = tattoos["ZONE_RIGHT_ARM"].at(selected_rarm_tat);
			refresh_tattoos();
		}
	}

	if (menu.IntOption("Left leg", selected_lleg_tat, 0, tattoos["ZONE_LEFT_LEG"].size() - 1)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.lleg_tattoo = tattoos["ZONE_LEFT_LEG"].at(selected_lleg_tat);
			refresh_tattoos();
		}
	}

	if (menu.IntOption("Right leg", selected_rleg_tat, 0, tattoos["ZONE_RIGHT_LEG"].size() - 1)) {
		if (CHARACTERMENU::Data::creating) {
			current_shape.rleg_tattoo = tattoos["ZONE_RIGHT_LEG"].at(selected_rleg_tat);
			refresh_tattoos();
		}
	}
}

void update_customization() {
	menu.Title("Character Creation");

	if (CHARACTERMENU::Data::mode == CHARACTERMENU::eMenuMode::all) {
		menu.Subtitle("Create a new character.");
	}
	else {
		menu.Subtitle("Customize your character.");
	}

	if (CHARACTERMENU::Data::mode == CHARACTERMENU::eMenuMode::all || CHARACTERMENU::Data::mode == CHARACTERMENU::eMenuMode::shape) {
		menu.MenuOption("Shape", "shapemenu", { "Customize your face features." });
	}

	if (CHARACTERMENU::Data::mode == CHARACTERMENU::eMenuMode::all || CHARACTERMENU::Data::mode == CHARACTERMENU::eMenuMode::cosmetic) {
		if (menu.MenuOption("Hair", "hairmenu", { "Change your hairstyle." })) {
			set_hair_values();
		}

		if (menu.MenuOption("Eyebrows", "eyebrowmenu", { "Change your eyebrows." })) {
			facecam_start();
			face_lock = true;
		}

		if (menu.MenuOption("Beard", "beardmenu", { "Change your beard." })) {
			facecam_start();
			face_lock = true;
		}

		if (menu.MenuOption("Chest Hair", "chestmenu", { "Change your chest hair." })) {
			facecam_start();
			face_lock = true;
		}

		if (menu.MenuOption("Blush", "blushmenu", { "Change your blush." })) {
			facecam_start();
			face_lock = true;
		}

		if (menu.MenuOption("Lipstick", "lipstickmenu", { "Change your lipstick." })) {
			facecam_start();
			face_lock = true;
		}

		if (menu.MenuOption("Blemish", "blemishmenu", { "Change your blemish." })) {
			facecam_start();
			face_lock = true;
		}

		if (menu.MenuOption("Age", "agemenu", { "Change your age." })) {
			facecam_start();
			face_lock = true;
		}

		if (menu.MenuOption("Makeup", "makeupmenu", { "Change your makeup." })) {
			facecam_start();
			face_lock = true;
		}

		if (menu.MenuOption("Complexion", "complexmenu", { "Change your complexion." })) {
			facecam_start();
			face_lock = true;
		}

		if (menu.MenuOption("Sun Damage", "sunmenu", { "Change your sun damage." })) {
			facecam_start();
			face_lock = true;
		}

		if (menu.MenuOption("Moles & Freckles", "molesmenu", { "Change your moles and freckles." })) {
			facecam_start();
			face_lock = true;
		}
	}

	if (CHARACTERMENU::Data::mode == CHARACTERMENU::eMenuMode::all || CHARACTERMENU::Data::mode == CHARACTERMENU::eMenuMode::tattoo) {
		if (menu.MenuOption("Tattoos", "tattoomenu")) {
			PED::CLEAR_ALL_PED_PROPS(GlobalData::PLAYER_ID);
			PED::SET_PED_DEFAULT_COMPONENT_VARIATION(GlobalData::PLAYER_ID);

			if (UTILS::get_gender() == eGender::GenderFemale) {
				PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 11, 15, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 8, 2, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 3, 15, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 4, 15, 0, 0);
			}
			else {
				PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 11, 15, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 8, 15, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 3, 15, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 4, 21, 0, 0);
			}

			tattoo_lock_out = true;
			tattoo_lock_in = false;
		}
	}
}

void restore_outfit() {
	if (tattoo_lock_in) return;

	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, current_character.outfit_selected_hairstyle, 0, 0);
	PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, current_character.outfit_selected_haircolor, current_character.outfit_selected_highlight);
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 1, current_character.mask_drawable, current_character.mask_texture, 0);
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 4, current_character.leg_drawable, current_character.leg_texture, 0);
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 5, current_character.bag_drawable, current_character.bag_texture, 0);
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 6, current_character.shoe_drawable, current_character.shoe_texture, 0);
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 7, current_character.accessory_drawable, current_character.accessory_texture, 0);
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 8, current_character.undershirt_drawable, current_character.undershirt_texture, 0);
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 9, current_character.armor_drawable, current_character.armor_texture, 0);
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 11, current_character.torso2_drawable, current_character.torso2_texture, 0);
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 3, current_character.torso_drawable, 0, 0);
	PED::SET_PED_EYE_COLOR_(GlobalData::PLAYER_ID, current_character.eye_color);
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 10, current_character.badge_drawable, 0, 0);

	if (current_character.has_hat) {
		PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 0, current_character.hat_type, current_character.hat_color, 1);
	}

	if (current_character.has_glasses) {
		PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 1, current_character.glasses_type, current_character.glasses_color, 1);
	}

	if (current_character.has_ear) {
		PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 2, current_character.ear_type, current_character.ear_color, 1);
	}

	if (current_character.has_watch) {
		PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 6, current_character.watch_type, current_character.watch_color, 1);
	}

	if (current_character.has_bracelet) {
		PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 7, current_character.bracelet_type, current_character.bracelet_color, 1);
	}

	tattoo_lock_in = true;
}

void CHARACTERMENU::OnTick() {
	menu.CheckKeys();
	face_lock = false;
	tattoo_lock_out = false;

	if (menu.CurrentMenu("mainmenu")) {
		update_mainmenu();
	}
	else if (menu.CurrentMenu("customization")) {
		update_customization();
	}
	else if (menu.CurrentMenu("hairmenu")) {
		update_hairmenu();
	}
	else if (menu.CurrentMenu("eyebrowmenu")) {
		update_eyebrowmenu();
		face_lock = true;
	}
	else if (menu.CurrentMenu("beardmenu")) {
		update_beardmenu();
		face_lock = true;
	}
	else if (menu.CurrentMenu("chestmenu")) {
		update_chestmenu();
		face_lock = true;
	}
	else if (menu.CurrentMenu("blushmenu")) {
		update_blushmenu();
		face_lock = true;
	}
	else if (menu.CurrentMenu("lipstickmenu")) {
		update_lipstickmenu();
		face_lock = true;
	}
	else if (menu.CurrentMenu("blemishmenu")) {
		update_blemishmenu();
		face_lock = true;
	}
	else if (menu.CurrentMenu("agemenu")) {
		update_agemenu();
		face_lock = true;
	}
	else if (menu.CurrentMenu("makeupmenu")) {
		update_makeupmenu();
		face_lock = true;
	}
	else if (menu.CurrentMenu("complexmenu")) {
		update_complexmenu();
		face_lock = true;
	}
	else if (menu.CurrentMenu("sunmenu")) {
		update_sunmenu();
		face_lock = true;
	}
	else if (menu.CurrentMenu("molesmenu")) {
		update_molesmenu();
		face_lock = true;
	}
	else if (menu.CurrentMenu("shapemenu")) {
		update_shapemenu();
	}
	else if (menu.CurrentMenu("tattoomenu")) {
		update_tattoomenu();
		tattoo_lock_out = true;
	}

	if (!face_lock) facecam_end();
	if (!tattoo_lock_out) restore_outfit();

	menu.EndMenu();
}