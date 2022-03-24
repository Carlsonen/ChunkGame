#include "camera/Camera.h"

Camera::Camera() {
	position = olc::vf2d{ 0,0 };
	view_dimensions = olc::vf2d{ 16,16 };
	downscaler = 1;
}
void Camera::set_position(olc::vf2d pos) {
	position = pos;
}
void Camera::move_position(olc::vf2d movement) {
	position += movement;
}
void Camera::set_view(olc::vf2d dims) {
	view_dimensions = dims;
	
}
void Camera::zoom(float z) {
	view_dimensions = shit::vfClamp(
		view_dimensions*z,
		olc::vf2d{4.0,4.0},
		olc::vf2d{1024.0,1024.0});
	float x = view_dimensions.x;
	if (x < 64) downscaler = 1;
	else if (x < 255) downscaler = 2;
	else if (x < 1024) downscaler = 4;
	else downscaler = 8;
	std::cout << "downscaler: " << downscaler << std::endl;
}
olc::vf2d Camera::UL_world() {
	return position - view_dimensions / 2;
}
olc::vf2d Camera::BR_world() {
	return position + view_dimensions / 2;
}
olc::vf2d Camera::world_to_screen(olc::vf2d world) {
	return (world - UL_world()) / view_dimensions;
}
olc::vf2d Camera::screen_to_world(olc::vi2d screen) {
	return UL_world() + screen * view_dimensions;
}
void Camera::save(std::fstream outfile) {
	// write position
	outfile.write((char*)&position, sizeof(olc::vf2d));
	// write view_dimenstions
	outfile.write((char*)&view_dimensions, sizeof(olc::vf2d));
}
void Camera::load(std::fstream infile) {
	// load position
	infile.read((char*)&position, sizeof(uint64_t));
	// load view_dimensions
	infile.read((char*)&view_dimensions, sizeof(uint64_t));
}

