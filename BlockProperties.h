#pragma once

struct BlockProperties
{
	static void init();
	static void clear();
	static std::pair<char, std::vector<std::vector<std::vector<bool>>>> getRandomMap();
	static std::pair<char, sf::Vector3f> getRandomColor();

	// Maps
	static std::pair<char, std::vector<std::vector<std::vector<bool>>>> O;
	static std::pair<char, std::vector<std::vector<std::vector<bool>>>> I;
	static std::pair<char, std::vector<std::vector<std::vector<bool>>>> T;
	static std::pair<char, std::vector<std::vector<std::vector<bool>>>> L;
	static std::pair<char, std::vector<std::vector<std::vector<bool>>>> J;
	static std::pair<char, std::vector<std::vector<std::vector<bool>>>> S;
	static std::pair<char, std::vector<std::vector<std::vector<bool>>>> Z;
	static std::vector< std::pair<char, std::vector<std::vector<std::vector<bool>>>>> map_list;

	// Colors
	static std::pair<char, sf::Vector3f> red;
	static std::pair<char, sf::Vector3f> yellow;
	static std::pair<char, sf::Vector3f> orange;
	static std::pair<char, sf::Vector3f> blue;
	static std::pair<char, sf::Vector3f> magenta;
	static std::pair<char, sf::Vector3f> green;
	static std::pair<char, sf::Vector3f> cyan;
	static std::vector<std::pair<char, sf::Vector3f>> color_list;
};