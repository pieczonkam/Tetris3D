#include "pch.h"

Block::Block(sf::Vector3f _pos, float _size, char _type) : pos_init(_pos), pos(_pos), size(sf::Vector3f(_size, _size, _size)), type(_type)
{
	dx_left = dx_right = size.x;
	dz_left = dz_right = size.z;
	dy = 0.0f;
	setNew();
}

void Block::resetPos()
{
	pos = pos_init;
	// Adjust block's position (since different blocks have maps of different sizes)
	if (id == 'I') { pos.x -= size.x; pos.y -= 2 * size.y; pos.z -= size.z; }
	else if (id != 'O') pos.y -= size.y;
	if (type == 'n')
		if (id == 'O' || id == 'I') pos.x += size.x / 2.0f;
}

void Block::setNew() // Reset block's properties
{	
	std::pair<char, std::vector<std::vector<std::vector<bool>>>> _map = BlockProperties::getRandomMap();
	id = _map.first;
	map = _map.second;
	color = BlockProperties::getRandomColor();
	resetPos();
}

void Block::copy(Block& block, bool with_pos) // Copy block's properties from another block
{
	dx_left = dx_right = size.x;
	dz_left = dz_right = size.z;
	dy = 0.0f;
	id = block.id;
	map = block.map;
	color = block.color;
	if (with_pos) pos = block.pos;
	else resetPos();
}

void Block::rotate(char axis)
{
	int x_size = (int)map[0][0].size(), y_size = (int)map[0].size(), z_size = (int)map.size();
	int x_size_half = x_size / 2, y_size_half = y_size / 2, z_size_half = z_size / 2;
	int x_new = 0, y_new = 0, z_new = 0, i, j, k, i_new, j_new, k_new;
	std::vector<std::vector<std::vector<bool>>> block_map, prev_map = map, tmp;

	// If block is 'O' or 'I', temporary map of odd size needs to be created so that rotation is done correctly
	if (id == 'O' || id == 'I')
	{
		++x_size; ++y_size; ++z_size;
		for (int i = 0, z = 0; z < z_size; ++i, ++z)
		{
			if (z == z_size_half) --i;
			block_map.push_back(std::vector<std::vector<bool>>{});			
			{ 
				for (int j = 0, y = 0; y < y_size; ++j, ++y)
				{
					if (y == z_size_half) --j;
					block_map[z].push_back(std::vector<bool>{});
					for (int k = 0, x = 0; x < x_size; ++k, ++x)
					{
						if (x == x_size_half) --k;
						if (x == x_size_half || y == y_size_half || z == z_size_half) block_map[z][y].push_back(0);
						else block_map[z][y].push_back(map[i][j][k]);
					}
				}
			}
		}
	}
	else block_map = map;
	tmp = block_map;
	
	// Rotation is done around center element of 3D matrix
	for (int z = -z_size_half; z <= z_size_half; ++z)
		for (int y = -y_size_half; y <= y_size_half; ++y)
			for (int x = -x_size_half; x <= x_size_half; ++x)
			{
				if (axis == 'X') { x_new = x; y_new = -z; z_new = y; }
				else if (axis == 'Y') { x_new = z; y_new = y; z_new = -x; }
				else { x_new = -y; y_new = x; z_new = z; }
				
				i = z + z_size_half; j = y + y_size_half; k = x + x_size_half;
				i_new = z_new + z_size_half; j_new = y_new + y_size_half; k_new = x_new + x_size_half;
				block_map[i_new][j_new][k_new] = tmp[i][j][k];
			}

	// Rewriting map
	if (id == 'O' || id == 'I')
	{
		for (int i = 0, z = 0; z < z_size; ++i, ++z)
		{
			if (z == z_size_half) { --i;  continue; }
			for (int j = 0, y = 0; y < y_size; ++j, ++y)
			{
				if (y == y_size_half) { --j;  continue; }
				for (int k = 0, x = 0; x < x_size; ++k, ++x)
				{
					if (x == x_size_half) { --k; continue; }
					map[i][j][k] = block_map[z][y][x];
				}
			}
		}
	}
	else map = block_map;
	// Restore previous state of map if current rotation causes collision
	if (Grid::checkCollision(*this)) map = prev_map;
}

void Block::move(char axis, float distance, char direction, bool reverse)
{
	float sign = reverse ? -1.0f : 1.0f;

	if (axis == 'x')
	{
		if (direction == 'l') // Move left
		{
			dx_left += distance; // Block is moved in every frame (when adequate button is pressed) by a small, constant value but position is updated only if dx_left is greater than block's size
			if (dx_left >= size.x)
			{
				pos.x -= sign * size.x;
				if (Grid::checkCollision(*this)) pos.x += sign * size.x; // If current position causes collision, previous position is restored
				dx_left = 0.0f;
			}
		}
		else // Move right
		{
			dx_right += distance;
			if (dx_right >= size.x)
			{
				pos.x += sign * size.x;
				if (Grid::checkCollision(*this)) pos.x -= sign * size.x;
				dx_right = 0.0f;
			}
		}
	}
	else if (axis == 'z')
	{
		if (direction == 'l')
		{
			dz_left += distance;
			if (dz_left >= size.z)
			{
				pos.z -= sign * size.z;
				if (Grid::checkCollision(*this)) pos.z += sign * size.z;
				dz_left = 0.0f;
			}
		}
		else
		{
			dz_right += distance;
			if (dz_right >= size.z)
			{
				pos.z += sign * size.z;
				if (Grid::checkCollision(*this)) pos.z -= sign * size.z;
				dz_right = 0.0f;
			}
		}
	}
	else
	{
		dy += distance;
		if (dy >= size.y)
		{
			pos.y -= size.y;
			dy = 0.0f;
		}
	}
}