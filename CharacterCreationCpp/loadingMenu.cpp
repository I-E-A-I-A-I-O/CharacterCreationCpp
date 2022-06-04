#include "pch.h"
#include "loadingmenu.h"
#include "natives.h"
#include "utils.h"
#include "mainScript.h"
#include "nlohmann/json.hpp"
#include "screen.h"
#include "characterData.h"
#include <filesystem>
#include <fstream>
#include <vector>

NativeMenu::Menu lmenu;
std::vector<std::string> saved_characters;
std::vector<std::string> saved_outfits;
bool LOADINGMENU::Data::loaded = false;
std::string LOADINGMENU::Data::last_loaded = "";
typedef struct {
	int shapeFirst, shapeSecond, shapeThird;
	int skinFirst, skinSecond, skinThird;
	int shapeMix, skinMix, thirdMix;
} headBlendData;

void OnLoadingMain() {
	lmenu.ReadSettings();
	AUDIO::PLAY_SOUND_FRONTEND(-1, (char*)"CONFIRM_BEEP", (char*)"HUD_MINI_GAME_SOUNDSET", true);
}

void OnLoadingExit() {
	PLAYER::SET_PLAYER_CONTROL(0, 1, 0);
}

bool LOADINGMENU::isOpen() {
	return lmenu.IsThisOpen();
}

void LOADINGMENU::open() {
	lmenu.OpenMenu();
}

void LOADINGMENU::close() {
	lmenu.CloseMenu();
}

void LOADINGMENU::Initialize() {
	lmenu.RegisterOnMain(std::bind(OnLoadingMain));
	lmenu.RegisterOnExit(std::bind(OnLoadingExit));
	lmenu.ReadSettings();
}

void load_characters() {
	saved_characters = std::vector<std::string>();
	std::string path = "CharacterCreationData\\Characters";

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			std::string filename = entry.path().filename().string();
			size_t index = filename.find_last_of(".");
			std::string name = filename.substr(0, index);
			saved_characters.push_back(name);
		}
	}
}

void load_outfits() {
	saved_outfits = std::vector<std::string>();
	std::string path = "CharacterCreationData\\Outfits";

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			std::string filename = entry.path().filename().string();
			size_t index = filename.find_last_of(".");
			std::string name = filename.substr(0, index);
			saved_outfits.push_back(name);
		}
	}
}

