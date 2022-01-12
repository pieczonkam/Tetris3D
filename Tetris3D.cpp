#include "pch.h"

typedef sf::Event sfe;
typedef sf::Keyboard sfk;

int main()
{
    // Memory leak check setup
    #ifdef _DEBUG
        _CrtMemState sOld;
        _CrtMemState sNew;
        _CrtMemState sDiff;
        _CrtMemCheckpoint(&sOld);
    #endif

    {
        srand((unsigned int)time(NULL));

        sf::RenderWindow window(sf::VideoMode(Settings::window_min_size.x, Settings::window_min_size.y), "Tetris 3D", 7U, sf::ContextSettings(24, 0, 16, 4, 5));
        sf::Image icon;
        icon.loadFromFile(Settings::icon_name);
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        window.setVerticalSyncEnabled(true);
        Drawing::reshapeScreen(window);

        BlockProperties::init();
        Grid::init();
        Drawing::initOpenGL();
        Drawing::initImGui(window);

        Spherical camera_dynamic(Settings::camera01_pos);
        Spherical camera_static(Settings::camera02_pos);
        Block current_block(Settings::block_init_pos, Settings::block_size, 'c'); 
        Block hint_block(Settings::block_init_pos, Settings::block_size, 'c');
        Block next_block(Settings::next_block_pos, Settings::next_block_size, 'n');
        
        sf::Clock deltaClock, fpsClock, signClock;
        std::vector<int> full_floors;
        bool running = true, reversed_movement = false;
        float delta_time;
        int fps = 0, sign = 1;
        char axis = ' ';

        while (running)
        {
            // Fps count (only in debug mode)
            #ifdef _DEBUG
                if (fpsClock.getElapsedTime().asSeconds() >= 1.0f)
                {
                    fpsClock.restart();
                    std::cout << "FPS : " << fps << "\t\r";
                    fps = 0;
                }
                else ++fps;
            #endif

            delta_time = deltaClock.getElapsedTime().asSeconds();
            if (signClock.getElapsedTime().asSeconds() >= 5.0f)
            {
                signClock.restart();
                sign = rand() % 2;
                sign = sign == 0 ? -1 : sign;
            }

            // Event loop
            sfe event;
            while (window.pollEvent(event))
            {
                ImGui::SFML::ProcessEvent(event);
                if (event.type == sfe::Closed || (event.type == sfe::KeyPressed && event.key.code == sfk::Escape)) running = false;
                if (event.type == sfe::Resized) Drawing::reshapeScreen(window);
                if (event.type == sfe::LostFocus) Settings::lost_focus = true;
                if (event.type == sfe::GainedFocus) Settings::lost_focus = false;
                if (event.type == sfe::KeyPressed && Settings::game_started)
                {
                    // Pause
                    if (event.key.code == sfk::P) { if (Settings::game_pausable) Settings::game_paused = !Settings::game_paused; Settings::game_pausable = false; }
                    if (!Settings::game_paused && !Settings::lost_focus)
                    {
                        // Rotations
                        if (event.key.code == sfk::Q && Settings::block_rotable_x) { Settings::block_rotable_x = false; current_block.rotate('X'); }
                        if (event.key.code == sfk::W && Settings::block_rotable_y) { Settings::block_rotable_y = false; current_block.rotate('Y'); }
                        if (event.key.code == sfk::E && Settings::block_rotable_z) { Settings::block_rotable_z = false; current_block.rotate('Z'); }
                    }
                }
                if (event.type == sfe::KeyReleased && Settings::game_started)
                {
                    // Pause
                    if (event.key.code == sfk::P) Settings::game_pausable = true;
                    if (!Settings::game_paused && !Settings::lost_focus)
                    {
                        // Rotations
                        if (event.key.code == sfk::Q) Settings::block_rotable_x = true;
                        if (event.key.code == sfk::W) Settings::block_rotable_y = true;
                        if (event.key.code == sfk::E) Settings::block_rotable_z = true;
                    }
                }
            }

            if (Settings::game_started)
            {
                if (!Settings::game_paused && !Settings::lost_focus)
                {
                    // Rotate game frame
                    if (!Settings::difficulty_hard)
                    {
                        if (!(sfk::isKeyPressed(sfk::Left) && sfk::isKeyPressed(sfk::Right)))
                        {
                            if (sfk::isKeyPressed(sfk::Left)) camera_dynamic.theta -= (Settings::game_frame_rotation_velocity + Settings::game_frame_added_rotation_velocity * Settings::level) * delta_time;
                            else if (sfk::isKeyPressed(sfk::Right)) camera_dynamic.theta += (Settings::game_frame_rotation_velocity + Settings::game_frame_added_rotation_velocity * Settings::level) * delta_time;
                            else camera_dynamic.theta += (Settings::game_frame_idle_rotation_velocity + Settings::game_frame_added_idle_rotation_velocity * Settings::level) * Settings::game_frame_rotation_velocity_multiplier * delta_time;
                        }
                        else camera_dynamic.theta += (Settings::game_frame_idle_rotation_velocity + Settings::game_frame_added_idle_rotation_velocity * Settings::level) * Settings::game_frame_rotation_velocity_multiplier * delta_time;
                    }
                    else camera_dynamic.theta += sign * (Settings::game_frame_idle_rotation_velocity + Settings::game_frame_added_idle_rotation_velocity * Settings::level) * Settings::game_frame_rotation_velocity_multiplier * delta_time;

                    if (camera_dynamic.theta < 0.0f) camera_dynamic.theta += 2.0f * Settings::pi;
                    else if (camera_dynamic.theta > 2.0f * Settings::pi) camera_dynamic.theta -= 2.0f * Settings::pi;

                    // Figure out the way to move block (as it depends on current theta angle)
                    if (camera_dynamic.theta > 0.25f * Settings::pi && camera_dynamic.theta < 0.75f * Settings::pi) { axis = 'x'; reversed_movement = false; }
                    else if (camera_dynamic.theta > 1.25f * Settings::pi && camera_dynamic.theta < 1.75f * Settings::pi) { axis = 'x'; reversed_movement = true; }
                    else if (camera_dynamic.theta >= 0.75f * Settings::pi && camera_dynamic.theta <= 1.25f * Settings::pi) { axis = 'z'; reversed_movement = false; }
                    else { axis = 'z'; reversed_movement = true; }

                    // Move block left/right/down
                    if (!(sfk::isKeyPressed(sfk::A) && sfk::isKeyPressed(sfk::D)))
                    {
                        if (sfk::isKeyPressed(sfk::A)) current_block.move(axis, Settings::block_velocity.x * delta_time, 'l', reversed_movement);
                        else { current_block.dx_left = current_block.size.x; current_block.dz_left = current_block.size.z; }
                        if (sfk::isKeyPressed(sfk::D)) current_block.move(axis, Settings::block_velocity.x * delta_time, 'r', reversed_movement);
                        else { current_block.dx_right = current_block.size.x; current_block.dz_right = current_block.size.z; }
                    }
                    if (sfk::isKeyPressed(sfk::S)) current_block.move('y', (Settings::block_speed_up_y_velocity + Settings::block_added_speed_up_y_velocity * Settings::level) * delta_time);
                    else current_block.move('y', (Settings::block_velocity.y + Settings::block_added_y_velocity * Settings::level) * delta_time);

                    // Things to do when block is on the ground
                    if (Grid::checkCollision(current_block))
                    {
                        current_block.pos.y += current_block.size.y;
                        Grid::setBlock(current_block);
                        if (!Grid::isGameOver())
                        {
                            current_block.copy(next_block);
                            next_block.setNew();

                            // Handle full floors
                            full_floors = Grid::getFullFloors();
                            if (!full_floors.empty())
                            {
                                switch ((int)full_floors.size())
                                {
                                    case 2: Settings::score += (int)(250 * Settings::score_multiplier); break;
                                    case 3: Settings::score += (int)(450 * Settings::score_multiplier); break;
                                    case 4: Settings::score += (int)(650 * Settings::score_multiplier); break;
                                    default: Settings::score += (int)(100 * Settings::score_multiplier);
                                }

                                for (auto floor : full_floors)
                                    Grid::moveBlocksDown(floor);
                            }

                            Settings::score += (int)(10 * Settings::score_multiplier);
                            Settings::level = Settings::score / 1000;
                        }
                        else
                            Settings::game_started = false;
                    }

                    if (Settings::show_hint) { hint_block.copy(current_block, true); Grid::setHintY(hint_block); }
                    Settings::info_text = "";
                }
                else Settings::info_text = " Game paused";
            }
            else if (!Settings::first_game) Settings::info_text = " Game over";

            // Drawing
            ImGui::SFML::Update(window, deltaClock.restart());
            Drawing::drawScene(camera_dynamic, camera_static, window.getSize(), current_block, hint_block, next_block);
            ImGui::SFML::Render(window);
            window.display();
        }
        // Clear before app is closed
        ImGui::SFML::Shutdown();
        Grid::clear();
        BlockProperties::clear();
        Settings::clear();
    }
    
    // Memory leak check (only in debug mode)
    #ifdef _DEBUG
        _CrtMemCheckpoint(&sNew);
        if (_CrtMemDifference(&sDiff, &sOld, &sNew)) _CrtMemDumpStatistics(&sDiff);
    #endif

    return 0;
}

