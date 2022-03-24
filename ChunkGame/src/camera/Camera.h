#include "main/olcPixelGameEngine.h"
#include <fstream>
#include "shit/shit.h"
class Camera {
public:
	Camera();
	void set_position(olc::vf2d pos);
	void move_position(olc::vf2d movement);
	void set_view(olc::vf2d dims);
	void zoom(float z);
	olc::vf2d UL_world();
	olc::vf2d BR_world();
	olc::vf2d world_to_screen(olc::vf2d world);
	olc::vf2d screen_to_world(olc::vi2d screen);
	void save(std::fstream outfile);
	void load(std::fstream infile);
public:
	olc::vf2d position;
	olc::vf2d view_dimensions;
	uint32_t downscaler;
};
