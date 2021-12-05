#pragma once

struct Settings
{
    static void clear();
    static void reset();

    // Booleans
    static bool lost_focus;
    static bool game_paused;
    static bool game_pausable;
    static bool game_started;
    static bool first_game;
    static bool difficulty_easy;
    static bool difficulty_medium;
    static bool difficulty_hard;
    static bool show_controls_description;
    static bool show_hint;
    static bool block_rotable_x;
    static bool block_rotable_y;
    static bool block_rotable_z;
    
    // Sizes
    static const sf::Vector2u window_min_size;
    static const sf::Vector3i grid_block_size;
    static const sf::Vector3i grid_block_offset_size;
    static const sf::Vector3i game_frame_block_size;
    static const sf::Vector3f game_frame_size;
    static const float block_size;
    static const float next_block_size;

    static const float menu_size_x;
    static const float button_size_x;
    static const float controls_frame_size_y;
    static const float next_block_frame_offset_size_x;
    static const float next_block_text_y_ratio;
    static const float next_block_frame_y_ratio;
    static const float menu_settings_y_ratio;
    static const float button_frame_y_ratio;
    static const float button_y_ratio;
    static const float font_size;
    static const float button_font_size;
    static const float controls_font_size;

    // Positions
    static const sf::Vector3f game_frame_pos;
    static const sf::Vector3f camera01_pos;
    static const sf::Vector3f camera02_pos;
    static const sf::Vector3f block_init_pos;
    static const sf::Vector3f next_block_pos;
    static const sf::Vector3f grid_pos;

    // Colors
    static const ImVec4 imgui_text_color;
    static const ImVec4 imgui_text_inactive_color;
    static const ImVec4 imgui_bg_color;
    static const ImVec4 imgui_border_color;
    static const ImVec4 imgui_frame_bg_color;
    static const ImVec4 imgui_frame_bg_hover_color;
    static const ImVec4 imgui_frame_bg_click_color;
    static const ImVec4 imgui_frame_bg_inactive_color;
    static const ImVec4 imgui_checkmark_color;
    static const ImVec4 imgui_checkmark_inactive_color;
    static const ImVec4 imgui_button_color;
    static const ImVec4 imgui_button_hover_color;
    static const ImVec4 imgui_button_click_color;
    static const ImVec4 imgui_button_inactive_color;

    static const sf::Vector3f game_frame_bg_color;
    static const sf::Vector3f next_block_frame_bg_color;
    static const sf::Vector3f game_frame_color;

    // Physics
    static const float game_frame_rotation_velocity;
    static const float game_frame_idle_rotation_velocity;
    static const float game_frame_added_rotation_velocity;
    static const float game_frame_added_idle_rotation_velocity;
    static const sf::Vector2f block_velocity;
    static const float block_speed_up_y_velocity;
    static const float block_added_y_velocity;
    static const float block_added_speed_up_y_velocity;
    static int game_frame_rotation_velocity_multiplier;

    // Others
    static int score;
    static int level;
    static float score_multiplier;
    static const float pi;
    static const std::string font_name;
    static const std::string texture_name;
    static const std::string icon_name;
    static std::string info_text;
    static std::vector<std::string> controls_description;
};
