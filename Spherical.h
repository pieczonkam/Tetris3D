#pragma once

struct Spherical
{
    Spherical(float _distance, float _theta, float _phi);
    Spherical(sf::Vector3f coords);
    float getX();
    float getY();
    float getZ();

    float distance, theta, phi;
};