void LOADINGMENU::load_outfit(const std::string& outfit_name) {
	Data::loaded = true;
	Data::last_loaded = outfit_name;
	nlohmann::json j;
	std::string filepath = "CharacterCreationData\\Outfits\\";
	filepath = filepath.append(outfit_name).append(".json");
	std::ifstream i(filepath);
	i >> j;
	i.close();
	current_character.outfit_selected_hairstyle = j["hair"]["style"];
	current_character.outfit_selected_haircolor = j["hair"]["color"];
	current_character.outfit_selected_highlight = j["hair"]["highlight"];
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, current_character.outfit_selected_hairstyle, 0, 0);
	PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, current_character.outfit_selected_haircolor, current_character.outfit_selected_highlight);
	current_character.outfit_has_blush = j["blush"]["enabled"];

	if (current_character.outfit_has_blush) {
		current_character.outfit_selected_blush_type = j["blush"]["type"];
		current_character.outfit_selected_blush_color = j["blush"]["color"];
		current_character.outfit_selected_blush_opacity = j["blush"]["opacity"];
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, current_character.outfit_selected_blush_type, current_character.outfit_selected_blush_opacity);
		PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 5, 2, current_character.outfit_selected_blush_color, current_character.outfit_selected_blush_color);
	}
	else {
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, 255, 1);
	}

	current_character.outfit_has_lipstick = j["lipstick"]["enabled"];

	if (current_character.outfit_has_lipstick) {
		current_character.outfit_selected_lipstick_type = j["lipstick"]["type"];
		current_character.outfit_selected_lipstick_color = j["lipstick"]["color"];
		current_character.outfit_selected_lipstick_opacity = j["lipstick"]["opacity"];
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, current_character.outfit_selected_lipstick_type, current_character.outfit_selected_lipstick_opacity);
		PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 8, 2, current_character.outfit_selected_lipstick_color, current_character.outfit_selected_lipstick_color);
	}
	else {
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, 255, 1);
	}

	current_character.outfit_has_makeup = j["makeup"]["enabled"];

	if (current_character.outfit_has_makeup) {
		current_character.outfit_selected_makeup_type = j["makeup"]["type"];
		current_character.outfit_selected_makeup_color = j["makeup"]["color"];
		current_character.outfit_selected_makeup_opacity = j["makeup"]["opacity"];
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, current_character.outfit_selected_makeup_type, current_character.outfit_selected_makeup_opacity);
		PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 4, 2, current_character.outfit_selected_makeup_color, current_character.outfit_selected_makeup_color);
	}
	else {
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, 255, 1);
	}

	current_character.mask_drawable = j["mask"]["drawable"];
	current_character.mask_texture = j["mask"]["texture"];
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 1, current_character.mask_drawable, current_character.mask_texture, 0);
	current_character.leg_drawable = j["leg"]["drawable"];
	current_character.leg_texture = j["leg"]["texture"];
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 4, current_character.leg_drawable, current_character.leg_texture, 0);
	current_character.bag_drawable = j["bag"]["drawable"];
	current_character.bag_texture = j["bag"]["texture"];
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 5, current_character.bag_drawable, current_character.bag_texture, 0);
	current_character.shoe_drawable = j["shoe"]["drawable"];
	current_character.shoe_texture = j["shoe"]["texture"];
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 6, current_character.shoe_drawable, current_character.shoe_texture, 0);
	current_character.accessory_drawable = j["accessory"]["drawable"];
	current_character.accessory_texture = j["accessory"]["texture"];
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 7, current_character.accessory_drawable, current_character.accessory_texture, 0);
	current_character.undershirt_drawable = j["undershirt"]["drawable"];
	current_character.undershirt_texture = j["undershirt"]["texture"];
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 8, current_character.undershirt_drawable, current_character.undershirt_texture, 0);
	current_character.armor_drawable = j["armor"]["drawable"];
	current_character.armor_texture = j["armor"]["texture"];
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 9, current_character.armor_drawable, current_character.armor_texture, 0);
	current_character.torso2_drawable = j["torso2"]["drawable"];
	current_character.torso2_texture = j["torso2"]["texture"];
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 11, current_character.torso2_drawable, current_character.torso2_texture, 0);
	current_character.torso_drawable = j["torso"]["drawable"];
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 3, current_character.torso_drawable, 0, 0);
	current_character.eye_color = j["eyecolor"];
	PED::SET_PED_EYE_COLOR_(GlobalData::PLAYER_ID, current_character.eye_color);
	current_character.badge_drawable = j["badge"];
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 10, current_character.badge_drawable, 0, 0);
	current_character.has_hat = j["hat"]["enabled"];

	if (current_character.has_hat) {
		current_character.hat_type = j["hat"]["drawable"];
		current_character.hat_color = j["hat"]["texture"];
		PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 0, current_character.hat_type, current_character.hat_color, 1);
	}
	else {
		PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 0);
	}

	current_character.has_glasses = j["glasses"]["enabled"];

	if (current_character.has_glasses) {
		current_character.glasses_type = j["glasses"]["drawable"];
		current_character.glasses_color = j["glasses"]["texture"];
		PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 1, current_character.glasses_type, current_character.glasses_color, 1);
	}
	else {
		PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 1);
	}

	current_character.has_ear = j["ear"]["enabled"];

	if (current_character.has_ear) {
		current_character.ear_type = j["ear"]["drawable"];
		current_character.ear_color = j["ear"]["texture"];
		PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 2, current_character.ear_type, current_character.ear_color, 1);
	}
	else {
		PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 2);
	}

	current_character.has_watch = j["watch"]["enabled"];

	if (current_character.has_watch) {
		current_character.watch_type = j["watch"]["drawable"];
		current_character.watch_color = j["watch"]["texture"];
		PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 6, current_character.watch_type, current_character.watch_color, 1);
	}
	else {
		PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 6);
	}

	current_character.has_bracelet = j["bracelet"]["enabled"];

	if (current_character.has_bracelet) {
		current_character.bracelet_type = j["bracelet"]["drawable"];
		current_character.bracelet_color = j["bracelet"]["texture"];
		PED::SET_PED_PROP_INDEX(GlobalData::PLAYER_ID, 7, current_character.bracelet_type, current_character.bracelet_color, 1);
	}
	else {
		PED::CLEAR_PED_PROP(GlobalData::PLAYER_ID, 7);
	}

	SCREEN::ShowNotification("~g~Outfit loaded!");
}

