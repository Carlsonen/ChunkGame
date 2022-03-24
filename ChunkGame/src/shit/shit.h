#pragma once
#include "main/olcPixelGameEngine.h"
#include<windows.h>
#include <fstream>
#include <string>
#include <iostream>

namespace shit {
	// file shit
	void write_string(std::fstream& file, std::string& str);
	std::string load_string(std::fstream& file);
	void createfolder(std::string path);
	bool file_exists_test(const std::string name);
	// math shit
	olc::vi2d to_chunk(olc::vf2d pos, int32_t chunk_size);
	olc::vi2d vClamp(olc::vi2d value, olc::vi2d vMin, olc::vi2d vMax);
}
 