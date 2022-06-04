#pragma once

struct GlobalData {
	static int PLAYER_ID;
	static bool swapped;
};

typedef struct Coords {
	float x;
	float y;
	float z;

	Coords(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

std::vector<Coords> clothes_coords{
	Coords(-1204.60474f, -780.3441f, 16.3322849f),

};
void MainScriptMain();
void MainScriptAbort();