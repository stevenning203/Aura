#pragma once

namespace aura
{
	int aura_imgui_static_window = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	enum class AuraParse
	{
		k_object, k_light, k_camera, k_script, k_null, k_model, k_seek,
	};
}