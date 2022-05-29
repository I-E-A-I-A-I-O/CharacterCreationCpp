#include "pch.h"
#include "outfitCreationMenu.h"
#include "natives.h"
#include "utils.h"
#include "mainScript.h"
#include "json.hpp"
#include "screen.h"
#include <filesystem>
#include <fstream>
#include <vector>

NativeMenu::Menu omenu;
bool OUTFITMENU::Data::creating = false;
int outfit_selected_hairstyle;
int outfit_selected_haircolor;
int outfit_selected_highlight;
int outfit_max_hairstyles;
int outfit_max_haircolors;
bool outfit_has_blush;
int outfit_selected_blush_type;
int outfit_selected_blush_color;
float outfit_selected_blush_opacity;
bool outfit_has_lipstick;
int outfit_selected_lipstick_type;
int outfit_selected_lipstick_color;
float outfit_selected_lipstick_opacity;
bool outfit_has_makeup = false;
int outfit_selected_makeup_type;
int outfit_selected_makeup_color;
float outfit_selected_makeup_opacity;
int mask_drawable;
int mask_texture;
int torso_drawable;
int leg_drawable;
int leg_texture;
int bag_drawable;
int bag_texture;
int shoe_drawable;
int shoe_texture;
int accessory_drawable;
int accessory_texture;
int undershirt_drawable;
int undershirt_texture;
int armor_drawable;
int armor_texture;
int badge_drawable;
int torso2_drawable;
int torso2_texture;
bool has_hat;
int hat_type;
int hat_color;
bool has_glasses;
int glasses_type;
int glasses_color;
bool has_ear;
int ear_type;
int ear_color;
bool has_watch;
int watch_type;
int watch_color;
bool has_bracelet;
int bracelet_type;
int bracelet_color;
int eye_color;
int max_mask_drawable;
int max_mask_texture;
int max_torso_drawable;
int max_leg_drawable;
int max_leg_texture;
int max_bag_drawable;
int max_bag_texture;
int max_shoe_drawable;
int max_shoe_texture;
int max_accessory_drawable;
int max_accessory_texture;
int max_undershirt_drawable;
int max_undershirt_texture;
int max_armor_drawable;
int max_armor_texture;
int max_badge_drawable;
int max_torso2_drawble;
int max_torso2_texture;
int max_hat_type;
int max_hat_color;
int max_glasses_type;
int max_glasses_color;
int max_ear_type;
int max_ear_color;
int max_watch_type;
int max_watch_color;
int max_bracelet_type;
int max_bracelet_color;

void OnOutfitMain() {
	omenu.ReadSettings();
	AUDIO::PLAY_SOUND_FRONTEND(-1, (char*)"CONFIRM_BEEP", (char*)"HUD_MINI_GAME_SOUNDSET", true);
}

void OnOutfitExit() {
	PLAYER::SET_PLAYER_CONTROL(0, 1, 0);
}

bool OUTFITMENU::isOpen() {
	return omenu.IsThisOpen();
}

void OUTFITMENU::open() {
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

void set_outfit_hair_values() {
	outfit_max_hairstyles = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 2);
	outfit_max_haircolors = PED::GET_NUM_HAIR_COLORS_();
}

void set_outfit_max_drawables() {
	max_mask_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 1);
	max_torso_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 3);
	max_leg_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 4);
	max_bag_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 5);
	max_shoe_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 6);
	max_accessory_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 7);
	max_undershirt_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 8);
	max_armor_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 9);
	max_badge_drawable = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 10);
	max_torso2_drawble = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 11);
	max_hat_type = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 0);
	max_glasses_type = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 1);
	max_ear_type = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 2);
	max_watch_type = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 6);
	max_bracelet_type = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(GlobalData::PLAYER_ID, 7);
}

