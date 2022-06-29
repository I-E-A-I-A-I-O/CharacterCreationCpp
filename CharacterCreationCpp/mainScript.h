#pragma once

#include <vector>

struct GlobalData {
	static int PLAYER_ID;
	static bool swapped;
	static int interaction_key;
	static int loadingmenu_key;
	static int secondary_key;
};

typedef struct Coords {
	float x;
	float y;
	float z;

	Coords(float x, float y, float z): x(x), y(y), z(z) {
	}
} Coords;

typedef struct OutInCoords {
	Coords out_coord;
	Coords in_coord;

	OutInCoords(Coords out_coord, Coords in_coord): out_coord(out_coord), in_coord(in_coord) {
	}
} OutInCoords;

inline std::vector<OutInCoords> clothes_coords = {
	OutInCoords(Coords(-1204.60474f, -780.3441f, 16.3322849f), Coords(-1188.65857f, -765.6327f, 16.3201351f)),
	OutInCoords(Coords(84.02837f, -1391.834f, 28.41698f), Coords(71.80997f, -1398.666f, 28.37614f)),
	OutInCoords(Coords(417.5807f, -807.6917f, 28.39415f), Coords(428.8251f, -800.1398f, 28.49113f)),
	OutInCoords(Coords(127.91f, -209.5596f, 53.54935f), Coords(121.2348f, -225.6124f, 53.55785f)),
	OutInCoords(Coords(-154.5667f, -305.9372f, 37.6882f), Coords(-158.7417f, -296.9941f, 38.7333f)),
	OutInCoords(Coords(-717.4099f, -157.1539f, 35.98805f), Coords(-708.4486f, -160.207f, 36.41516f)),
	OutInCoords(Coords(-1456.945f, -231.3031f, 48.33997f), Coords(-1456.956f, -241.163f, 48.80581f)),
	OutInCoords(Coords(-1095.001f, 2705.316f, 18.08161f), Coords(-1108.402f, 2709.413f, 18.10786f)),
	OutInCoords(Coords(618.2285f, 2749.662f, 41.08868f), Coords(617.6564f, 2766.51f, 41.08812f)),
	OutInCoords(Coords(1198.001f, 2701.644f, 37.16101f), Coords(1190.962f, 2713.843f, 37.22262f)),
	OutInCoords(Coords(1685.406f, 4820.185f, 40.99575f), Coords(1697.076f, 4829.566f, 41.0631f)),
	OutInCoords(Coords(-2.020693f, 6517.645f, 30.48172f), Coords(11.79138f, 6514.016f, 30.87784f))
};

inline std::vector<Coords> barber_coords = {
	Coords(-31.3666f, -145.0191f, 56.0639f),
	Coords(132.3541f, -1712.453f, 28.29168f),
	Coords(1204.733f, -470.5503f, 65.25225f),
	Coords(-824.4573f, -188.7151f, 36.61749f),
	Coords(1934.078f, 3724.152f, 31.8054f),
	Coords(-282.6994f, 6233.681f, 30.48855f)
};

inline std::vector<Coords> tattoo_coords = {
	Coords(-1155.941f, -1422.426f, 3.773431f),
	Coords(1320.64f, -1648.719f, 51.15056f),
	Coords(320.9127f, 176.4153f, 102.6271f),
	Coords(-3166.484f, 1073.67f, 19.84789f),
	Coords(1858.272f, 3747.994f, 32.08252f),
	Coords(-288.3917f, 6200.439f, 30.46353f)
};

void MainScriptMain();
void MainScriptAbort();