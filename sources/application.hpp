#pragma once

#include <graphics/render_window.hpp>
#include <imgui/backend.hpp>

class Application {
private:
	RenderWindow m_Window{ 1280, 720, "DevManager" , TextureFormat::Depth32};
	ImGuiBackend m_ImGuiBackend{m_Window.FramebufferPass()};
	bool m_IsFocused = true;
public:
	int Run();
private:
	void RenderImGui();

	void OnEvent(const Event& e);
};