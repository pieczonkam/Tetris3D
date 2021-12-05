#include "pch.h"

// SFML to GL
void Utils::glColorsf(sf::Vector3f v)
{
    glColor3f(v.x, v.y, v.z);
}

void Utils::glVertexsf(sf::Vector3f v)
{
    glVertex3f(v.x, v.y, v.z);
}
