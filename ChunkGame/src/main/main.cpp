#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <complex>
#include <math.h>
#include <iomanip>
#include <cstddef>

#include "world/World.h"
#include "shit/shit.h"
#include "shit/inputs.h"
#include "camera/Camera.h"
#include "player/Player.h"

int w = 960;
int h = 960;
const int pixelSize = 1;

class MainGameObject : public olc::PixelGameEngine
{
public:
	std::unique_ptr<olc::Sprite> sprTiles;
	std::unique_ptr<olc::Decal> decTiles;
	std::unique_ptr<olc::Sprite> sprPlayer;
	std::unique_ptr<olc::Decal> decPlayer;

	World world;
	Camera camera;
	Player player;
	InputList inputs;
	MainGameObject() { sAppName = "The name of the app"; }

	void Draw_partial_chunk(olc::vf2d upper_left, olc::vf2d bottom_right, Chunk* chunk, olc::vf2d decal_scale) {
		
		olc::vi2d chunk_ul = chunk->get_id() * 256;
		olc::vi2d chunk_br = chunk_ul + olc::vi2d{ 256,256 };

		olc::vi2d start = shit::viClamp((upper_left / camera.downscaler).floor() 
			* camera.downscaler, chunk_ul, chunk_br);
		olc::vi2d end = shit::viClamp((bottom_right / camera.downscaler).ceil() 
			* camera.downscaler, chunk_ul, chunk_br) ;
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
				
				DrawPartialDecal(screen_pos, decal_scale,
					decTiles.get(), olc::vf2d{(float)tile * 16, 0}, olc::vf2d{16.0,16.0});
			}
		}
	}
	void Drawer() {
		Clear(olc::Pixel::Pixel());
		olc::vf2d decal_scale = olc::vf2d{ (float)w,(float)h } / camera.view_dimensions;
		for (int i = 0; i < world.loaded_chunks.size(); i++) {
			Draw_partial_chunk(camera.UL_world(), camera.BR_world(), world.loaded_chunks[i], (decal_scale * camera.downscaler).ceil());
		}

		olc::vf2d player_screen_pos = camera.world_to_screen(player.GetPos()) 
			* olc::vf2d{ (float)w,(float)h };
		DrawPartialDecal(player_screen_pos, decal_scale,
			decPlayer.get(), olc::vf2d{16.0, 16.0 }*(player.GetFacing()+olc::vi2d{1,1}), olc::vf2d{16.0,16.0});
	}
	void UpdateInputs(InputList& inputs) {
		inputs.player_walk_up = GetKey(olc::Key::W).bHeld;
		inputs.player_walk_down = GetKey(olc::Key::S).bHeld;
		inputs.player_walk_left = GetKey(olc::Key::A).bHeld;
		inputs.player_walk_right = GetKey(olc::Key::D).bHeld;
		inputs.player_is_sprinting = GetKey(olc::Key::SHIFT).bHeld;
	}
	
public:
	bool OnUserCreate() override
	{
		// Load Sprite
		sprTiles = std::make_unique<olc::Sprite>("./assets/tiles.png");
		decTiles = std::make_unique<olc::Decal>(sprTiles.get());
		sprPlayer = std::make_unique<olc::Sprite>("./assets/player.png");
		decPlayer = std::make_unique<olc::Decal>(sprPlayer.get());

		//world.create("cock", 0);
		world.create("cock",0);
		//world.load_chunk({ 0,0 });
		

		return true;
	}

	bool OnUserUpdate(const float fElapsedTime) override
	{
		UpdateInputs(inputs);
		player.Tick(inputs, fElapsedTime);
		
		world.update_chunks(player.GetPos());

		
		olc::vi2d mouse_on_screen = GetMousePos();
		olc::vf2d mouse_on_screen_scale = olc::vf2d{ (float)mouse_on_screen.x / w,(float)mouse_on_screen.y / h };
		olc::vf2d mouse_in_world = camera.screen_to_world(mouse_on_screen_scale);
		if (GetMouseWheel() < 0) {
			camera.zoom(1.1, mouse_in_world);
		}
		if (GetMouseWheel() > 0) {
			camera.zoom(1/1.1, mouse_in_world);
		}

		camera.set_position(player.GetPos() + olc::vf2d(0.5,0.5)); // +0.5 to center on player center
		Drawer();
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