void update_outfit_hairmenu() {
	omenu.Title("Hair");
	omenu.Subtitle("Customize your hairstyle.");

	if (omenu.IntOption("Hairstyle", outfit_selected_hairstyle, 0, outfit_max_hairstyles)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, outfit_selected_hairstyle, 0, 0);
			PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, outfit_selected_haircolor, outfit_selected_highlight);
		}
	}

	if (omenu.IntOption("Hair color", outfit_selected_haircolor, 0, outfit_max_haircolors)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, outfit_selected_hairstyle, 0, 0);
			PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, outfit_selected_haircolor, outfit_selected_highlight);
		}
	}

	if (omenu.IntOption("Hightlight color", outfit_selected_highlight, 0, outfit_max_haircolors)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, outfit_selected_hairstyle, 0, 0);
			PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, outfit_selected_haircolor, outfit_selected_highlight);
		}
	}
}

void update_outfit_blushmenu() {
	omenu.Title("Blush");
	omenu.Subtitle("Customize your blush.");

	if (omenu.BoolOption("Has blush", outfit_has_blush)) {
		if (OUTFITMENU::Data::creating) {
			if (!outfit_has_blush) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, 255, 1);
			}
		}
	}

	if (omenu.IntOption("Type", outfit_selected_blush_type, 0, 6)) {
		if (OUTFITMENU::Data::creating) {
			outfit_has_blush = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, outfit_selected_blush_type, outfit_selected_blush_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 5, 2, outfit_selected_blush_color, outfit_selected_blush_color);
		}
	}

	if (omenu.IntOption("Color", outfit_selected_blush_color, 0, 63)) {
		if (OUTFITMENU::Data::creating) {
			outfit_has_blush = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, outfit_selected_blush_type, outfit_selected_blush_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 5, 2, outfit_selected_blush_color, outfit_selected_blush_color);
		}
	}

	if (omenu.FloatOption("Opacity", outfit_selected_blush_opacity, 0.0f, 1.0f)) {
		if (OUTFITMENU::Data::creating) {
			outfit_has_blush = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, outfit_selected_blush_type, outfit_selected_blush_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 5, 2, outfit_selected_blush_color, outfit_selected_blush_color);
		}
	}
}

void update_outfit_lipstickmenu() {
	omenu.Title("Lipstick");
	omenu.Subtitle("Customize your lipstick.");

	if (omenu.BoolOption("Has lipstick", outfit_has_lipstick)) {
		if (OUTFITMENU::Data::creating) {
			if (!outfit_has_lipstick) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, 255, 1);
			}
		}
	}

	if (omenu.IntOption("Type", outfit_selected_lipstick_type, 0, 9)) {
		if (OUTFITMENU::Data::creating) {
			outfit_has_lipstick = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, outfit_selected_lipstick_type, outfit_selected_lipstick_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 8, 2, outfit_selected_lipstick_color, outfit_selected_lipstick_color);
		}
	}

	if (omenu.IntOption("Color", outfit_selected_lipstick_color, 0, 63)) {
		if (OUTFITMENU::Data::creating) {
			outfit_has_lipstick = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, outfit_selected_lipstick_type, outfit_selected_lipstick_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 8, 2, outfit_selected_lipstick_color, outfit_selected_lipstick_color);
		}
	}

	if (omenu.FloatOption("Opacity", outfit_selected_lipstick_opacity, 0.0f, 1.0f)) {
		if (OUTFITMENU::Data::creating) {
			outfit_has_lipstick = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, outfit_selected_lipstick_type, outfit_selected_lipstick_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 8, 2, outfit_selected_lipstick_color, outfit_selected_lipstick_color);
		}
	}
}

