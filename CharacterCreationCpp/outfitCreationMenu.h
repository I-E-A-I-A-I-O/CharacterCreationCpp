#pragma once

#include "menu.h"

namespace OUTFITMENU {
	struct Data {
		static bool creating;
		static bool reset;
	};

	typedef struct {
		int outfit_max_hairstyles;
		int outfit_max_haircolors;
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
		bool face_cam_lock;
		bool leg_cam_lock;
		bool shoe_cam_lock;
	} OutfitData;

	static OutfitData outfit_data = OutfitData();

	void Initialize();
	void OnTick();
	bool isOpen();
	void open();
	void close();
}