#pragma once

class soPostureModuleImpl {
public:
	char _spacer[0xC];
	//0xC
	float xPos;
	//0x10
	float yPos;
	//0x14
	float zPos;
	//0x18
	float prevXPos;
	//0x1C
	float prevYPos;
	//0x20
	float prevZPos;
	char _spacer2[0x3C - 0x20 - 4];

	//0x3C
	float scale;

	//0x40
	//right = 1.0, left = -1.0
	float direction;

	//0x5C
	//a pointer to a vector with Rotations

	char _spacer3[0x68 - 0x40 - 4];

	//0x68
	float ownerScale;
};