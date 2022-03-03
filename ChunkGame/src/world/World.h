#pragma once
#include "FastNoiseLite.h"
#include "main/olcPixelGameEngine.h"
#include <chrono>
#include <fstream>
#include <sys/stat.h>
#include <string>

class World {
	std::vector<Chunk> loaded_chunks;
};

class Chunk {
	olc::vi2d position;
};