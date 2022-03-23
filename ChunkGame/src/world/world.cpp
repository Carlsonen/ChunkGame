#pragma once
#include "world/World.h"

void Chunk::generate(olc::vi2d pos, uint64_t seed) {
	position = pos;

	tiledata = new uint8_t[size * size];
	for (int i = 0; i < size * size; i++) {
		tiledata[i] = (seed + i) % 3;
	}
	
	
}
Chunk::Chunk(olc::vi2d pos, std::string world_name, uint64_t seed) {
	std::string filepath = "./worlds/" + world_name + "/chunks/" + pos.str() + ".chonk";

	if (shit::file_exists_test(filepath)) {
		// - (1) read tiledata
		
		// create file
		std::fstream infile(filepath, std::ios::binary | std::ios::in);

		// (1)
		// read tile data
		tiledata = new uint8_t[size * size];
		infile.read((char*)tiledata, size * size * sizeof(uint8_t));
		position = pos;

		infile.close();
		std::cout << "loaded " << filepath <<  std::endl;
	}
	else {
		generate(pos, seed);
		std::cout << "generated " << filepath <<  std::endl;
	}
}
Chunk::~Chunk() {
	delete[] tiledata;
}
void Chunk::save(std::string world_name) {
	// - (1) write tiledata to file

	// create file
	std::string filepath = "./worlds/" + world_name
		+ "/chunks/" + position.str() + ".chonk";
	std::fstream outfile(filepath, std::ios::binary | std::ios::out);

	// (1)
	// write tiledata to file
	outfile.write((char*)tiledata, size * size * sizeof(uint8_t));
	std::cout << "saved " << filepath << std::endl;
	outfile.close();
}
int Chunk::dist_from_point_max(olc::vi2d point) {
	olc::vi2d radius = { size / 2,size / 2 };
	olc::vi2d center = position * size + radius;

	olc::vi2d d = center - point;
	d.x = abs(d.x);
	d.y = abs(d.y);
	d -= radius;
	d = d.max({ 0,0 });
	return std::max(d.x, d.y);
}
olc::vi2d Chunk::get_id() {
	return position;
}


void World::create(std::string world_name, uint64_t _seed) {
	// - (1) create folder for world
	// - (2) initialize values

	// (1)
	std::string filepath = "./worlds";
	shit::createfolder(filepath);
	filepath += "/" + world_name;
	shit::createfolder(filepath);
	filepath += "/chunks";
	shit::createfolder(filepath);

	// (2)
	name = world_name;
	seed = _seed;
	world_tick = 0;
}
void World::load(std::string world_name) {
	std::string filepath = "./worlds/" + world_name + "/meta.data";
	if (shit::file_exists_test(filepath)) {
		// create file
		std::fstream metadata(filepath, std::ios::binary | std::ios::in);
		// load name
		name = shit::load_string(metadata);
		// load seed
		metadata.read((char*)&seed, sizeof(uint64_t));
		// load world_tick
		metadata.read((char*)&world_tick, sizeof(uint64_t));

		metadata.close();
		std::cout << "it might have loaded correctly\n";
	}
	else {
		std::cout << "fucking noob it doesnt exist\n";
	}
}
void World::save() {
	// - (1) save world metadata in meta.data
	// - (2) save all chunks in <pos>.chonk files

	// (1)
	// create file
	std::string filepath = "./worlds/" + name + "/meta.data";
	std::fstream metadata(filepath, std::ios::binary | std::ios::out);
	// write name
	shit::write_string(metadata, name);
	// write seed
	metadata.write((char*)&seed, sizeof(uint64_t));
	// write world_tick
	metadata.write((char*)&world_tick, sizeof(world_tick));
	metadata.close();

	// (2) 
	// write all loaded chunks
	for (int i = 0; i < loaded_chunks.size(); i++) loaded_chunks[i]->save(name);

}
void World::load_chunk(olc::vi2d pos) {
	loaded_chunks.push_back(new Chunk(pos,name,seed));
}
bool World::is_chunk_loaded(olc::vi2d id) {
	for (int i = 0; i < loaded_chunks.size(); i++) {
		if (loaded_chunks[i]->get_id() == id) {
			return true;
		}
	}
	return false;
}
void World::update_chunks(olc::vi2d player_pos) {
	for (int i = 0; i < loaded_chunks.size(); i++) {
		Chunk* chunk = loaded_chunks[i];
		if (chunk->dist_from_point_max(player_pos) > 100) {
			chunk->save(name);
			delete chunk;
			loaded_chunks.erase(loaded_chunks.begin() + i);
			return;
		}
	}
	olc::vi2d id = shit::to_chunk(player_pos + olc::vi2d{ -64, -64 }, 256);
	if (!is_chunk_loaded(id)) {
		load_chunk(id);
		return;
	}
	id = shit::to_chunk(player_pos + olc::vi2d{ 64, -64 }, 256);
	if (!is_chunk_loaded(id)) {
		load_chunk(id);
		return;
	}
	id = shit::to_chunk(player_pos + olc::vi2d{ -64, 64 }, 256);
	if (!is_chunk_loaded(id)) {
		load_chunk(id);
		return;
	}
	id = shit::to_chunk(player_pos + olc::vi2d{ 64, 64 }, 256);
	if (!is_chunk_loaded(id)) {
		load_chunk(id);
		return;
	}
}

olc::vi2d shit::to_chunk(olc::vf2d pos, int32_t chunk_size) {
	return (pos / chunk_size).floor();
}


