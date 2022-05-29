#include "pch.h"
#include "characterCreationMenu.h"
#include "natives.h"
#include "utils.h"
#include "json.hpp"
#include "mainScript.h"
#include "screen.h"
#include <fstream>

bool CHARACTERMENU::Data::creating = false;
NativeMenu::Menu menu;
int selected_hairstyle;
int selected_haircolor;
int selected_highlight;
int max_hairstyles;
int max_haircolors;
bool has_eyebrows = false;
int selected_eyebrow_type;
int selected_eyebrow_color;
bool has_beard = false;
int selected_beard_type;
int selected_beard_color;
bool has_chesthair = false;
int selected_chesthair_type;
int selected_chesthair_color;
bool has_blush;
int selected_blush_type;
int selected_blush_color;
float selected_blush_opacity;
bool has_lipstick;
int selected_lipstick_type;
int selected_lipstick_color;
float selected_lipstick_opacity;
bool has_blemish = false;
int selected_blemish;
bool has_aging = false;
int selected_age;
bool has_makeup = false;
int selected_makeup_type;
int selected_makeup_color;
float selected_makeup_opacity;
bool has_complexion = false;
int selected_complexion;
bool has_sundamage = false;
int selected_sundamage;
bool has_moles;
int selected_moles;
int shape_mother;
int shape_father;
int skin_mother;
int skin_father;
float shape_mix;
float skin_mix;
float nose_width;
float nose_peak_height;
float nose_peak_length;
float nose_bone_height;
float nose_peak_lowering;
float nose_bone_twist;
float eyebrow_height;
float eyebrow_forward;
float cheekbone_height;
float cheekbone_width;
float cheeks_width;
float eyes_opening;
float lips_thickness;
float jawbone_width;
float jawbone_back_length;
float chinbone_lowering;
float chinbone_length;
float chinbone_width;
float chinbone_hole;
float neck_thickness;

void OnMain() {
	menu.ReadSettings();
	AUDIO::PLAY_SOUND_FRONTEND(-1, (char*)"CONFIRM_BEEP", (char*)"HUD_MINI_GAME_SOUNDSET", true);
}

void reset_items() {
	selected_hairstyle = 0;
	selected_haircolor = 0;
	selected_highlight = 0;
	has_eyebrows = false;
	selected_eyebrow_color = 0;
	selected_eyebrow_type = 0;
}

void OnExit() {
	reset_items();
}

bool CHARACTERMENU::isOpen() {
	return menu.IsThisOpen();
}

