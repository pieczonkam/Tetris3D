#include "pch.h"

void Grid::init()
{
	for (int z = 0; z < Settings::grid_block_size.z; ++z)
	{
		color.push_back(std::vector<std::vector<std::pair<char, sf::Vector3f>>>{});
		map.push_back(std::vector<std::vector<bool>>{});
		for (int y = 0; y < Settings::grid_block_size.y; ++y)
		{
			color[z].push_back(std::vector<std::pair<char, sf::Vector3f>>{});
			map[z].push_back(std::vector<bool>{});
			for (int x = 0; x < Settings::grid_block_size.x; ++x)
			{
				color[z][y].push_back(std::pair<char, sf::Vector3f>{ ' ', sf::Vector3f{ 0.0f, 0.0f, 0.0f } });
				if (z < Settings::grid_block_offset_size.z || z >= Settings::grid_block_size.z - Settings::grid_block_offset_size.z
					|| y < Settings::grid_block_offset_size.y
					|| x < Settings::grid_block_offset_size.x || x >= Settings::grid_block_size.x - Settings::grid_block_offset_size.x) map[z][y].push_back(1);
				else map[z][y].push_back(0);
			}
		}
	}
}

void Grid::reset()
{
	for (int z = 0; z < Settings::grid_block_size.z; ++z)
	{
		for (int y = 0; y < Settings::grid_block_size.y; ++y)
		{
			for (int x = 0; x < Settings::grid_block_size.x; ++x)
			{
				color[z][y][x] = std::pair<char, sf::Vector3f>{ ' ', sf::Vector3f{ 0.0f, 0.0f, 0.0f } };
				if (z < Settings::grid_block_offset_size.z || z >= Settings::grid_block_size.z - Settings::grid_block_offset_size.z
					|| y < Settings::grid_block_offset_size.y
					|| x < Settings::grid_block_offset_size.x || x >= Settings::grid_block_size.x - Settings::grid_block_offset_size.x) map[z][y][x] = 1;
				else map[z][y][x] = 0;
			}
		}
	}
}

void Grid::clear()  // Vectors and arrays need to be cleared because either opengl or sfml prevent their destructors to be called, which causes memory leaks
					// (this solution is not perfect as there are still some minor memory leaks but it mitigates damage)
{
	color = std::vector<std::vector<std::vector<std::pair<char, sf::Vector3f>>>>{};
	map = std::vector<std::vector<std::vector<bool>>>{};
}

void Grid::setBlock(Block& block)
{
	int x = std::lround((block.pos.x - Settings::game_frame_pos.x) / block.size.x) + Settings::grid_block_offset_size.x;
	int y = std::lround((block.pos.y - Settings::game_frame_pos.y) / block.size.y) + Settings::grid_block_offset_size.y;
	int z = std::lround((block.pos.z - Settings::game_frame_pos.z) / block.size.z) + Settings::grid_block_offset_size.z;

	int x_pos, y_pos, z_pos, x_size = (int)block.map[0][0].size(), y_size = (int)block.map[0].size(), z_size = (int)block.map.size();
	for (int i = 0; i < z_size; ++i)
		for (int j = 0; j < y_size; ++j)
			for (int k = 0; k < x_size; ++k)
			{
				x_pos = x + k; y_pos = y + j; z_pos = z + i;
				if (block.map[i][j][k])
				{
					color[z_pos][y_pos][x_pos] = block.color;
					map[z_pos][y_pos][x_pos] = 1;
				}
			}
}

bool Grid::checkCollision(Block& block)
{
	int x = std::lround((block.pos.x - Settings::game_frame_pos.x) / block.size.x) + Settings::grid_block_offset_size.x;
	int y = std::lround((block.pos.y - Settings::game_frame_pos.y) / block.size.y) + Settings::grid_block_offset_size.y;
	int z = std::lround((block.pos.z - Settings::game_frame_pos.z) / block.size.z) + Settings::grid_block_offset_size.z;

	int x_pos, y_pos, z_pos, x_size = (int)block.map[0][0].size(), y_size = (int)block.map[0].size(), z_size = (int)block.map.size();
	for (int i = 0; i < z_size; ++i)
		for (int j = 0; j < y_size; ++j)
			for (int k = 0; k < x_size; ++k)
			{
				x_pos = x + k; y_pos = y + j; z_pos = z + i;
				if (map[z_pos][y_pos][x_pos] && block.map[i][j][k]) return true;
			}
	return false;
}

void Grid::setHintY(Block& block) // Sets hint_block's position by moving it down as long as collision hasn't occured
{
	while (!checkCollision(block)) 
		block.pos.y -= Settings::block_size;
	block.pos.y += Settings::block_size;
}

bool Grid::isGameOver() // Game is over when set block sticks out of the game frame
{
	int y = Settings::game_frame_block_size.y + Settings::grid_block_offset_size.y;
	for (int z = Settings::grid_block_offset_size.z; z < Settings::grid_block_size.z - Settings::grid_block_offset_size.z; ++z)
		for (int x = Settings::grid_block_offset_size.x; x < Settings::grid_block_size.x - Settings::grid_block_offset_size.x; ++x)
			if (map[z][y][x]) return true;
	return false;
}

bool Grid::isFloorFull(int y)
{
	for (int z = Settings::grid_block_offset_size.z; z < Settings::grid_block_size.z - Settings::grid_block_offset_size.z; ++z)
		for (int x = Settings::grid_block_offset_size.x; x < Settings::grid_block_size.x - Settings::grid_block_offset_size.x; ++x)
			if (!map[z][y][x]) return false;
	return true;
}

std::vector<int> Grid::getFullFloors() // Returns vector of full floors' y-values in descending order
{
	std::vector<int> full_floors;
	for (int y = Settings::grid_block_size.y - Settings::grid_block_offset_size.y - 1; y >= Settings::grid_block_offset_size.y; --y)
		if (isFloorFull(y)) full_floors.push_back(y);
	return full_floors;
}

void Grid::moveBlocksDown(int _y) // Moves each block in grid's map from y == _y upwards
{
	int y, y_idx = _y + 1;
	for (y = _y; y < Settings::grid_block_size.y - Settings::grid_block_offset_size.y - 1; ++y, ++y_idx)
		for (int z = Settings::grid_block_offset_size.z; z < Settings::grid_block_size.z - Settings::grid_block_offset_size.z; ++z)
			for (int x = Settings::grid_block_offset_size.x; x < Settings::grid_block_size.x - Settings::grid_block_offset_size.x; ++x)
			{
				color[z][y][x] = color[z][y_idx][x];
				map[z][y][x] = map[z][y_idx][x];
			}
	for (int z = Settings::grid_block_offset_size.z; z < Settings::grid_block_size.z - Settings::grid_block_offset_size.z; ++z)
		for (int x = Settings::grid_block_offset_size.x; x < Settings::grid_block_size.x - Settings::grid_block_offset_size.x; ++x)
		{
			color[z][y][x] = std::pair<char, sf::Vector3f>{ ' ', sf::Vector3f{ 0.0f, 0.0f, 0.0f } };
			map[z][y][x] = 0;
		}
}

std::vector<std::vector<std::vector<std::pair<char, sf::Vector3f>>>> Grid::color{}; // Char value determines texture's color and Vector3f value determines color of edges
std::vector<std::vector<std::vector<bool>>> Grid::map{};