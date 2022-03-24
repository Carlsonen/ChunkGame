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
	int dist_from_point_max(olc::vi2d point);
	olc::vi2d get_id();
	uint8_t get_tile(int x, int y) {
		return tiledata[y * size + x];
	}
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
	bool is_chunk_loaded(olc::vi2d id);
	void update_chunks(olc::vi2d player_pos);
public:
	std::string name;
	uint64_t seed;
	uint64_t world_tick;

	std::vector<Chunk*> loaded_chunks;
};



