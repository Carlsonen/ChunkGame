#pragma once

#include "FastNoiseLite.h"
#include "main/olcPixelGameEngine.h"
#include "player.cpp"
#include "sprIndexDefine.h"
#include "gameStuff.h"

#include <chrono>
#include <fstream>
#include <sys/stat.h>
#include <string>
#include "goodieFuncs.h"

class World {
public:
	int seed = 0;
	olc::vi2d size;
	char* data;
	char* objectData;
	std::string worldFileName;
	
public:
	void GenerateWorld(std::string filename, olc::vi2d worldSize, int _seed) {
		size = worldSize;
		seed = _seed;
		worldFileName = filename;
		data = new char[size.x * size.y];
		objectData = new char[size.x * size.y];
		//settings
		float biomeFactor = 0.3f;
		float baseFactor = 1;
		float noiseFactor = 8;
		
		FastNoiseLite noise;
		noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		noise.SetSeed(seed);
		srand(seed);

		//base layer
		for (int x = 0; x < size.x; x++)
		{
			for (int y = 0; y < size.y; y++)
			{
				float biome = noise.GetNoise((float)x * biomeFactor, (float)y * biomeFactor);
				float base = noise.GetNoise((float)x * baseFactor, (float)y * baseFactor);
				float noiseLayer = noise.GetNoise((float)x * noiseFactor, (float)y * noiseFactor);

				float val = (biome*0 + base*0 + noiseLayer) / 1 + 1; // generates 0 - 2

				if (val < 0.15) data[x * size.x + y] = sprTile_TILLED_DIRT;
				else if (val < 0.8) data[x * size.x + y] = sprTile_DIRT;
				else if (val < 1.6) data[x * size.x + y] = sprTile_GRASS;
				else data[x * size.x + y] = sprTile_GRASS2;
			}
		}

		//stone
		for (int i = 0; i < size.x * size.y; i++) {
			if (rand() % 16 == 0 && data[i] == 0) {
				data[i] = sprTile_STONE;
			}
		}

		//objects
		for (int i = 0; i < size.x * size.y; i++) {
			int rng = rand() % 103;
			char tile = data[i];
			if (rng == 0 && tile >= sprTile_GRASS) objectData[i] = sprObject_STICK;
			else if (rng == 1 && tile < sprTile_GRASS) objectData[i] = sprObject_SMALL_ROCK;
			else if (rng == 2 && tile >= sprTile_GRASS) objectData[i] = sprObject_TREE;
			else if (rng == 3 && tile < sprTile_GRASS) objectData[i] = sprObject_ROCK;
			else objectData[i] = 0;
		}
		std::cout << filename << " was generated" << std::endl;

		Player p; //new empty player
		Camera c; //new empty camera
		SaveWorld(filename, p, c);
	}
	void SaveWorld(std::string filename, Player& player, Camera& camera) {
		// open the file in binary output mode
		std::fstream outfile(filename, std::ios::binary | std::ios::out);

		camera.Save(outfile);

		// write size x to file
		outfile.write((char*)(&size.x), sizeof(size.x));

		// write size y to file
		outfile.write((char*)(&size.y), sizeof(size.y));

		// write world data to file
		outfile.write(data, size.x * size.y * sizeof(char));

		// write worldObjects data to file
		outfile.write(objectData, size.x * size.y * sizeof(char));

		player.Save(outfile);

		// close the file
		outfile.close();
		std::cout << filename << " Saved!\n";
	}
	void LoadWorld(std::string filename, Player& player, Camera& camera) {
		if (exists_test(filename)) {
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			
			std::fstream infile(filename, std::ios::binary | std::ios::in);

			camera.Load(infile);

			// read size x
			infile.read((char*)(&size.x), sizeof(size.x));

			// read size y
			infile.read((char*)(&size.y), sizeof(size.y));

			// allocate space for world data
			delete[] data;
			data = new char[size.x * size.y];

			// read world data
			infile.read(data, size.x * size.y * sizeof(char));

			// allocate space for worldObjects data
			delete[] objectData;
			objectData = new char[size.x * size.y];

			// read worldObjects data
			infile.read(objectData, size.x * size.y * sizeof(char));


			player.Load(infile);


			
			// close file
			infile.close();


			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			worldFileName = filename;
			std::cout << worldFileName << " loaded in ";
			std::cout << (float)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000 << "ms" << std::endl;
		}
		else {
			GenerateWorld(filename, { 256,256 }, (int)filename[0]);
			LoadWorld(filename, player, camera);
			std::cout << filename << " was not found and was generated" << std::endl;
		}
		
	}
	void SwitchWorld(std::string filename, Player& player, Camera& camera) {
		SaveWorld(worldFileName, player, camera);
		LoadWorld(filename, player, camera);
	}
	//check for file (copied from god knows where)
	inline bool exists_test(const std::string name) {
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}

};



