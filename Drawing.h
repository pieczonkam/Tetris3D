#pragma once

struct Drawing
{
    static void initOpenGL();
    static void initImGui(sf::RenderWindow& window);
    static void reshapeScreen(sf::RenderWindow& window);
    static void setPerspective(float fov, float aspect_ratio, float z_min, float z_max);
    static sf::Vector2f getTexPos(char color);
    static sf::Vector2f getTexSize();
    static void setButtonInactive();
    static void setButtonActive();

    static void drawScene(Spherical& camera01, Spherical& camera02, sf::Vector2u size, Block& current_block, Block& hint_block, Block& next_block);
    static void drawGameFrame(Spherical& camera, sf::Vector2u size, Block& current_block, Block& hint_block);
    static void drawMenu(Spherical& camera, sf::Vector2u size, Block& current_block, Block& next_block);
    static void drawCuboid(sf::Vector3f pos, sf::Vector3f size, sf::Vector2f tex_pos, sf::Vector2f tex_size);
    static void drawCuboidWireframe(sf::Vector3f pos, sf::Vector3f size, sf::Vector3f color, float thickness = 1.0f, bool is_offset = false);
    static void drawBlock(Block& block, bool wireframe = false);
    static void drawGrid();

    static sf::Texture block_texture;
};