void CHARACTERMENU::open() {
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
	std::string filepath = "CharacterCreationData\\Characters\\";
	filepath = filepath.append(name) + ".json";
	nlohmann::json j;
	j["model"] = ENTITY::GET_ENTITY_MODEL(GlobalData::PLAYER_ID);
	j["features"]["shape_mother"] = shape_mother;
	j["features"]["shape_father"] = shape_father;
	j["features"]["skin_mother"] = skin_mother;
	j["features"]["skin_father"] = skin_father;
	j["features"]["shape_mix"] = shape_mix;
	j["features"]["skin_mix"] = skin_mix;
	j["features"]["nose_width"] = nose_width;
	j["features"]["nose_peak_height"] = nose_peak_height;
	j["features"]["nose_peak_length"] = nose_peak_length;
	j["features"]["nose_bone_height"] = nose_bone_height;
	j["features"]["nose_peak_lowering"] = nose_peak_lowering;
	j["features"]["nose_bone_twist"] = nose_bone_twist;
	j["features"]["eyebrow_height"] = eyebrow_height;
	j["features"]["eyebrow_forward"] = eyebrow_forward;
	j["features"]["cheekbone_height"] = cheekbone_height;
	j["features"]["cheekbone_width"] = cheekbone_width;
	j["features"]["cheeks_width"] = cheeks_width;
	j["features"]["eyes_opening"] = eyes_opening;
	j["features"]["lips_thickness"] = lips_thickness;
	j["features"]["jawbone_width"] = jawbone_width;
	j["features"]["jawbone_back_length"] = jawbone_back_length;
	j["features"]["chinbone_lowering"] = chinbone_lowering;
	j["features"]["chinbone_length"] = chinbone_length;
	j["features"]["chinbone_width"] = chinbone_width;
	j["features"]["chin_hole"] = chinbone_hole;
	j["features"]["neck_thickness"] = neck_thickness;
	j["hair"]["style"] = selected_hairstyle;
	j["hair"]["color"] = selected_haircolor;
	j["hair"]["highlight"] = selected_highlight;
	j["eyebrows"]["enabled"] = has_eyebrows;
	j["eyebrows"]["type"] = selected_eyebrow_type;
	j["eyebrows"]["color"] = selected_eyebrow_color;
	j["beard"]["enabled"] = has_beard;
	j["beard"]["type"] = selected_beard_type;
	j["beard"]["color"] = selected_beard_color;
	j["chesthair"]["enabled"] = has_chesthair;
	j["chesthair"]["type"] = selected_chesthair_type;
	j["chesthair"]["color"] = selected_chesthair_color;
	j["blush"]["enabled"] = has_blush;
	j["blush"]["type"] = selected_blush_type;
	j["blush"]["color"] = selected_blush_color;
	j["blush"]["opacity"] = selected_blush_opacity;
	j["lipstick"]["enabled"] = has_lipstick;
	j["lipstick"]["type"] = selected_lipstick_type;
	j["lipstick"]["color"] = selected_lipstick_color;
	j["lipstick"]["opacity"] = selected_lipstick_opacity;
	j["blemish"]["enabled"] = has_blemish;
	j["blemish"]["type"] = selected_blemish;
	j["aging"]["enabled"] = has_aging;
	j["aging"]["age"] = selected_age;
	j["makeup"]["enabled"] = has_makeup;
	j["makeup"]["type"] = selected_makeup_type;
	j["makeup"]["color"] = selected_makeup_color;
	j["makeup"]["opacity"] = selected_makeup_opacity;
	j["complexion"]["enabled"] = has_complexion;
	j["complexion"]["type"] = selected_complexion;
	j["sundamage"]["enabled"] = has_sundamage;
	j["sundamage"]["type"] = selected_sundamage;
	j["molesfreckles"]["enabled"] = has_moles;
	j["molesfreckles"]["type"] = selected_moles;
	std::ofstream o(filepath);
	o << std::setw(4) << j << std::endl;
	SCREEN::ShowNotification("~g~Character saved!");
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

	if (menu.IntOption("Shape mother", shape_mother, 0, 45)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, shape_mother, shape_father, 0, skin_mother, skin_father, 0, shape_mix, skin_mix, 0, 0);
		}
	}

	if (menu.IntOption("Shape father", shape_father, 0, 45)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, shape_mother, shape_father, 0, skin_mother, skin_father, 0, shape_mix, skin_mix, 0, 0);
		}
	}

	if (menu.IntOption("Skin mother", skin_mother, 0, 45)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, shape_mother, shape_father, 0, skin_mother, skin_father, 0, shape_mix, skin_mix, 0, 0);
		}
	}

	if (menu.IntOption("Skin father", skin_father, 0, 45)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, shape_mother, shape_father, 0, skin_mother, skin_father, 0, shape_mix, skin_mix, 0, 0);
		}
	}

	if (menu.FloatOption("Shape mix", shape_mix, 0.00f, 1.00f, 0.01f, { "Set which parent head shape has more influence over your ped's head shape. Values go from Mother to Father." })) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, shape_mother, shape_father, 0, skin_mother, skin_father, 0, shape_mix, skin_mix, 0, 0);
		}
	}

	if (menu.FloatOption("Skin mix", skin_mix, 0.00f, 1.00f, 0.01f, { "Set which parent skin color has more influence over your ped's skin color. Values go from Mother to Father." })) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, shape_mother, shape_father, 0, skin_mother, skin_father, 0, shape_mix, skin_mix, 0, 0);
		}
	}

	if (menu.FloatOption("Nose width", nose_width, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 0, nose_width);
		}
	}

	if (menu.FloatOption("Nose peak height", nose_peak_height, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 1, nose_peak_height);
		}
	}

	if (menu.FloatOption("Nose peak length", nose_peak_length, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 2, nose_peak_length);
		}
	}

	if (menu.FloatOption("Nose bone height", nose_bone_height, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 3, nose_bone_height);
		}
	}

	if (menu.FloatOption("Nose peak lowering", nose_peak_lowering, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 4, nose_peak_lowering);
		}
	}

	if (menu.FloatOption("Nose bone twist", nose_bone_twist, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 5, nose_bone_twist);
		}
	}

	if (menu.FloatOption("Eyebrow height", eyebrow_height, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 6, eyebrow_height);
		}
	}

	if (menu.FloatOption("Eyebrow forward", eyebrow_forward, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 7, eyebrow_forward);
		}
	}

	if (menu.FloatOption("Cheekbone height", cheekbone_height, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 8, cheekbone_height);
		}
	}

	if (menu.FloatOption("Cheekbone width", cheekbone_width, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 9, cheekbone_width);
		}
	}

	if (menu.FloatOption("Cheeks width", cheeks_width, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 10, cheeks_width);
		}
	}

	if (menu.FloatOption("Eye opening", eyes_opening, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 11, eyes_opening);
		}
	}

	if (menu.FloatOption("Lip thickness", lips_thickness, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 12, lips_thickness);
		}
	}

	if (menu.FloatOption("Jawbone width", jawbone_width, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 13, jawbone_width);
		}
	}

	if (menu.FloatOption("Jawbone back length", jawbone_back_length, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 14, jawbone_back_length);
		}
	}

	if (menu.FloatOption("Chinbone lowering", chinbone_lowering, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 15, chinbone_lowering);
		}
	}

	if (menu.FloatOption("Chinbone length", chinbone_length, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 16, chinbone_length);
		}
	}

	if (menu.FloatOption("Chinbone width", chinbone_width, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 17, chinbone_width);
		}
	}

	if (menu.FloatOption("Chin hole", chinbone_hole, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 18, chinbone_hole);
		}
	}

	if (menu.FloatOption("Neck thickness", neck_thickness, -1.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 19, neck_thickness);
		}
	}
}