void update_outfit_makeupmenu() {
	omenu.Title("Makeup");
	omenu.Subtitle("Customize your makeup.");

	if (omenu.BoolOption("Has makeup", outfit_has_makeup)) {
		if (OUTFITMENU::Data::creating) {
			if (!outfit_has_makeup) {
				PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, 255, 1);
			}
		}
	}

	if (omenu.IntOption("Style", outfit_selected_makeup_type, 0, 74)) {
		if (OUTFITMENU::Data::creating) {
			outfit_has_makeup = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, outfit_selected_makeup_type, outfit_selected_makeup_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 4, 2, outfit_selected_makeup_color, outfit_selected_makeup_color);
		}
	}

	if (omenu.IntOption("Color", outfit_selected_makeup_color, 0, 63)) {
		if (OUTFITMENU::Data::creating) {
			outfit_has_makeup = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, outfit_selected_makeup_type, outfit_selected_makeup_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 4, 2, outfit_selected_makeup_color, outfit_selected_makeup_color);
		}
	}

	if (omenu.FloatOption("Opacity", outfit_selected_makeup_opacity, 0.0f, 1.0f)) {
		if (OUTFITMENU::Data::creating) {
			outfit_has_makeup = true;
			PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, outfit_selected_makeup_type, outfit_selected_makeup_opacity);
			PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 4, 2, outfit_selected_makeup_color, outfit_selected_makeup_color);
		}
	}
}

void update_maskmenu() {
	omenu.Title("Mask");
	omenu.Subtitle("Change your mask.");
	
	if (omenu.IntOption("Type", mask_drawable, 0, max_mask_drawable)) {
		mask_texture = 0;
		max_mask_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 1, mask_drawable);

		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 1, mask_drawable, 0, 0);
		}
	}

	if (omenu.IntOption("Color", mask_texture, 0, max_mask_texture)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 1, mask_drawable, mask_texture, 0);
		}
	}
}

void update_legmenu() {
	omenu.Title("Lower Body");
	omenu.Subtitle("Change lower body.");

	if (omenu.IntOption("Type", leg_drawable, 0, max_leg_drawable)) {
		leg_texture = 0;
		max_leg_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 4, leg_drawable);

		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 4, leg_drawable, 0, 0);
		}
	}

	if (omenu.IntOption("Color", leg_texture, 0, max_leg_texture)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 4, leg_drawable, leg_texture, 0);
		}
	}
}

void update_bagmenu() {
	omenu.Title("Bags & Parachute");
	omenu.Subtitle("Change Bags & Parachute.");

	if (omenu.IntOption("Type", bag_drawable, 0, max_bag_drawable)) {
		bag_texture = 0;
		max_bag_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 5, bag_drawable);

		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 5, bag_drawable, 0, 0);
		}
	}

	if (omenu.IntOption("Color", bag_texture, 0, max_bag_texture)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 5, bag_drawable, bag_texture, 0);
		}
	}
}

void update_shoemenu() {
	omenu.Title("Shoes");
	omenu.Subtitle("Change your shoes.");

	if (omenu.IntOption("Type", shoe_drawable, 0, max_shoe_drawable)) {
		shoe_texture = 0;
		max_shoe_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 6, shoe_drawable);

		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 6, shoe_drawable, 0, 0);
		}
	}

	if (omenu.IntOption("Color", shoe_texture, 0, max_shoe_texture)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 6, shoe_drawable, shoe_texture, 0);
		}
	}
}

void update_accessorymenu() {
	omenu.Title("Accessory");
	omenu.Subtitle("Change your accessory.");

	if (omenu.IntOption("Type", accessory_drawable, 0, max_accessory_drawable)) {
		accessory_texture = 0;
		max_accessory_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 7, accessory_drawable);

		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 7, accessory_drawable, 0, 0);
		}
	}

	if (omenu.IntOption("Color", accessory_texture, 0, max_accessory_texture)) {
		if (OUTFITMENU::Data::creating) {
			PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 7, accessory_drawable, accessory_texture, 0);
		}
	}
}

