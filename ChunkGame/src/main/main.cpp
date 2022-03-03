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

using namespace std;

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
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
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
