#pragma once
#include<windows.h>
#include <fstream>
#include <string>
#include <iostream>
namespace shit {
	void write_string(std::fstream& file, std::string& str);
	std::string load_string(std::fstream& file);
	void createfolder(std::string path);
	bool file_exists_test(const std::string name);
}
 