void load_character(const std::string& character_name) {
	CAM::DO_SCREEN_FADE_OUT(2000);
	WAIT(4000);
	nlohmann::json j;
	std::string filepath = "CharacterCreationData\\Characters\\";
	filepath = filepath.append(character_name).append(".json");
	std::ifstream i(filepath);
	i >> j;
	i.close();
	Hash model = j["model"];
	UTILS::loadModel(model);
	PLAYER::SET_PLAYER_MODEL(0, model);
	UTILS::unloadModel(model);
	GlobalData::PLAYER_ID = PLAYER::PLAYER_PED_ID();
	GlobalData::swapped = true;
	current_character = CharacterData();
	current_shape.shape_mother = j["features"]["shape_mother"];
	current_shape.shape_father = j["features"]["shape_father"];
	current_shape.skin_mother = j["features"]["skin_mother"];
	current_shape.skin_father = j["features"]["skin_father"];
	current_shape.shape_mix = j["features"]["shape_mix"];
	current_shape.skin_mix = j["features"]["skin_mix"];
	PED::SET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, current_shape.shape_mother, current_shape.shape_father, 0, current_shape.skin_mother, current_shape.skin_father, 0, current_shape.shape_mix, current_shape.skin_mix, 0, 0);
	current_shape.nose_width = j["features"]["nose_width"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 0, current_shape.nose_width);
	current_shape.nose_peak_height = j["features"]["nose_peak_height"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 1, current_shape.nose_peak_height);
	current_shape.nose_peak_length = j["features"]["nose_peak_length"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 2, current_shape.nose_peak_length);
	current_shape.nose_bone_height = j["features"]["nose_bone_height"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 3, current_shape.nose_bone_height);
	current_shape.nose_peak_lowering = j["features"]["nose_peak_lowering"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 4, current_shape.nose_peak_lowering);
	current_shape.nose_bone_twist = j["features"]["nose_bone_twist"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 5, current_shape.nose_bone_twist);
	current_shape.eyebrow_height = j["features"]["eyebrow_height"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 6, current_shape.eyebrow_height);
	current_shape.eyebrow_forward = j["features"]["eyebrow_forward"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 7, current_shape.eyebrow_forward);
	current_shape.cheekbone_height = j["features"]["cheekbone_height"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 8, current_shape.cheekbone_height);
	current_shape.cheekbone_width = j["features"]["cheekbone_width"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 9, current_shape.cheekbone_width);
	current_shape.cheeks_width = j["features"]["cheeks_width"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 10, current_shape.cheeks_width);
	current_shape.eyes_opening = j["features"]["eyes_opening"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 11, current_shape.eyes_opening);
	current_shape.lips_thickness = j["features"]["lips_thickness"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 12, current_shape.lips_thickness);
	current_shape.jawbone_width = j["features"]["jawbone_width"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 13, current_shape.jawbone_width);
	current_shape.jawbone_back_length = j["features"]["jawbone_back_length"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 14, current_shape.jawbone_back_length);
	current_shape.chinbone_lowering = j["features"]["chinbone_lowering"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 15, current_shape.chinbone_lowering);
	current_shape.chinbone_length = j["features"]["chinbone_length"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 16, current_shape.chinbone_length);
	current_shape.chinbone_width = j["features"]["chinbone_width"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 17, current_shape.chinbone_width);
	current_shape.chinbone_hole = j["features"]["chin_hole"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 18, current_shape.chinbone_hole);
	current_shape.neck_thickness = j["features"]["neck_thickness"];
	PED::SET_PED_MICRO_MORPH_VALUE_(GlobalData::PLAYER_ID, 19, current_shape.neck_thickness);
	current_character.outfit_selected_hairstyle = j["hair"]["style"];
	current_character.outfit_selected_haircolor = j["hair"]["color"];
	current_character.outfit_selected_highlight = j["hair"]["highlight"];
	PED::SET_PED_COMPONENT_VARIATION(GlobalData::PLAYER_ID, 2, current_character.outfit_selected_hairstyle, 0, 0);
	PED::SET_PED_HAIR_COLOR_(GlobalData::PLAYER_ID, current_character.outfit_selected_haircolor, current_character.outfit_selected_highlight);
	current_shape.has_eyebrows = j["eyebrows"]["enabled"];

	if (current_shape.has_eyebrows) {
		current_shape.selected_eyebrow_type = j["eyebrows"]["type"];
		current_shape.selected_eyebrow_color = j["eyebrows"]["color"];
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 2, current_shape.selected_eyebrow_type, 1);
		PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 2, 1, current_shape.selected_eyebrow_color, current_shape.selected_eyebrow_color);
	}

	current_shape.has_beard = j["beard"]["enabled"];

	if (current_shape.has_beard) {
		current_shape.selected_beard_type = j["beard"]["type"];
		current_shape.selected_beard_color = j["beard"]["color"];
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 1, current_shape.selected_beard_type, 1);
		PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 1, 1, current_shape.selected_beard_color, current_shape.selected_beard_color);
	}

	current_shape.has_chesthair = j["chesthair"]["enabled"];

	if (current_shape.has_chesthair) {
		current_shape.selected_chesthair_type = j["chesthair"]["type"];
		current_shape.selected_chesthair_color = j["chesthair"]["color"];
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 10, current_shape.selected_chesthair_type, 1);
		PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 10, 1, current_shape.selected_chesthair_color, current_shape.selected_chesthair_color);
	}

	current_character.outfit_has_blush = j["blush"]["enabled"];

	if (current_character.outfit_has_blush) {
		current_character.outfit_selected_blush_type = j["blush"]["type"];
		current_character.outfit_selected_blush_color = j["blush"]["color"];
		current_character.outfit_selected_blush_opacity = j["blush"]["opacity"];
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 5, current_character.outfit_selected_blush_type, current_character.outfit_selected_blush_opacity);
		PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 5, 2, current_character.outfit_selected_blush_color, current_character.outfit_selected_blush_color);
	}

	current_character.outfit_has_lipstick = j["lipstick"]["enabled"];

	if (current_character.outfit_has_lipstick) {
		current_character.outfit_selected_lipstick_type = j["lipstick"]["type"];
		current_character.outfit_selected_lipstick_color = j["lipstick"]["color"];
		current_character.outfit_selected_lipstick_opacity = j["lipstick"]["opacity"];
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 8, current_character.outfit_selected_lipstick_type, current_character.outfit_selected_lipstick_opacity);
		PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 8, 2, current_character.outfit_selected_lipstick_color, current_character.outfit_selected_lipstick_color);
	}

	current_shape.has_blemish = j["blemish"]["enabled"];

	if (current_shape.has_blemish) {
		current_shape.selected_blemish = j["blemish"]["type"];
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 0, current_shape.selected_blemish, 1);
	}

	current_shape.has_aging = j["aging"]["enabled"];

	if (current_shape.has_aging) {
		current_shape.selected_age = j["aging"]["age"];
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 3, current_shape.selected_age, 1);
	}

	current_character.outfit_has_makeup = j["makeup"]["enabled"];

	if (current_character.outfit_has_makeup) {
		current_character.outfit_selected_makeup_type = j["makeup"]["type"];
		current_character.outfit_selected_makeup_color = j["makeup"]["color"];
		current_character.outfit_selected_makeup_opacity = j["makeup"]["opacity"];
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 4, current_character.outfit_selected_makeup_type, current_character.outfit_selected_makeup_opacity);
		PED::SET_PED_HEAD_OVERLAY_COLOR_(GlobalData::PLAYER_ID, 4, 2, current_character.outfit_selected_makeup_color, current_character.outfit_selected_makeup_color);
	}

	current_shape.has_complexion = j["complexion"]["enabled"];

	if (current_shape.has_complexion) {
		current_shape.selected_complexion = j["complexion"]["type"];
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 6, current_shape.selected_complexion, 1);
	}

	current_shape.has_sundamage = j["sundamage"]["enabled"];

	if (current_shape.has_sundamage) {
		current_shape.selected_sundamage = j["sundamage"]["type"];
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 7, current_shape.selected_sundamage, 1);
	}

	current_shape.has_moles = j["molesfreckles"]["enabled"];

	if (current_shape.has_moles) {
		current_shape.selected_moles = j["molesfreckles"]["type"];
		PED::SET_PED_HEAD_OVERLAY(GlobalData::PLAYER_ID, 9, current_shape.selected_moles, 1);
	}

	WAIT(1000);
	CAM::DO_SCREEN_FADE_IN(2000);
	SCREEN::ShowNotification("~g~Character loaded!");
}

