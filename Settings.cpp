#include "pch.h"

void Settings::clear()  // Vectors and arrays need to be cleared because either opengl or sfml prevent their destructors to be called, which causes memory leaks
					    // (this solution is not perfect as there are still some minor memory leaks but it mitigates damage)
{
    controls_description = std::vector<std::string>{};
}

void Settings::reset() // Resets some values when game is started
{
    lost_focus = false;
    game_paused = false;
    game_pausable = true;
    game_started = false;
    block_rotable_x = true;
    block_rotable_y = true;
    block_rotable_z = true;

    score = 0;
    level = 0;
}

// Booleans
bool Settings::lost_focus = false;
bool Settings::game_paused = false;
bool Settings::game_pausable = true;
bool Settings::game_started = false;
bool Settings::first_game = true;
bool Settings::difficulty_easy = false;
bool Settings::difficulty_medium = true;
bool Settings::difficulty_hard = false;
bool Settings::show_controls_description = false;
bool Settings::show_hint = true;
bool Settings::block_rotable_x = true;
bool Settings::block_rotable_y = true;
bool Settings::block_rotable_z = true;

// Sizes
const sf::Vector2u Settings::window_min_size{ 780, 900 };
const sf::Vector3i Settings::grid_block_size{ 16, 27, 16 };
const sf::Vector3i Settings::grid_block_offset_size{ 3, 3, 3 };
const sf::Vector3i Settings::game_frame_block_size{ 10, 20, 10 };
const sf::Vector3f Settings::game_frame_size{ 1.0f, 2.0f, 1.0f };
const float Settings::block_size = 0.1f;
const float Settings::next_block_size = 0.3f;

const float Settings::menu_size_x = 280.0f;
const float Settings::button_size_x = 260.0f;
const float Settings::controls_frame_size_y = 230.0f;
const float Settings::next_block_frame_offset_size_x = 10.0f;
const float Settings::next_block_text_y_ratio = 0.05f;
const float Settings::next_block_frame_y_ratio = 0.3f;
const float Settings::menu_settings_y_ratio = 0.55f;
const float Settings::button_frame_y_ratio = 0.1f;
const float Settings::button_y_ratio = 0.7f;
const float Settings::font_size = 34.0f;
const float Settings::button_font_size = 40.0f;
const float Settings::controls_font_size = 28.0f;

// Positions
const sf::Vector3f Settings::game_frame_pos{ -0.5f, -1.0f, -0.5f };
const sf::Vector3f Settings::camera01_pos{ 4.0f, 1.0f, 0.4f }; // spherical coords
const sf::Vector3f Settings::camera02_pos{ 2.0f, Settings::pi / 2.0f - 0.3f, 0.3f }; // spherical coords
const sf::Vector3f Settings::block_init_pos{ -0.1f, 1.2f, -0.1f }; // left, bottom, back corner
const sf::Vector3f Settings::next_block_pos{ -0.45f, -0.35f, -0.45f }; // left, bottom, back corner
const sf::Vector3f Settings::grid_pos{ -0.8f, -1.3f, -0.8f };

// Colors
const ImVec4 Settings::imgui_text_color{ 1.0f, 1.0f, 1.0f, 1.0f };
const ImVec4 Settings::imgui_text_inactive_color{ 0.85f, 0.85f, 0.85f, 1.0f };
const ImVec4 Settings::imgui_bg_color{ 0.4f, 0.4f, 0.4f, 1.0f };
const ImVec4 Settings::imgui_border_color{ 0.4f, 0.4f, 0.4f, 1.0f };
const ImVec4 Settings::imgui_frame_bg_color{ 0.5f, 0.5f, 0.5f, 1.0f };
const ImVec4 Settings::imgui_frame_bg_hover_color{ 0.6f, 0.6f, 0.6f, 1.0f };
const ImVec4 Settings::imgui_frame_bg_click_color{ 0.55f, 0.55f, 0.55f, 1.0f };
const ImVec4 Settings::imgui_frame_bg_inactive_color{ 0.7f, 0.7f, 0.7f, 1.0f };
const ImVec4 Settings::imgui_checkmark_color{ 0.35f, 0.35f, 0.35f, 1.0f };
const ImVec4 Settings::imgui_checkmark_inactive_color{ 0.55f, 0.55f, 0.55f, 1.0f };
const ImVec4 Settings::imgui_button_color{ 0.5f, 0.5f, 0.5f, 1.0f };
const ImVec4 Settings::imgui_button_hover_color{ 0.6f, 0.6f, 0.6f, 1.0f };
const ImVec4 Settings::imgui_button_click_color{ 0.55f, 0.55f, 0.55f, 1.0f };
const ImVec4 Settings::imgui_button_inactive_color{ 0.7f, 0.7f, 0.7f, 1.0f };

const sf::Vector3f Settings::game_frame_bg_color{ 0.7f, 0.7f, 0.7f };
const sf::Vector3f Settings::next_block_frame_bg_color{ 0.6f, 0.6f, 0.6f };
const sf::Vector3f Settings::game_frame_color{ 0.0f, 0.0f, 0.0f };

// Physics
const float Settings::game_frame_rotation_velocity = 1.0f;
const float Settings::game_frame_idle_rotation_velocity = 0.7f;
const float Settings::game_frame_added_rotation_velocity = 0.1f;
const float Settings::game_frame_added_idle_rotation_velocity = 0.07f;
const sf::Vector2f Settings::block_velocity{ 0.5f, 0.2f };
const float Settings::block_speed_up_y_velocity = 1.5f;
const float Settings::block_added_y_velocity = 0.02f;
const float Settings::block_added_speed_up_y_velocity = 0.15f;
int Settings::game_frame_rotation_velocity_multiplier = 1;

// Others
int Settings::score = 0;
int Settings::level = 0;
float Settings::score_multiplier = 1.0f;
const float Settings::pi = (float)std::numbers::pi;
const std::string Settings::font_name = "monofonto.regular.otf";
// Modified version of image by antony999k under CC-BY 4.0 license : https://opengameart.org/content/tetris-tiles
const std::string Settings::texture_name = "Tiles.png";
const std::string Settings::icon_name = "Tetris3DLogo.png";
std::string Settings::info_text = "";
std::vector<std::string> Settings::controls_description {
                                                            "Esc   : Exit",
                                                            "P     : Pause game",
                                                            "S     : Speed up block",
                                                            "A/D   : Move block left/right",
                                                            "Q/W/E : Rotate block around X/Y/Z axis",
                                                            "<-/-> : Rotate frame left/right",
                                                            "        (disabled on hardmode)"
                                                        };