void update_undermenu() {
	omenu.Title("Undershirt");
	omenu.Subtitle("Change your undershirt.");

	if (omenu.IntOption("Type", undershirt_drawable, 0, max_undershirt_drawable)) {
		undershirt_texture = 0;
		max_undershirt_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 8, undershirt_drawable);

		if (OUTFITMENU::Data::creating) PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 8, undershirt_drawable, 0, 0);
	}

	if (omenu.IntOption("Color", undershirt_texture, 0, max_undershirt_texture)) {
		if (OUTFITMENU::Data::creating) PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 8, undershirt_drawable, undershirt_texture, 0);
	}
}

void update_armormenu() {
	omenu.Title("Armor");
	omenu.Subtitle("Change your armor.");

	if (omenu.IntOption("Type", armor_drawable, 0, max_armor_drawable)) {
		armor_texture = 0;
		max_armor_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 9, armor_drawable);

		if (OUTFITMENU::Data::creating) PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 9, armor_drawable, 0, 0);
	}

	if (omenu.IntOption("Color", armor_texture, 0, max_armor_texture)) {
		if (OUTFITMENU::Data::creating) PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 9, armor_drawable, armor_texture, 0);
	}
}

void update_uppermenu() {
	omenu.Title("Upper Body");
	omenu.Subtitle("Change upper body.");

	if (omenu.IntOption("Type", torso2_drawable, 0, max_torso2_drawble)) {
		torso2_texture = 0;
		max_torso2_texture = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 11, torso2_drawable);

		if (OUTFITMENU::Data::creating) PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 11, torso2_drawable, 0, 0);
	}

	if (omenu.IntOption("Color", torso2_texture, 0, max_torso2_texture)) {
		if (OUTFITMENU::Data::creating) PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 11, torso2_drawable, torso2_texture, 0);
	}
}

void update_hatmenu() {
	omenu.Title("Hat");
	omenu.Subtitle("Change your hat.");

	if (omenu.BoolOption("Has hat", has_hat)) {
		if (OUTFITMENU::Data::creating && !has_hat) PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 0);
	}

	if (omenu.IntOption("Type", hat_type, 0, max_hat_type)) {
		hat_color = 0;
		max_hat_color = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 0, hat_type);

		if (OUTFITMENU::Data::creating) {
			has_hat = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 0, hat_type, 0, 1);
		}
	}

	if (omenu.IntOption("Color", hat_color, 0, max_hat_color)) {
		if (OUTFITMENU::Data::creating) {
			has_hat = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 0, hat_type, hat_color, 1);
		}
	}
}

void update_glassmenu() {
	omenu.Title("Glasses");
	omenu.Subtitle("Change your glasses");

	if (omenu.BoolOption("Has glasses", has_glasses)) {
		if (OUTFITMENU::Data::creating && !has_glasses) PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 1);
	}

	if (omenu.IntOption("Type", glasses_type, 0, max_glasses_type)) {
		glasses_color = 0;
		max_glasses_color = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 1, glasses_type);

		if (OUTFITMENU::Data::creating) {
			has_glasses = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 1, glasses_type, 0, 1);
		}
	}

	if (omenu.IntOption("Color", glasses_color, 0, max_glasses_color)) {
		if (OUTFITMENU::Data::creating) {
			has_glasses = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 1, glasses_type, glasses_color, 1);
		}
	}
}

void update_earmenu() {
	omenu.Title("Ear Accessory");
	omenu.Subtitle("Change your ear accessory");

	if (omenu.BoolOption("Has ear accessory", has_ear)) {
		if (OUTFITMENU::Data::creating && !has_ear) PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 2);
	}

	if (omenu.IntOption("Type", ear_type, 0, max_ear_type)) {
		ear_color = 0;
		max_ear_color = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 2, ear_type);

		if (OUTFITMENU::Data::creating) {
			has_ear = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 2, ear_type, 0, 1);
		}
	}

	if (omenu.IntOption("Color", ear_color, 0, max_ear_color)) {
		if (OUTFITMENU::Data::creating) {
			has_ear = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 2, ear_type, ear_color, 1);
		}
	}
}