void update_hairmenu() {
	menu.Title("Hair");
	menu.Subtitle("Customize your hairstyle.");

	if (menu.IntOption("Hairstyle", selected_hairstyle, 0, max_hairstyles)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, selected_hairstyle, 0, 0);
			PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, selected_haircolor, selected_highlight);
		}
	}

	if (menu.IntOption("Hair color", selected_haircolor, 0, max_haircolors)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, selected_hairstyle, 0, 0);
			PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, selected_haircolor, selected_highlight);
		}
	}

	if (menu.IntOption("Hightlight color", selected_highlight, 0, max_haircolors)) {
		if (CHARACTERMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, selected_hairstyle, 0, 0);
			PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, selected_haircolor, selected_highlight);
		}
	}
}

void update_eyebrowmenu() {
	menu.Title("Eyebrows");
	menu.Subtitle("Customize your eyebrows.");

	if (menu.BoolOption("Has eyebrows", has_eyebrows)) {
		if (CHARACTERMENU::Data::creating) {
			if (!has_eyebrows) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 2, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", selected_eyebrow_type, 0, 33)) {
		if (CHARACTERMENU::Data::creating) {
			has_eyebrows = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 2, selected_eyebrow_type, 1);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 2, 1, selected_eyebrow_color, selected_eyebrow_color);
		}
	}

	if (menu.IntOption("Color", selected_eyebrow_color, 0, 63)) {
		if (CHARACTERMENU::Data::creating) {
			has_eyebrows = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 2, selected_eyebrow_type, 1);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 2, 1, selected_eyebrow_color, selected_eyebrow_color);
		}
	}
}

void update_beardmenu() {
	menu.Title("Beard");
	menu.Subtitle("Customize your beard.");

	if (menu.BoolOption("Has beard", has_beard)) {
		if (CHARACTERMENU::Data::creating) {
			if (!has_beard) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 1, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", selected_beard_type, 0, 28)) {
		if (CHARACTERMENU::Data::creating) {
			has_beard = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 1, selected_beard_type, 1);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 1, 1, selected_beard_color, selected_beard_color);
		}
	}

	if (menu.IntOption("Color", selected_beard_color, 0, 63)) {
		if (CHARACTERMENU::Data::creating) {
			has_beard = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 1, selected_beard_type, 1);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 1, 1, selected_beard_color, selected_beard_color);
		}
	}
}

