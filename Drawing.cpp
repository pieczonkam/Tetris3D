#include "pch.h"

void Drawing::initOpenGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_SCISSOR_TEST);

    block_texture.loadFromFile(Settings::texture_name);
}

void Drawing::initImGui(sf::RenderWindow& window)
{
    ImGui::SFML::Init(window, false);

    // Set ImGui style
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = Settings::imgui_text_color;
    colors[ImGuiCol_WindowBg] = Settings::imgui_bg_color;
    colors[ImGuiCol_TitleBg] = Settings::imgui_bg_color;
    colors[ImGuiCol_TitleBgCollapsed] = Settings::imgui_bg_color;
    colors[ImGuiCol_TitleBgActive] = Settings::imgui_bg_color;
    colors[ImGuiCol_Border] = Settings::imgui_border_color;
    colors[ImGuiCol_FrameBg] = Settings::imgui_frame_bg_color;
    colors[ImGuiCol_FrameBgHovered] = Settings::imgui_frame_bg_hover_color;
    colors[ImGuiCol_FrameBgActive] = Settings::imgui_frame_bg_click_color;
    colors[ImGuiCol_CheckMark] = Settings::imgui_checkmark_color;
    colors[ImGuiCol_Button] = Settings::imgui_button_color;
    colors[ImGuiCol_ButtonHovered] = Settings::imgui_button_hover_color;
    colors[ImGuiCol_ButtonActive] = Settings::imgui_button_click_color;

    // Load font for ImGui
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF(Settings::font_name.c_str(), Settings::font_size);
    io.Fonts->AddFontFromFileTTF(Settings::font_name.c_str(), Settings::button_font_size);
    io.Fonts->AddFontFromFileTTF(Settings::font_name.c_str(), Settings::controls_font_size);
    ImGui::SFML::UpdateFontTexture();
}

void Drawing::reshapeScreen(sf::RenderWindow& window)
{
    sf::Vector2u size = window.getSize();
    int sx = size.x >= Settings::window_min_size.x ? size.x : Settings::window_min_size.x;
    int sy = size.y >= Settings::window_min_size.y ? size.y : Settings::window_min_size.y;
    window.setSize(sf::Vector2u(sx, sy));
}

void Drawing::setPerspective(float fov, float aspect_ratio, float z_min, float z_max)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect_ratio, z_min, z_max);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

sf::Vector2f Drawing::getTexPos(char color) // Block's textures are stored in one image, so there is a need to choose right position for given color
{
    int nmb_of_colors = (int)BlockProperties::color_list.size();
    sf::Vector2f tex_pos{ 0.0f, 0.0f };

    if (color == 'y') tex_pos.x = 0.0f;
    else if (color == 'r') tex_pos.x = 1.0f / nmb_of_colors;
    else if (color == 'g') tex_pos.x = 2.0f / nmb_of_colors;
    else if (color == 'm') tex_pos.x = 3.0f / nmb_of_colors;
    else if (color == 'o') tex_pos.x = 4.0f / nmb_of_colors;
    else if (color == 'c') tex_pos.x = 5.0f / nmb_of_colors;
    else if (color == 'b') tex_pos.x = 6.0f / nmb_of_colors;

    return tex_pos;
}

sf::Vector2f Drawing::getTexSize()
{
    return sf::Vector2f{ 1.0f / (int)BlockProperties::color_list.size(), 1.0f };
}

void Drawing::setButtonInactive()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Button] = Settings::imgui_button_inactive_color;
    colors[ImGuiCol_ButtonHovered] = Settings::imgui_button_inactive_color;
    colors[ImGuiCol_ButtonActive] = Settings::imgui_button_inactive_color;
}

void Drawing::setButtonActive()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Button] = Settings::imgui_button_color;
    colors[ImGuiCol_ButtonHovered] = Settings::imgui_button_hover_color;
    colors[ImGuiCol_ButtonActive] = Settings::imgui_button_click_color;
}

void Drawing::drawScene(Spherical& camera01, Spherical& camera02, sf::Vector2u size, Block& current_block, Block& hint_block, Block& next_block)
{
    glEnable(GL_DEPTH_TEST);
    drawGameFrame(camera01, size, current_block, hint_block);
    drawMenu(camera02, size, current_block, next_block);
}

