#pragma once

struct Grid
{
	static void init();
	static void reset();
	static void clear();

	static void setBlock(Block& block);
	static bool checkCollision(Block& block);
	static void setHintY(Block& block);
	static bool isGameOver();
	static bool isFloorFull(int y);
	static std::vector<int> getFullFloors();
	static void moveBlocksDown(int _y);

	static std::vector<std::vector<std::vector<std::pair<char, sf::Vector3f>>>> color;
	static std::vector<std::vector<std::vector<bool>>> map;
};