void update_watchmenu() {
	omenu.Title("Watch");
	omenu.Subtitle("Change your watch");

	if (omenu.BoolOption("Has watch", has_watch)) {
		if (OUTFITMENU::Data::creating && !has_watch) PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 6);
	}

	if (omenu.IntOption("Type", watch_type, 0, max_watch_type)) {
		watch_color = 0;
		max_watch_color = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 6, watch_type);

		if (OUTFITMENU::Data::creating) {
			has_watch = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 6, watch_type, 0, 1);
		}
	}

	if (omenu.IntOption("Color", watch_color, 0, max_watch_color)) {
		if (OUTFITMENU::Data::creating) {
			has_watch = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 6, watch_type, watch_color, 1);
		}
	}
}

void update_braceletmenu() {
	omenu.Title("Bracelet");
	omenu.Subtitle("Change your bracelet");

	if (omenu.BoolOption("Has bracelet", has_bracelet)) {
		if (OUTFITMENU::Data::creating && !has_bracelet) PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 7);
	}

	if (omenu.IntOption("Type", bracelet_type, 0, max_bracelet_type)) {
		bracelet_color = 0;
		max_bracelet_color = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(GlobalData::PLAYER_ID, 7, bracelet_type);

		if (OUTFITMENU::Data::creating) {
			has_bracelet = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 7, bracelet_type, 0, 1);
		}
	}

	if (omenu.IntOption("Color", bracelet_color, 0, max_bracelet_color)) {
		if (OUTFITMENU::Data::creating) {
			has_bracelet = true;
			PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 7, bracelet_type, bracelet_color, 1);
		}
	}
}

void update_clothesmenu() {
	omenu.Title("Outfit Customization");
	omenu.Subtitle("Customize your outfit.");

	if (omenu.IntOption("Eye color", eye_color, 0, 29)) {
		if (OUTFITMENU::Data::creating) PED::SET_PED_EYE_COLOR_(GlobalData::PLAYER_ID, eye_color);
	}

	if (omenu.MenuOption("Hair", "hairmenu", { "Change your hairstyle." })) {
		set_outfit_hair_values();
	}

	omenu.MenuOption("Blush", "blushmenu", { "Change your blush." });
	omenu.MenuOption("Lipstick", "lipstickmenu", { "Change your lipstick." });
	omenu.MenuOption("Makeup", "makeupmenu", { "Change your makeup." });
	omenu.MenuOption("Mask", "maskmenu", { "Change your mask." });
	
	if (omenu.IntOption("Torso", torso_drawable, 0, max_torso_drawable, 1, { "Change your torso." })) {
		PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 3, torso_drawable, 0, 0);
	}

	omenu.MenuOption("Lower body", "legmenu", { "Change lower body." });
	omenu.MenuOption("Bags & Parachute", "bagmenu", { "Change Bags & Parachute." });
	omenu.MenuOption("Shoes", "shoemenu", { "Change your shoes." });
	omenu.MenuOption("Accessory", "accessorymenu", { "Change your accessory." });
	omenu.MenuOption("Undershirt", "undermenu", { "Change your undershirt." });
	omenu.MenuOption("Armor", "armormenu", { "Change your armor." });

	if (omenu.IntOption("Badge", badge_drawable, 0, max_badge_drawable, 1, { "Change your badge." })) {
		PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 10, badge_drawable, 0, 0);
	}

	omenu.MenuOption("Upper body", "uppermenu", { "Change upper body." });
	omenu.MenuOption("Hat", "hatmenu", { "Change your hat." });
	omenu.MenuOption("Glasess", "glassmenu", { "Change your glasess." });
	omenu.MenuOption("Ear accessory", "earmenu", { "Change your ear accessory." });
	omenu.MenuOption("Watch", "watchmenu", { "Change your watch." });
	omenu.MenuOption("Bracelet", "braceletmenu", { "Change your bracelet." });
}

