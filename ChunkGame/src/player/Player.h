#include "main/olcPixelGameEngine.h"
#include "shit/inputs.h"

class Player {
public:
	Player();
	void Tick(InputList &inputs, float fElapsedTime);
	olc::vf2d GetPos();
	olc::vi2d GetFacing();
private:
	olc::vf2d position;
	olc::vi2d facing_vector;
};