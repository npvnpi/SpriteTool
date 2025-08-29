#pragma once
enum SpriteMenu : int
{
	ID_FILE_OPEN = 1001,
	ID_REMOVE_COLOR = 1002,
	ID_FILE_SAVE = 1003
};

struct Image32 
{
	UINT width = 0, height = 0, stride = 0;         // stride = width*4
	std::vector<uint8_t> pixels;              // BGRA, straight alpha
};