#pragma once

struct Block
{
	Block(sf::Vector3f _pos, float _size, char _type);
	void resetPos();
	void setNew();
	void copy(Block& block, bool with_pos = false);

	void rotate(char axis);
	void move(char axis, float distance, char direction = ' ', bool reverse = false);

	char id, type; // type: 'c' - blocks (current_block, hint_block) in game frame, 'n' - block in next block frame
	float dx_left, dx_right, dz_left, dz_right, dy;
	const sf::Vector3f pos_init;
	sf::Vector3f pos, size;
	std::pair<char, sf::Vector3f> color;
	std::vector<std::vector<std::vector<bool>>> map;
};
