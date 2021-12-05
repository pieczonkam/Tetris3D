#include "pch.h"

Spherical::Spherical(float _distance, float _theta, float _phi) : distance(_distance), theta(_theta), phi(_phi)
{ }

Spherical::Spherical(sf::Vector3f coords) : distance(coords.x), theta(coords.y), phi(coords.z)
{}

float Spherical::getX()
{
    return distance * cos(phi) * cos(theta);
}

float Spherical::getY()
{
    return distance * sin(phi);
}

float Spherical::getZ()
{
    return distance * cos(phi) * sin(theta);
}