#include "pch.h"
#include "utils.h"
#include "natives.h"
#include "screen.h"
#include <cmath>
#include <math.h>
#include <string>

int UTILS::ranInt(const int& max, const int& min)
{
	return min + (std::rand() % (max - min + 1));
}

size_t UTILS::ranInt(const size_t& max, const size_t& min)
{
	return min + (std::rand() % (max - min + 1));
}

bool UTILS::is_main_character() {
	return PED::IS_PED_MODEL(GlobalData::PLAYER_ID, 0xD7114C9) || PED::IS_PED_MODEL(GlobalData::PLAYER_ID, 0x9B22DBAF) || PED::IS_PED_MODEL(GlobalData::PLAYER_ID, 0x9B810FA2);
}

bool UTILS::is_freemode_character() {
	return PED::IS_PED_MODEL(GlobalData::PLAYER_ID, 0x9C9EFFD8) || PED::IS_PED_MODEL(GlobalData::PLAYER_ID, 0x705E61F2);
}

bool UTILS::is_main_character_2() {
	Hash player_model = ENTITY::GET_ENTITY_MODEL(GlobalData::PLAYER_ID);
	return player_model == 0xD7114C9 || player_model == 0x9B22DBAF || player_model == 0x9B810FA2;
}

eGender UTILS::get_gender() {
	if (PED::IS_PED_MODEL(GlobalData::PLAYER_ID, 0x9C9EFFD8)) return eGender::GenderFemale;
	return eGender::GenderMale;
}

bool UTILS::isInRange(const Vector3& a, const Vector3& b, const float& range)
{
	Vector3 dif = Vector3();
	dif.x = a.x - b.x;
	dif.y = a.y - b.y;
	dif.z = a.z - b.z;
	float length = (dif.x * dif.x) + (dif.y * dif.y) + (dif.z * dif.z);
	return std::sqrt(length) < range;
}

bool is_busy() {
	return PLAYER::GET_PLAYER_WANTED_LEVEL(0) || MISC::GET_MISSION_FLAG() || PED::IS_PED_IN_ANY_VEHICLE(GlobalData::PLAYER_ID, 0);
}

bool UTILS::can_open_loadingmenu() {
	return !can_open_creationmenu() && !is_busy();
}

bool UTILS::can_open_outfitmenu(OutInCoords& p_active_location) {
	for (auto& coords : clothes_coords) {
		Vector3 shop_coords = Vector3();
		shop_coords.x = coords.out_coord.x;
		shop_coords.y = coords.out_coord.y;
		shop_coords.z = coords.out_coord.z;

		if (isInRange(ENTITY::GET_ENTITY_COORDS(GlobalData::PLAYER_ID, 1), shop_coords, 5)) {
			if (is_busy()) {
				return false;
			}
			else {
				p_active_location = coords;
				return true;
			}
		}
	}

	return false;
}

bool UTILS::can_open_creationmenu() {
	Vector3 creation_coords = Vector3();
	creation_coords.x = -1042.3564f;
	creation_coords.y = -2745.46924f;
	creation_coords.z = 20.36439f;

	if (isInRange(ENTITY::GET_ENTITY_COORDS(GlobalData::PLAYER_ID, 1), creation_coords, 5)) return !is_busy();
	return false;
}

bool UTILS::can_open_hospitalmenu() {
	if (is_freemode_character()) {
		Vector3 creation_coords = Vector3();
		creation_coords.x = -453.2057f;
		creation_coords.y = -339.4345f;
		creation_coords.z = 33.3635f;

		if (isInRange(ENTITY::GET_ENTITY_COORDS(GlobalData::PLAYER_ID, 1), creation_coords, 5)) return !is_busy();
	}
	return false;
}

bool UTILS::can_open_barbershopmenu() {
	for (auto& coords : barber_coords) {
		Vector3 shop_coords = Vector3();
		shop_coords.x = coords.x;
		shop_coords.y = coords.y;
		shop_coords.z = coords.z;

		if (isInRange(ENTITY::GET_ENTITY_COORDS(GlobalData::PLAYER_ID, 1), shop_coords, 5)) {
			if (is_busy()) {
				return false;
			}
			else {
				return true;
			}
		}
	}

	return false;
}

bool UTILS::can_open_tattoomenu() {
	for (auto& coords : tattoo_coords) {
		Vector3 shop_coords = Vector3();
		shop_coords.x = coords.x;
		shop_coords.y = coords.y;
		shop_coords.z = coords.z;

		if (isInRange(ENTITY::GET_ENTITY_COORDS(GlobalData::PLAYER_ID, 1), shop_coords, 5)) {
			if (is_busy()) {
				return false;
			}
			else {
				return true;
			}
		}
	}

	return false;
}

void UTILS::loadModel(Hash model) {
	STREAMING::REQUEST_MODEL(model);

	while (!STREAMING::HAS_MODEL_LOADED(model))
		WAIT(0);
}

void UTILS::unloadModel(Hash model) {
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
}


float UTILS::ranFloat() {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float UTILS::ranFloat(float max) {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX / max);
}

float VectorLength(Vector3 vector) {
	return sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
}

Vector3 NormalizeVector(Vector3 vector) {
	float length = VectorLength(vector);

	if (length == 0)
		return vector;

	Vector3 newVector = Vector3();
	float num = 1 / length;
	newVector.x = vector.x * num;
	newVector.y = vector.y * num;
	newVector.z = vector.z * num;

	return newVector;
}

Vector3 RandomXY() {
	Vector3 vector = Vector3();
	float radian = UTILS::ranFloat() * 2 * (atan(1) * 4);
	vector.x = cos(radian);
	vector.y = sin(radian);
	vector.z = 0;
	return NormalizeVector(vector);
}

void MultiplyVector(Vector3& vector, float value) {
	vector.x *= value;
	vector.y *= value;
	vector.z *= value;
}

Vector3 AddVectors(const Vector3& a, const Vector3& b) {
	Vector3 vec = Vector3();
	vec.x = a.x + b.x;
	vec.y = a.y + b.y;
	vec.z = a.z + b.z;
	return vec;
}

Vector3 UTILS::aroundCoords(float distance, Vector3 coords) {
	Vector3 ranXY = RandomXY();
	MultiplyVector(ranXY, distance);
	return AddVectors(coords, ranXY);
}

std::string UTILS::getLabel(std::string name) {
	const char* label = HUD::GET_LABEL_TEXT_(name.c_str());

	if (MISC::IS_STRING_NULL_OR_EMPTY(label))
		label = "NULL";

	return std::string(label);
}
