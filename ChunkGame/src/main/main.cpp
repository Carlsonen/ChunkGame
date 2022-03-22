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


//screen setup
const int w = 960;
const int h = 960;
const int pixelSize = 1;


class MainGameObject : public olc::PixelGameEngine
{
public:
	MainGameObject() { sAppName = "The name of the app"; }
	
	void Drawer() {
		Clear(olc::Pixel::Pixel());
	}
	
public:
	bool OnUserCreate() override
	{
		World world;
		world.create("cock", 0xf0f0f0f0f0f0f0f0);
		world.load_chunk({ 0,0 });
		world.save();
		std::cout << "seed nuts " << world.seed << "\n";
		world.load("cock");
		std::cout << "seed nuts " << world.seed << "\n";
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//Drawer();

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