void update_chestmenu() {
	menu.Title("Chest Hair");
	menu.Subtitle("Customize your chest hair.");

	if (menu.BoolOption("Has chest hair", has_chesthair)) {
		if (CHARACTERMENU::Data::creating) {
			if (!has_chesthair) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 10, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", selected_chesthair_type, 0, 16)) {
		if (CHARACTERMENU::Data::creating) {
			has_chesthair = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 10, selected_chesthair_type, 1);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 10, 1, selected_chesthair_color, selected_chesthair_color);
		}
	}

	if (menu.IntOption("Color", selected_chesthair_color, 0, 63)) {
		if (CHARACTERMENU::Data::creating) {
			has_chesthair = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 10, selected_chesthair_type, 1);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 10, 1, selected_chesthair_color, selected_chesthair_color);
		}
	}
}

void update_blushmenu() {
	menu.Title("Blush");
	menu.Subtitle("Customize your blush.");

	if (menu.BoolOption("Has blush", has_blush)) {
		if (CHARACTERMENU::Data::creating) {
			if (!has_blush) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", selected_blush_type, 0, 6)) {
		if (CHARACTERMENU::Data::creating) {
			has_blush = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, selected_blush_type, selected_blush_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 5, 2, selected_blush_color, selected_blush_color);
		}
	}

	if (menu.IntOption("Color", selected_blush_color, 0, 63)) {
		if (CHARACTERMENU::Data::creating) {
			has_blush = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, selected_blush_type, selected_blush_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 5, 2, selected_blush_color, selected_blush_color);
		}
	}

	if (menu.FloatOption("Opacity", selected_blush_opacity, 0.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			has_blush = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, selected_blush_type, selected_blush_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 5, 2, selected_blush_color, selected_blush_color);
		}
	}
}

void update_lipstickmenu() {
	menu.Title("Lipstick");
	menu.Subtitle("Customize your lipstick.");

	if (menu.BoolOption("Has lipstick", has_lipstick)) {
		if (CHARACTERMENU::Data::creating) {
			if (!has_lipstick) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", selected_lipstick_type, 0, 9)) {
		if (CHARACTERMENU::Data::creating) {
			has_lipstick = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, selected_lipstick_type, selected_lipstick_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 8, 2, selected_lipstick_color, selected_lipstick_color);
		}
	}

	if (menu.IntOption("Color", selected_lipstick_color, 0, 63)) {
		if (CHARACTERMENU::Data::creating) {
			has_lipstick = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, selected_lipstick_type, selected_lipstick_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 8, 2, selected_lipstick_color, selected_lipstick_color);
		}
	}

	if (menu.FloatOption("Opacity", selected_lipstick_opacity, 0.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			has_lipstick = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, selected_lipstick_type, selected_lipstick_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 8, 2, selected_lipstick_color, selected_lipstick_color);
		}
	}
}

void update_blemishmenu() {
	menu.Title("Blemish");
	menu.Subtitle("Customize your blemish.");

	if (menu.BoolOption("Has blemish", has_blemish)) {
		if (CHARACTERMENU::Data::creating) {
			if (!has_blemish) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 0, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", selected_blemish, 0, 23)) {
		if (CHARACTERMENU::Data::creating) {
			has_blemish = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 0, selected_blemish, 1);
		}
	}
}

void update_agemenu() {
	menu.Title("Age");
	menu.Subtitle("Customize your age.");

	if (menu.BoolOption("Aging", has_aging)) {
		if (CHARACTERMENU::Data::creating) {
			if (!has_aging) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 3, 255, 1);
			}
		}
	}

	if (menu.IntOption("Age", selected_age, 0, 14)) {
		if (CHARACTERMENU::Data::creating) {
			has_aging = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 3, selected_age, 1);
		}
	}
}

void update_makeupmenu() {
	menu.Title("Makeup");
	menu.Subtitle("Customize your makeup.");

	if (menu.BoolOption("Has makeup", has_makeup)) {
		if (CHARACTERMENU::Data::creating) {
			if (!has_makeup) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, 255, 1);
			}
		}
	}

	if (menu.IntOption("Style", selected_makeup_type, 0, 74)) {
		if (CHARACTERMENU::Data::creating) {
			has_makeup = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, selected_makeup_type, selected_makeup_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 4, 2, selected_makeup_color, selected_makeup_color);

		}
	}

	if (menu.IntOption("Color", selected_makeup_color, 0, 63)) {
		if (CHARACTERMENU::Data::creating) {
			has_makeup = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, selected_makeup_type, selected_makeup_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 4, 2, selected_makeup_color, selected_makeup_color);

		}
	}

	if (menu.FloatOption("Opacity", selected_makeup_opacity, 0.0f, 1.0f)) {
		if (CHARACTERMENU::Data::creating) {
			has_makeup = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, selected_makeup_type, selected_makeup_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 4, 2, selected_makeup_color, selected_makeup_color);

		}
	}
}

