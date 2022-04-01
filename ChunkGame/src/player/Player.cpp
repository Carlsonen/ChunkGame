#include "player/Player.h"

Player::Player() {
	position = { 0,0 };
	facing_vector = { 0,1 };
}
void Player::Tick(InputList& inputs, float fElapsedTime) {
	
	olc::vi2d tmp = { 0,0 };
	tmp.x = inputs.player_walk_right - inputs.player_walk_left;
	tmp.y = inputs.player_walk_down - inputs.player_walk_up;
	if (tmp != olc::vi2d{ 0, 0 }) {
		facing_vector = tmp;
		float move_speed = 10 + 30 * inputs.player_is_sprinting;
		position += olc::vf2d(facing_vector).norm() * fElapsedTime * move_speed;
	}
}
olc::vf2d Player::GetPos() { return position; }
olc::vi2d Player::GetFacing() { return facing_vector; }