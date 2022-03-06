#pragma once
#include "FastNoiseLite.h"
#include "main/olcPixelGameEngine.h"
#include<windows.h>
#include <chrono>
#include <fstream>
#include <sys/stat.h>
#include <string>

class Chunk {
public:
	void load(olc::vi2d pos, std::string world_name);
	void save(std::string world_name);
private:
	void generate(olc::vi2d pos);
	olc::vi2d position;
	int size = 64;
	uint8_t* tiledata;
};

class World {
public:
	void load(std::string world_name);
	std::vector<Chunk*> loaded_chunks;
};

inline bool exists_test(const std::string name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

