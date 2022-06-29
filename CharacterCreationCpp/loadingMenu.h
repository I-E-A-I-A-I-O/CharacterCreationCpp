#pragma once

#include "menu.h"

namespace LOADINGMENU {
	struct Data {
		static bool loaded;
		static std::string last_loaded;
	};

	const std::vector<std::string> facial_expression_anims = { "mood_Normal_1", "mood_Happy_1", "mood_Angry_1", "mood_Aiming_1", "mood_Injured_1", "mood_stressed_1", "mood_smug_1", "mood_sulk_1", };
	const std::vector<std::string> facial_expression_labels = { "Normal", "Happy", "Angry", "Aiming", "Injured", "Stressed", "Smug", "Sulking", };
	const std::vector<std::string> walking_style_labels = { "Normal", "Femme", "Gangster", "Posh", "Tough Guy", "Grooving" };
	const std::vector<std::string> walking_styles_female = { "move_f@femme@", "move_f@gangster@ng", "move_f@posh@", "move_f@tough_guy@", "anim@move_f@grooving@" };
	const std::vector<std::string> walking_styles_male = { "move_m@femme@", "move_m@gangster@ng", "move_m@posh@", "move_m@tough_guy@", "anim@move_m@grooving@" };
	void load_outfit(const std::string& outfit_name);
	void Initialize();
	void OnTick();
	bool isOpen();
	void open();
	void close();
}