#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <queue>
#include <complex>
#include <math.h>
#include <thread>
#include <vector>
#include <iomanip>
#include <cstring>
#include <cstddef>

#include "world/World.h"
#include "shit/shit.h"
#include "camera/Camera.h"


//screen setup
const int w = 960;
const int h = 960;
const int pixelSize = 1;

std::unique_ptr<olc::Sprite> sprTiles;
std::unique_ptr<olc::Decal> decTiles;

class MainGameObject : public olc::PixelGameEngine
{
public:
	World world;
	Camera camera;
	MainGameObject() { sAppName = "The name of the app"; }

	void Draw_partial_chunk(olc::vf2d upper_left, olc::vf2d bottom_right, Chunk* chunk) {
		
		olc::vi2d chunk_ul = chunk->get_id() * 256;
		olc::vi2d chunk_br = chunk_ul + olc::vi2d{ 256,256 };

		olc::vi2d start = shit::viClamp((upper_left / camera.downscaler).floor() * camera.downscaler, chunk_ul, chunk_br);
		olc::vi2d end = shit::viClamp((bottom_right / camera.downscaler).ceil() * camera.downscaler, chunk_ul, chunk_br) ;
		//std::cout << start << " - " << end << std::endl;
		for (int y = start.y; y < end.y; y += camera.downscaler) {
			for (int x = start.x; x < end.x; x += camera.downscaler) {
				olc::vf2d screen_pos = (
					camera.world_to_screen(olc::vf2d{ (float)x,(float)y }) 
					* olc::vi2d{ w, h }
				).ceil();
				int chunk_x = x - chunk_ul.x;
				int chunk_y = y - chunk_ul.y;
				uint8_t tile = chunk->get_tile(chunk_x, chunk_y);
				olc::Pixel color = olc::WHITE;
				if (tile == 1) color = olc::RED;
				else if (tile == 2) color = olc::BLUE;
				//DrawRect(screen_pos, olc::vi2d{10,10}, color);
				olc::vf2d decal_scale = (olc::vf2d{ (float)w,(float)h } / camera.view_dimensions * camera.downscaler).ceil();
				DrawPartialDecal(screen_pos, decal_scale,
					decTiles.get(), olc::vf2d{(float)tile * 16, 0}, olc::vf2d{16.0,16.0});
			}
		}
	}
	void Drawer() {
		Clear(olc::Pixel::Pixel());
		for (int i = 0; i < world.loaded_chunks.size(); i++) {
			Draw_partial_chunk(camera.UL_world(), camera.BR_world(), world.loaded_chunks[i]);
		}
	}
	
public:
	
	olc::vi2d test_pos = {0,0};
	bool OnUserCreate() override
	{
		// Load Sprite
		sprTiles = std::make_unique<olc::Sprite>("./assets/tiles.png");
		decTiles = std::make_unique<olc::Decal>(sprTiles.get());

		//world.create("cock", 0);
		world.create("cock",0);
		//world.load_chunk({ 0,0 });
		

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Drawer();
		world.update_chunks(camera.position);

		float speed = 50;
		if (GetKey(olc::Key::UP).bHeld) {
			camera.move_position(olc::vf2d{ 0, -1 } * fElapsedTime * speed);
		}
		if (GetKey(olc::Key::DOWN).bHeld) {
			camera.move_position(olc::vf2d{ 0, 1 } * fElapsedTime * speed);
		}
		if (GetKey(olc::Key::LEFT).bHeld) {
			camera.move_position(olc::vf2d{ -1, 0 } * fElapsedTime * speed);
		}
		if (GetKey(olc::Key::RIGHT).bHeld) {
			camera.move_position(olc::vf2d{ 1, 0 } * fElapsedTime * speed);
		}

		if (GetMouseWheel() < 0) {
			camera.zoom(1.1);
			//downscaler = downscaler * 2;
		}
		if (GetMouseWheel() > 0) {
			camera.zoom(1/1.1);
			//downscaler = std::max(downscaler * 0.5, 1.0);
		}
		return true;
	}
};


int main()
{
	MainGameObject maingame;
	if (maingame.Construct(w, h, pixelSize, pixelSize))
		maingame.Start();
	return 0;
}
