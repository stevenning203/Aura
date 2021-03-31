#pragma once
constexpr int k_max_n_lights = 100;
constexpr int k_max_n_diffuse = 10;
constexpr int k_max_n_specular = 10;
constexpr int k_max_n_normal = 10;
constexpr int k_max_n_height = 10;
constexpr float k_pi = 3.1415926f;
constexpr double k_max_fps_inv = (double)1000 / (double)24;

namespace gloom
{
	enum class Gloonum
	{
		k_gloom_mode_freecam, k_gloom_camera_mode_disabled, k_gloom_camera_mode_limited,
		k_gloom_mouse_mode_show, k_gloom_mouse_mode_hide, k_gloom_texture_diffuse, k_gloom_texture_specular,
		k_gloom_texture_normal, k_gloom_texture_null, k_gloom_texture_missing,
	};
}