void save_current_character() {
	MISC::DISPLAY_ONSCREEN_KEYBOARD(1, "FMMC_KEY_TIP9", nullptr, "Name", nullptr, nullptr, nullptr, 21);

	while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);

	const char* name = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	std::string filepath = "CharacterCreationData\\Characters\\";
	filepath = filepath.append(name) + ".json";
	nlohmann::json j;
	j["model"] = ENTITY::GET_ENTITY_MODEL(GlobalData::PLAYER_ID);
	headBlendData headData = headBlendData();
	PED::GET_PED_HEAD_BLEND_DATA(GlobalData::PLAYER_ID, (Any*) &headData);
}

void update_charmenu() {
	lmenu.Title("Current Character");
	lmenu.Subtitle("Save your current character and outfit");

	if (lmenu.Option("Save character")) {
		save_current_character();
	}

	if (lmenu.Option("Save outfit")) {

	}
}

void update_loadingmenu() {
	lmenu.Title("Character Creation");
	lmenu.Subtitle("Load saved characters & outfits.");

	if (lmenu.MenuOption("Characters", "characters", { "Load saved characters." })) {
		load_characters();
	}

	if (lmenu.MenuOption("Outfits", "outfits", { "Load saved outfits." })) {
		load_outfits();
	}

	//lmenu.MenuOption("Current character", "charmenu", { "Save your current character and outfit." });
}

void update_characters() {
	lmenu.Title("Characters");
	lmenu.Subtitle("Load saved characters.");
	
	for (const auto& entry : saved_characters) {
		if (lmenu.Option(entry, { "Load this character" })) {
			load_character(entry);
		}
	}
}

void update_outfits() {
	lmenu.Title("Outfits");
	lmenu.Subtitle("Load saved outfits.");

	for (const auto& entry : saved_outfits) {
		if (lmenu.Option(entry, { "Load this outfit" })) {
			if (!UTILS::is_freemode_character()) {
				SCREEN::ShowNotification("~r~Not available for this character.");
			}
			else {
				LOADINGMENU::load_outfit(entry);
			}
		}
	}
}

void LOADINGMENU::OnTick() {
	lmenu.CheckKeys();

	if (lmenu.CurrentMenu("mainmenu")) {
		update_loadingmenu();
	}
	else if (lmenu.CurrentMenu("characters")) {
		update_characters();
	}
	else if (lmenu.CurrentMenu("outfits")) {
		update_outfits();
	}
	/*else if (lmenu.CurrentMenu("charmenu")) {
		update_charmenu();
	}*/

	lmenu.EndMenu();
}