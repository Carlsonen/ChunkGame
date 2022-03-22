#pragma once
#include "FastNoiseLite.h"
#include "main/olcPixelGameEngine.h"
#include <chrono>
#include <fstream>
#include <sys/stat.h>
#include <string>
#include "shit/shit.h"


class Chunk {
public:
	// load or generate if non existant
	Chunk(olc::vi2d pos, std::string world_name, uint64_t seed);
	~Chunk();
	// save
	void save(std::string world_name);
private:
	void generate(olc::vi2d pos, uint64_t seed);
	const int size = 256;
	olc::vi2d position;
	uint8_t* tiledata;
};

class World {
public:
	void create(std::string world_name, uint64_t _seed);
	void load(std::string world_name);
	void save();
	void load_chunk(olc::vi2d pos);
public:
	std::string name;
	uint64_t seed;
	uint64_t world_tick;

	std::vector<Chunk*> loaded_chunks;
};



