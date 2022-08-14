﻿#pragma once

#include <graphics/render_window.hpp>
#include <imgui/backend.hpp>
#include <core/list.hpp>
#include <core/string.hpp>
#include <core/unicode.hpp>
#include "views/project_types.hpp"
#include "views/search_paths_view.hpp"

class Application {
private:
	RenderWindow m_Window{ 1280, 720, "DevManager" , TextureFormat::Depth32};
	ImGuiBackend m_ImGuiBackend{m_Window.FramebufferPass()};
	bool m_IsFocused = true;
	List<String> m_Files;

	List<ProjectType> m_ProjectTypes;
	ProjectTypesView m_ProjectTypesView{m_ProjectTypes};

	List<String> m_SearchPaths;
	SearchPathsView m_SearchPathsView{ m_SearchPaths };
public:
	Application();

	int Run();
private:
	void OnImGuiRender();

	void OnEvent(const Event& e);
};