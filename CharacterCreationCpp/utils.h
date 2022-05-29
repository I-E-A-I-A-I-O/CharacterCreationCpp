#pragma once

#include "types.h"

namespace UTILS {
	int ranInt(const int& max, const int& min);
	float ranFloat();
	float ranFloat(float max);
	bool can_open_outfitmenu();
	bool can_open_loadingmenu();
	bool can_open_creationmenu();
	size_t ranInt(const size_t& max, const size_t& min);
	bool isInRange(const Vector3& a, const Vector3& b, const float& range);
	void loadModel(Hash model);
	void unloadModel(Hash model);
	Vector3 aroundCoords(float distance, Vector3 coords);
}