void save_outfit() {
	MISC::DISPLAY_ONSCREEN_KEYBOARD(1, "FMMC_KEY_TIP9", nullptr, "Name", nullptr, nullptr, nullptr, 21);

	while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);

	const char* name = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	std::string filepath = "CharacterCreationData\\Outfits\\";
	filepath = filepath.append(name) + ".json";
	nlohmann::json j;
	j["eyecolor"] = eye_color;
	j["hair"]["style"] = outfit_selected_hairstyle;
	j["hair"]["color"] = outfit_selected_haircolor;
	j["hair"]["highlight"] = outfit_selected_highlight;
	j["blush"]["enabled"] = outfit_has_blush;
	j["blush"]["type"] = outfit_selected_blush_type;
	j["blush"]["color"] = outfit_selected_blush_color;
	j["blush"]["opacity"] = outfit_selected_blush_opacity;
	j["lipstick"]["enabled"] = outfit_has_lipstick;
	j["lipstick"]["type"] = outfit_selected_lipstick_type;
	j["lipstick"]["color"] = outfit_selected_lipstick_color;
	j["lipstick"]["opacity"] = outfit_selected_lipstick_opacity;
	j["makeup"]["enabled"] = outfit_has_makeup;
	j["makeup"]["type"] = outfit_selected_makeup_type;
	j["makeup"]["color"] = outfit_selected_makeup_color;
	j["makeup"]["opacity"] = outfit_selected_makeup_opacity;
	j["mask"]["drawable"] = mask_drawable;
	j["mask"]["texture"] = mask_texture;
	j["leg"]["drawable"] = leg_drawable;
	j["leg"]["texture"] = leg_texture;
	j["bag"]["drawable"] = bag_drawable;
	j["bag"]["texture"] = bag_texture;
	j["shoe"]["drawable"] = shoe_drawable;
	j["shoe"]["texture"] = shoe_texture;
	j["accessory"]["drawable"] = accessory_drawable;
	j["accessory"]["texture"] = accessory_texture;
	j["torso"]["drawable"] = torso_drawable;
	j["torso2"]["drawable"] = torso2_drawable;
	j["torso2"]["texture"] = torso2_texture;
	j["undershirt"]["drawable"] = undershirt_drawable;
	j["undershirt"]["texture"] = undershirt_texture;
	j["armor"]["drawable"] = armor_drawable;
	j["armor"]["texture"] = armor_texture;
	j["hat"]["enabled"] = has_hat;
	j["hat"]["drawable"] = hat_type;
	j["hat"]["texture"] = hat_color;
	j["glasses"]["enabled"] = has_glasses;
	j["glasses"]["drawable"] = glasses_type;
	j["glasses"]["texture"] = glasses_color;
	j["ear"]["enabled"] = has_ear;
	j["ear"]["drawable"] = ear_type;
	j["ear"]["texture"] = ear_color;
	j["watch"]["enabled"] = has_watch;
	j["watch"]["drawable"] = watch_type;
	j["watch"]["texture"] = watch_color;
	j["bracelet"]["enabled"] = has_bracelet;
	j["bracelet"]["drawable"] = bracelet_type;
	j["bracelet"]["texture"] = bracelet_color;
	j["badge"] = badge_drawable;
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
	}
	else if (omenu.CurrentMenu("lipstickmenu")) {
		update_outfit_lipstickmenu();
	}
	else if (omenu.CurrentMenu("makeupmenu")) {
		update_outfit_makeupmenu();
	}
	else if (omenu.CurrentMenu("maskmenu")) {
		update_maskmenu();
	}
	else if (omenu.CurrentMenu("legmenu")) {
		update_legmenu();
	}
	else if (omenu.CurrentMenu("bagmenu")) {
		update_bagmenu();
	}
	else if (omenu.CurrentMenu("shoemenu")) {
		update_shoemenu();
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

	omenu.EndMenu();
}