void update_complexmenu() {
	menu.Title("Complexion");
	menu.Subtitle("Customize your complexion.");

	if (menu.BoolOption("Complexion", has_complexion)) {
		if (CHARACTERMENU::Data::creating) {
			if (!has_complexion) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 6, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", selected_complexion, 0, 11)) {
		if (CHARACTERMENU::Data::creating) {
			has_complexion = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 6, selected_complexion, 1);
		}
	}
}

void update_sunmenu() {
	menu.Title("Sun Damage");
	menu.Subtitle("Customize your sun damage.");

	if (menu.BoolOption("Has sun damage", has_sundamage)) {
		if (CHARACTERMENU::Data::creating) {
			if (!has_sundamage) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 7, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", selected_sundamage, 0, 10)) {
		if (CHARACTERMENU::Data::creating) {
			has_sundamage = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 7, selected_sundamage, 1);
		}
	}
}

void update_molesmenu() {
	menu.Title("Moles & Freckles");
	menu.Subtitle("Customize your moles & freckles.");

	if (menu.BoolOption("Has moles & freckles", has_moles)) {
		if (CHARACTERMENU::Data::creating) {
			if (!has_moles) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 9, 255, 1);
			}
		}
	}

	if (menu.IntOption("Type", selected_moles, 0, 17)) {
		if (CHARACTERMENU::Data::creating) {
			has_moles = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 9, selected_moles, 1);
		}
	}
}

void update_customization() {
	menu.Title("Character Creation");
	menu.Subtitle("Create a new character.");
	menu.MenuOption("Shape", "shapemenu", { "Customize your face features." });

	if (menu.MenuOption("Hair", "hairmenu", { "Change your hairstyle." })) {
		set_hair_values();
	}
	
	menu.MenuOption("Eyebrows", "eyebrowmenu", { "Change your eyebrows." });
	menu.MenuOption("Beard", "beardmenu", { "Change your beard." });
	menu.MenuOption("Chest Hair", "chestmenu", { "Change your chest hair." });
	menu.MenuOption("Blush", "blushmenu", { "Change your blush." });
	menu.MenuOption("Lipstick", "lipstickmenu", { "Change your lipstick." });
	menu.MenuOption("Blemish", "blemishmenu", { "Change your blemish." });
	menu.MenuOption("Age", "agemenu", { "Change your age." });
	menu.MenuOption("Makeup", "makeupmenu", { "Change your makeup." });
	menu.MenuOption("Complexion", "complexmenu", { "Change your complexion." });
	menu.MenuOption("Sun Damage", "sunmenu", { "Change your sun damage." });
	menu.MenuOption("Moles & Freckles", "molesmenu", { "Change your moles and freckles." });
}

void CHARACTERMENU::OnTick() {
	menu.CheckKeys();

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
	}
	else if (menu.CurrentMenu("beardmenu")) {
		update_beardmenu();
	}
	else if (menu.CurrentMenu("chestmenu")) {
		update_chestmenu();
	}
	else if (menu.CurrentMenu("blushmenu")) {
		update_blushmenu();
	}
	else if (menu.CurrentMenu("lipstickmenu")) {
		update_lipstickmenu();
	}
	else if (menu.CurrentMenu("blemishmenu")) {
		update_blemishmenu();
	}
	else if (menu.CurrentMenu("agemenu")) {
		update_agemenu();
	}
	else if (menu.CurrentMenu("makeupmenu")) {
		update_makeupmenu();
	}
	else if (menu.CurrentMenu("complexmenu")) {
		update_complexmenu();
	}
	else if (menu.CurrentMenu("sunmenu")) {
		update_sunmenu();
	}
	else if (menu.CurrentMenu("molesmenu")) {
		update_molesmenu();
	}
	else if (menu.CurrentMenu("shapemenu")) {
		update_shapemenu();
	}

	menu.EndMenu();
}