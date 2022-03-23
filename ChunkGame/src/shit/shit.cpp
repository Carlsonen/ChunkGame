#include "shit/shit.h"

void shit::write_string(std::fstream& file, std::string& str) {
	uint64_t lenght = str.length();
	file.write((char*)&lenght, sizeof(uint64_t));
	file.write(str.c_str(), lenght);
}

std::string shit::load_string(std::fstream& file) {
	uint64_t size;
	file.read((char*)(&size), sizeof(uint64_t));

	char* in_str = new char[size+1];
	file.read(in_str, size);
	in_str[size] = '\0';
	std::string s(in_str);
	return s;
}

void shit::createfolder(std::string path) {
	std::wstring stemp = std::wstring(path.begin(), path.end());
	LPCWSTR sw = stemp.c_str();

	if (CreateDirectory(sw, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		std::cout << "success:  " << path << std::endl;
	}
	else {
		std::cout << "fail:  " << path << std::endl;
	}
}

bool shit::file_exists_test(const std::string name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}