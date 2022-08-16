#pragma once

#include <graphics/render_window.hpp>
#include <imgui/backend.hpp>
#include <core/list.hpp>
#include <core/string.hpp>
#include <core/unicode.hpp>
#include <core/string_print.hpp>
#include "views/project_types_view.hpp"
#include "views/search_paths_view.hpp"
#include <core/pair.hpp>

enum class LogType {
	Error,
	Info,
	Warrning
};

class Application {
	static constexpr const char* ConfigFilepath = "config.json";
private:
	RenderWindow m_Window{ 1280, 720, "DevManager" , TextureFormat::Depth32};
	ImGuiBackend m_ImGuiBackend{m_Window.FramebufferPass()};
	bool m_IsFocused = true;
	List<String> m_Files;

	List<ProjectType> m_ProjectTypes;
	ProjectTypesView m_ProjectTypesView{m_ProjectTypes};

	List<String> m_SearchPaths;
	SearchPathsView m_SearchPathsView{ m_SearchPaths };
	
	size_t m_Selected = InvalidIndex;
	List<Project> m_Projects;

	List<Pair<LogType,String>> m_Log;

	File m_LogFile{"log.txt", File::Mode::Write, true};
public:
	Application();

	~Application();

	int Run();
private:
	void OnImGuiRender();

	void OnUpdate();

	void OnEvent(const Event& e);

	void QueryProjects();

	void ClearSelected();

	void ClearAll();

	void ClearProject(const Project& project);
	
	template<typename...ArgsType>
	void Log(LogType type, const char* fmt, const ArgsType &...args) {
		String line = StringPrint(fmt, args...);
		if (m_LogFile.IsOpen()) {
			m_LogFile.Write(line.Data(), line.Size());
			m_LogFile.Write("\n", 1);
		}
		m_Log.Emplace(type, Move(line));
	}
};