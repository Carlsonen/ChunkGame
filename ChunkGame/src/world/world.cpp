#pragma once
#include "world/World.h"

void createfolder(std::string path) {
	std::wstring stemp = std::wstring(path.begin(), path.end());
	LPCWSTR sw = stemp.c_str();

	if (CreateDirectory(sw, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		std::cout << "success:  " << path << std::endl;
	}
	else {
		std::cout << "fail:  " << path << std::endl;
	}
}

void World::load(std::string world_name) {
	// make sure folders exists
	std::string filepath = "./worlds";
	createfolder(filepath);
	filepath += "/" + world_name;
	createfolder(filepath);
	// load random shit
	
}

void Chunk::generate(olc::vi2d pos) {
	tiledata = new uint8_t[size * size];
	for (int i = 0; i < size * size; i++) {
		tiledata[i] = 0;
	}
	position = pos;

	std::cout << "generation complete\n";
}
void Chunk::load(olc::vi2d pos, std::string world_name) {
	std::string filepath = "./worlds/" + world_name + "/" + pos.str() + ".chonk";

	if (exists_test(filepath)) {
		std::fstream infile(filepath, std::ios::binary | std::ios::in);

		// read tile data
		tiledata = new uint8_t[size * size];
		infile.read((char*)tiledata, size * size * sizeof(uint8_t));
		position = pos;

		infile.close();
		std::cout << "load complete\n";
	}
	else {
		generate(pos);
	}
}
void Chunk::save(std::string world_name) {
	std::string filepath = "./worlds/" + world_name;
	filepath += "/" + position.str() + ".chonk";

	std::fstream outfile(filepath, std::ios::binary | std::ios::out);

	// write world data to file
	outfile.write((char*)tiledata, size * size * sizeof(uint8_t));

	outfile.close();
}