void Drawing::drawGameFrame(Spherical& camera, sf::Vector2u size, Block& current_block, Block& hint_block)
{
    float sx = (float)size.x - Settings::menu_size_x;
    float sy = Settings::show_controls_description ? (float)size.y - Settings::controls_frame_size_y : (float)size.y;

    setPerspective(45.0f, sx / sy, 0.1f, 100.0f);
    glViewport(0, 0, (GLsizei)sx, (GLsizei)sy);
    glScissor(0, 0, (GLsizei)sx, (GLsizei)sy);

    glClearColor(Settings::game_frame_bg_color.x, Settings::game_frame_bg_color.y, Settings::game_frame_bg_color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    Spherical north_of_camera(camera.distance, camera.theta, camera.phi + 0.01f);
    gluLookAt(camera.getX(), camera.getY(), camera.getZ(), 0.0, 0.0, 0.0, north_of_camera.getX(), north_of_camera.getY(), north_of_camera.getZ());

    // Drawing
    if (!Settings::first_game)
    {
        drawCuboidWireframe(Settings::game_frame_pos, Settings::game_frame_size, Settings::game_frame_color, 3.0f);
        drawBlock(current_block);
        if (Settings::show_hint && Settings::game_started) drawBlock(hint_block, true);
        drawGrid();
    }
}

void Drawing::drawMenu(Spherical& camera, sf::Vector2u size, Block& current_block, Block& next_block)
{
    sf::Vector2f next_block_text_size{ Settings::menu_size_x, size.y * Settings::next_block_text_y_ratio };
    sf::Vector2f next_block_frame_size{ Settings::menu_size_x - Settings::next_block_frame_offset_size_x * 2.0f, size.y * Settings::next_block_frame_y_ratio };
    sf::Vector2f menu_settings_size{ Settings::menu_size_x, size.y * Settings::menu_settings_y_ratio };
    sf::Vector2f button_frame_size{ Settings::menu_size_x, size.y * Settings::button_frame_y_ratio };

    // OpenGL
    setPerspective(45.0f, next_block_frame_size.x / next_block_frame_size.y, 0.1f, 100.0f);
    glViewport((GLsizei)(size.x - Settings::menu_size_x + Settings::next_block_frame_offset_size_x), (GLsizei)(menu_settings_size.y + button_frame_size.y), (GLsizei)next_block_frame_size.x, (GLsizei)next_block_frame_size.y);
    glScissor((GLsizei)(size.x - Settings::menu_size_x + Settings::next_block_frame_offset_size_x), (GLsizei)(menu_settings_size.y + button_frame_size.y), (GLsizei)next_block_frame_size.x, (GLsizei)next_block_frame_size.y);

    glClearColor(Settings::next_block_frame_bg_color.x, Settings::next_block_frame_bg_color.y, Settings::next_block_frame_bg_color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    Spherical north_of_camera(camera.distance, camera.theta, camera.phi + 0.01f);
    gluLookAt(camera.getX(), camera.getY(), camera.getZ(), 0.0, 0.0, 0.0, north_of_camera.getX(), north_of_camera.getY(), north_of_camera.getZ());

    // Drawing
    if (!Settings::first_game) drawBlock(next_block);

    // ImGui
    ImVec4* colors = ImGui::GetStyle().Colors;
    // Controls frame
    if (Settings::show_controls_description)
    {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2((float)size.x - Settings::menu_size_x, Settings::controls_frame_size_y + 5.0f)); // Adding a small value to y-size resolves bug occuring during resize event
        ImGui::Begin("Controls", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
            for (auto& str : Settings::controls_description) ImGui::Text(str.c_str());
            ImGui::PopFont();
        ImGui::End();
    }

    // Next block text
    ImGui::SetNextWindowPos(ImVec2((float)size.x - Settings::menu_size_x, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(next_block_text_size.x, next_block_text_size.y + 5.0f)); // Adding a small value to y-size resolves bug occuring during resize event
    ImGui::Begin("Next block", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::Text(" Next block");
        ImGui::PopFont();
    ImGui::End();

    // Next block side frames
    ImGui::SetNextWindowPos(ImVec2((float)size.x - Settings::menu_size_x, next_block_text_size.y));
    ImGui::SetNextWindowSize(ImVec2(Settings::next_block_frame_offset_size_x, next_block_frame_size.y + 5.0f)); // Adding a small value to y-size resolves bug occuring during resize event
    ImGui::Begin("Left-side frame", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2((float)size.x - Settings::menu_size_x + next_block_frame_size.x + Settings::next_block_frame_offset_size_x, next_block_text_size.y));
    ImGui::SetNextWindowSize(ImVec2(Settings::next_block_frame_offset_size_x, next_block_frame_size.y + 5.0f)); // Adding a small value to y-size resolves bug occuring during resize event
    ImGui::Begin("Right-side frame", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
    ImGui::End();

    // Menu settings
    ImGui::SetNextWindowPos(ImVec2((float)size.x - Settings::menu_size_x, (float)size.y - menu_settings_size.y - button_frame_size.y));
    ImGui::SetNextWindowSize(ImVec2(menu_settings_size.x, menu_settings_size.y + 5.0f)); // Adding a small value to y-size resolves bug occuring during resize event
    ImGui::Begin("Settings and info", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::Text((" Score: " + std::to_string(Settings::score)).c_str());
        ImGui::Text((" Level: " + std::to_string(Settings::level)).c_str());
        ImGui::Text(Settings::info_text.c_str());
        ImGui::NewLine();
        ImGui::Text(" Difficulty:");
        ImGui::SetCursorPos(ImVec2(2 * Settings::next_block_frame_offset_size_x, ImGui::GetCursorPosY() + Settings::next_block_frame_offset_size_x));
        if (Settings::game_started)
        {
            colors[ImGuiCol_Text] = Settings::imgui_text_inactive_color;
            colors[ImGuiCol_FrameBg] = Settings::imgui_frame_bg_inactive_color;
            colors[ImGuiCol_FrameBgHovered] = Settings::imgui_frame_bg_inactive_color;
            colors[ImGuiCol_FrameBgActive] = Settings::imgui_frame_bg_inactive_color;
            colors[ImGuiCol_CheckMark] = Settings::imgui_checkmark_inactive_color;
        }
        if (ImGui::RadioButton(" Easy", Settings::difficulty_easy) && !Settings::game_started)
        { 
            Settings::difficulty_easy = true; Settings::difficulty_medium = false; Settings::difficulty_hard = false; 
            Settings::game_frame_rotation_velocity_multiplier = 0;
            Settings::score_multiplier = 0.5f;
        }
        ImGui::SetCursorPosX(2 * Settings::next_block_frame_offset_size_x);
        if (ImGui::RadioButton(" Medium", Settings::difficulty_medium) && !Settings::game_started)
        { 
            Settings::difficulty_easy = false; Settings::difficulty_medium = true; Settings::difficulty_hard = false;
            Settings::game_frame_rotation_velocity_multiplier = 1;
            Settings::score_multiplier = 1.0f;
        }
        ImGui::SetCursorPosX(2 * Settings::next_block_frame_offset_size_x);
        if (ImGui::RadioButton(" Hard", Settings::difficulty_hard) && !Settings::game_started)
        {
            Settings::difficulty_easy = false; Settings::difficulty_medium = false; Settings::difficulty_hard = true; 
            Settings::game_frame_rotation_velocity_multiplier = 1;
            Settings::score_multiplier = 2.0f;
        }
        if (Settings::game_started)
        {
            colors[ImGuiCol_Text] = Settings::imgui_text_color;
            colors[ImGuiCol_FrameBg] = Settings::imgui_frame_bg_color;
            colors[ImGuiCol_FrameBgHovered] = Settings::imgui_frame_bg_hover_color;
            colors[ImGuiCol_FrameBgActive] = Settings::imgui_frame_bg_click_color;
            colors[ImGuiCol_CheckMark] = Settings::imgui_checkmark_color;
        }
        ImGui::NewLine();
        ImGui::SetCursorPosX(1.65f * Settings::next_block_frame_offset_size_x);
        ImGui::Checkbox(" Show controls", &Settings::show_controls_description);
        ImGui::SetCursorPosX(1.65f * Settings::next_block_frame_offset_size_x);
        ImGui::Checkbox(" Show hint", &Settings::show_hint);
        ImGui::PopFont();
    ImGui::End();

    // Button frame
    ImGui::SetNextWindowPos(ImVec2((float)size.x - Settings::menu_size_x, (float)size.y - button_frame_size.y));
    ImGui::SetNextWindowSize(ImVec2(button_frame_size.x, button_frame_size.y + 5.0f)); // Adding a small value to y-size resolves bug occuring during resize event
    ImGui::Begin("Button frame", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        ImGui::SetCursorPos(ImVec2((button_frame_size.x - Settings::button_size_x) / 2.0f, (button_frame_size.y * (1.0f - Settings::button_y_ratio)) / 2.0f));
        if (!Settings::game_started)
        {
            if (ImGui::Button("Start game", ImVec2(Settings::button_size_x, button_frame_size.y * Settings::button_y_ratio)))
            {
                Settings::reset();
                Grid::reset();
                current_block.setNew();
                next_block.setNew();
                Settings::first_game = false;
                Settings::game_started = true;
            }
        }
        else
        {
            if (Settings::game_paused || Settings::lost_focus)
            {
                setButtonInactive();
                colors[ImGuiCol_Text] = Settings::imgui_text_inactive_color;
            }
            else
            {
                setButtonActive();
                colors[ImGuiCol_Text] = Settings::imgui_text_color;
            }
            if (ImGui::Button("Reset", ImVec2(Settings::button_size_x, button_frame_size.y * Settings::button_y_ratio)) && !Settings::game_paused && !Settings::lost_focus)
            {
                Settings::reset();
                Grid::reset();
                Settings::first_game = true;
            }
            colors[ImGuiCol_Text] = Settings::imgui_text_color;
        }
        
        ImGui::PopFont();
    ImGui::End();
}

void Drawing::drawCuboid(sf::Vector3f pos, sf::Vector3f size, sf::Vector2f tex_pos, sf::Vector2f tex_size) // Draws cuboid with texture
{
    // pos - left, bottom, back corner of cuboid
    float x = pos.x, y = pos.y, z = pos.z, sx = size.x, sy = size.y, sz = size.z;

    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    sf::Texture::bind(&block_texture);
    glBegin(GL_QUADS);
        glTexCoord2f(tex_pos.x + tex_size.x, tex_pos.y + tex_size.y); glVertex3f(x, y, z); glTexCoord2f(tex_pos.x, tex_pos.y + tex_size.y); glVertex3f(x + sx, y, z); glTexCoord2f(tex_pos.x, tex_pos.y); glVertex3f(x + sx, y, z + sz); glTexCoord2f(tex_pos.x + tex_size.x, tex_pos.y); glVertex3f(x, y, z + sz);
        glTexCoord2f(tex_pos.x + tex_size.x, tex_pos.y + tex_size.y); glVertex3f(x, y + sy, z); glTexCoord2f(tex_pos.x, tex_pos.y + tex_size.y); glVertex3f(x + sx, y + sy, z); glTexCoord2f(tex_pos.x, tex_pos.y); glVertex3f(x + sx, y + sy, z + sz); glTexCoord2f(tex_pos.x + tex_size.x, tex_pos.y); glVertex3f(x, y + sy, z + sz);
        glTexCoord2f(tex_pos.x + tex_size.x, tex_pos.y + tex_size.y); glVertex3f(x, y, z); glTexCoord2f(tex_pos.x, tex_pos.y + tex_size.y); glVertex3f(x + sx, y, z); glTexCoord2f(tex_pos.x, tex_pos.y); glVertex3f(x + sx, y + sy, z); glTexCoord2f(tex_pos.x + tex_size.x, tex_pos.y); glVertex3f(x, y + sy, z);
        glTexCoord2f(tex_pos.x + tex_size.x, tex_pos.y + tex_size.y); glVertex3f(x, y, z + sz); glTexCoord2f(tex_pos.x, tex_pos.y + tex_size.y); glVertex3f(x + sx, y, z + sz); glTexCoord2f(tex_pos.x, tex_pos.y); glVertex3f(x + sx, y + sy, z + sz); glTexCoord2f(tex_pos.x + tex_size.x, tex_pos.y); glVertex3f(x, y + sy, z + sz);
        glTexCoord2f(tex_pos.x + tex_size.x, tex_pos.y + tex_size.y); glVertex3f(x, y, z); glTexCoord2f(tex_pos.x, tex_pos.y + tex_size.y); glVertex3f(x, y, z + sz); glTexCoord2f(tex_pos.x, tex_pos.y); glVertex3f(x, y + sy, z + sz); glTexCoord2f(tex_pos.x + tex_size.x, tex_pos.y); glVertex3f(x, y + sy, z);
        glTexCoord2f(tex_pos.x + tex_size.x, tex_pos.y + tex_size.y); glVertex3f(x + sx, y, z); glTexCoord2f(tex_pos.x, tex_pos.y + tex_size.y); glVertex3f(x + sx, y, z + sz); glTexCoord2f(tex_pos.x, tex_pos.y); glVertex3f(x + sx, y + sy, z + sz); glTexCoord2f(tex_pos.x + tex_size.x, tex_pos.y); glVertex3f(x + sx, y + sy, z);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void Drawing::drawCuboidWireframe(sf::Vector3f pos, sf::Vector3f size, sf::Vector3f color, float thickness, bool is_offset) // Draws cuboid's edges
{
    // pos - left, bottom, back corner of cuboid

    float x, y, z, sx, sy, sz, offset = thickness / 400;
    if (is_offset) x = pos.x + offset, y = pos.y + offset, z = pos.z + offset, sx = size.x - 2 * offset, sy = size.y - 2 * offset, sz = size.z - 2 * offset;
    else x = pos.x, y = pos.y, z = pos.z, sx = size.x, sy = size.y, sz = size.z;

    Utils::glColorsf(color);
    glLineWidth(thickness);
    glBegin(GL_LINES);
        glVertex3f(x, y, z); glVertex3f(x + sx, y, z);
        glVertex3f(x, y, z); glVertex3f(x, y + sy, z);
        glVertex3f(x + sx, y + sy, z); glVertex3f(x + sx, y, z);
        glVertex3f(x + sx, y + sy, z); glVertex3f(x, y + sy, z);

        glVertex3f(x, y, z + sz); glVertex3f(x + sx, y, z + sz);
        glVertex3f(x, y, z + sz); glVertex3f(x, y + sy, z + sz);
        glVertex3f(x + sx, y + sy, z + sz); glVertex3f(x + sx, y, z + sz);
        glVertex3f(x + sx, y + sy, z + sz); glVertex3f(x, y + sy, z + sz);

        glVertex3f(x, y, z); glVertex3f(x, y, z + sz);
        glVertex3f(x + sx, y, z); glVertex3f(x + sx, y, z + sz);
        glVertex3f(x, y + sy, z); glVertex3f(x, y + sy, z + sz);
        glVertex3f(x + sx, y + sy, z); glVertex3f(x + sx, y + sy, z + sz);
    glEnd();
    glLineWidth(1.0f);
}

void Drawing::drawBlock(Block& block, bool wireframe)
{
    int x_size = (int)block.map[0][0].size(), y_size = (int)block.map[0].size(), z_size = (int)block.map.size();
    sf::Vector3f pos;
    sf::Vector2f tex_pos = getTexPos(block.color.first), tex_size = getTexSize();
    
    for (int z = 0; z < z_size; ++z)
        for (int y = 0; y < y_size; ++y)
            for (int x = 0; x < x_size; ++x)
            {
                pos = block.pos + sf::Vector3f(x * block.size.x, y * block.size.y, z * block.size.z);
                if (block.map[z][y][x])
                    if (wireframe) drawCuboidWireframe(pos, block.size, block.color.second, 2.0f, true);
                    else drawCuboid(pos, block.size, tex_pos, tex_size);
            }
};

void Drawing::drawGrid()
{
    int x_size = Settings::grid_block_size.x, y_size = Settings::grid_block_size.y, z_size = Settings::grid_block_size.z;
    sf::Vector3f pos, size{ Settings::block_size, Settings::block_size, Settings::block_size };
    sf::Vector2f tex_pos, tex_size = getTexSize();
    char color;

    for (int z = 0; z < z_size; ++z)
        for (int y = 0; y < y_size; ++y)
            for (int x = 0; x < x_size; ++x)
            {
                color = Grid::color[z][y][x].first;
                pos = Settings::grid_pos + sf::Vector3f(x * Settings::block_size, y * Settings::block_size, z * Settings::block_size);
                if (Grid::map[z][y][x] && color != ' ')
                {
                    tex_pos = getTexPos(color);
                    drawCuboid(pos, size, tex_pos, tex_size);
                }
            }
}

sf::Texture Drawing::block_texture{};