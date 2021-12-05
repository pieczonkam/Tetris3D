#include "pch.h"

void BlockProperties::init()
{
	O.second =	{
					{
						{ 0, 0 },
						{ 0, 0 }
					},
					{
						{ 1, 1 },
						{ 1, 1 }
					}
				};

	I.second =	{
					{
						{ 0, 0, 0, 0 },
						{ 0, 0, 0, 0 },
						{ 0, 0, 0, 0 },
						{ 0, 0, 0, 0 }
					},
					{
						{ 0, 0, 0, 0 },
						{ 0, 0, 0, 0 },
						{ 0, 0, 0, 0 },
						{ 0, 0, 0, 0 }
					},
					{
						{ 0, 0, 0, 0 },
						{ 0, 0, 0, 0 },
						{ 1, 1, 1, 1 },
						{ 0, 0, 0, 0 }
					},
					{
						{ 0, 0, 0, 0 },
						{ 0, 0, 0, 0 },
						{ 0, 0, 0, 0 },
						{ 0, 0, 0, 0 }
					}
				};

	T.second =	{
					{
						{ 0, 0, 0 },
						{ 0, 0, 0 },
						{ 0, 0, 0 }
					},
					{
						{ 0, 0, 0 },
						{ 1, 1, 1 },
						{ 0, 1, 0 }
					},
					{
						{ 0, 0, 0 },
						{ 0, 0, 0 },
						{ 0, 0, 0 }
					}
				};

	L.second =	{
					{
						{ 0, 0, 0 },
						{ 0, 0, 0 },
						{ 0, 0, 0 }
					},
					{
						{ 0, 0, 0 },
						{ 1, 1, 1 },
						{ 1, 0, 0 }
					},
					{
						{ 0, 0, 0 },
						{ 0, 0, 0 },
						{ 0, 0, 0 }
					}
				};

	J.second =	{
					{
						{ 0, 0, 0 },
						{ 0, 0, 0 },
						{ 0, 0, 0 }
					},
					{
						{ 0, 0, 0 },
						{ 1, 1, 1 },
						{ 0, 0, 1 }
					},
					{
						{ 0, 0, 0 },
						{ 0, 0, 0 },
						{ 0, 0, 0 }
					}
				};

	S.second =	{
					{
						{ 0, 0, 0 },
						{ 0, 0, 0 },
						{ 0, 0, 0 }
					},
					{
						{ 0, 0, 0 },
						{ 0, 1, 1 },
						{ 1, 1, 0 }
					},
					{
						{ 0, 0, 0 },
						{ 0, 0, 0 },
						{ 0, 0, 0 }
					}
				};

	Z.second =	{
					{
						{ 0, 0, 0 },
						{ 0, 0, 0 },
						{ 0, 0, 0 }
					},
					{
						{ 0, 0, 0 },
						{ 1, 1, 0 },
						{ 0, 1, 1 }
					},
					{
						{ 0, 0, 0 },
						{ 0, 0, 0 },
						{ 0, 0, 0 }
					}
				};
	
	map_list = { O, I, T, L, J, S, Z };
	color_list = { yellow, red, green, magenta, orange, cyan, blue };
}

void BlockProperties::clear()   // Vectors and arrays need to be cleared because either opengl or sfml prevent their destructors to be called, which causes memory leaks
								// (this solution is not perfect as there are still some minor memory leaks but it mitigates damage)
{
	O.second = std::vector<std::vector<std::vector<bool>>>{};
	I.second = std::vector<std::vector<std::vector<bool>>>{};
	T.second = std::vector<std::vector<std::vector<bool>>>{};
	L.second = std::vector<std::vector<std::vector<bool>>>{};
	J.second = std::vector<std::vector<std::vector<bool>>>{};
	S.second = std::vector<std::vector<std::vector<bool>>>{};
	Z.second = std::vector<std::vector<std::vector<bool>>>{};

	map_list = std::vector<std::pair<char, std::vector<std::vector<std::vector<bool>>>>>{};
	color_list = std::vector<std::pair<char, sf::Vector3f>>{};
}

std::pair<char, std::vector<std::vector<std::vector<bool>>>> BlockProperties::getRandomMap()
{
	return map_list[rand() % (int)map_list.size()];
}

std::pair<char, sf::Vector3f> BlockProperties::getRandomColor()
{
	return color_list[rand() % (int)color_list.size()];
}

// Maps
std::pair<char, std::vector<std::vector<std::vector<bool>>>> BlockProperties::O{ 'O', {} };
std::pair<char, std::vector<std::vector<std::vector<bool>>>> BlockProperties::I{ 'I', {} };
std::pair<char, std::vector<std::vector<std::vector<bool>>>> BlockProperties::T{ 'T', {} };
std::pair<char, std::vector<std::vector<std::vector<bool>>>> BlockProperties::L{ 'L', {} };
std::pair<char, std::vector<std::vector<std::vector<bool>>>> BlockProperties::J{ 'J', {} };
std::pair<char, std::vector<std::vector<std::vector<bool>>>> BlockProperties::S{ 'S', {} };
std::pair<char, std::vector<std::vector<std::vector<bool>>>> BlockProperties::Z{ 'Z', {} };
std::vector<std::pair<char, std::vector<std::vector<std::vector<bool>>>>> BlockProperties::map_list;

// Colors
std::pair<char, sf::Vector3f> BlockProperties::yellow{ 'y', sf::Vector3f{ 1.0f, 0.855f, 0.0f } };
std::pair<char, sf::Vector3f> BlockProperties::red{ 'r', sf::Vector3f{ 1.0f, 0.0f, 0.004f } };
std::pair<char, sf::Vector3f> BlockProperties::green{ 'g', sf::Vector3f{ 0.0f, 0.922f, 0.0f } };
std::pair<char, sf::Vector3f> BlockProperties::magenta{ 'm', sf::Vector3f{ 0.933f, 0.0f, 1.0f } };
std::pair<char, sf::Vector3f> BlockProperties::orange{ 'o', sf::Vector3f{ 1.0f, 0.427f, 0.0f } };
std::pair<char, sf::Vector3f> BlockProperties::cyan{ 'c', sf::Vector3f{ 0.0f, 0.843f, 0.988f } };
std::pair<char, sf::Vector3f> BlockProperties::blue{ 'b', sf::Vector3f{ 0.0f, 0.247f, 0.867f } };
std::vector<std::pair<char, sf::Vector3f>> BlockProperties::color_list{};
