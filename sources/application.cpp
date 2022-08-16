#include "application.hpp"
#include <core/os/clock.hpp>
#include <core/os/sleep.hpp>
#include <core/print.hpp>
#include <graphics/api/gpu.hpp>
#include <core/os/directory.hpp>
#include <regex>
#include "serialization.hpp"

using json = nlohmann::json;

ImU32 LogTypeColor(LogType type) {
	switch (type)
	{
	case LogType::Error:
		return IM_COL32(255, 0, 0, 255);
	case LogType::Info:
		return IM_COL32_WHITE;
	case LogType::Warrning:
		return IM_COL32(255, 255, 0, 255);
	}
	SX_ASSERT(false);
}

Application::Application() {
	m_Window.SetEventsHanlder({ this, &Application::OnEvent });

	constexpr ImWchar s_Ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x044F, // Cyrillic
		0,
	};
	
	m_ImGuiBackend.GetIO().Fonts->Clear();
	m_ImGuiBackend.GetIO().Fonts->AddFontFromFileTTF(R"(C:\Users\E1\Desktop\Graph\Fonts\Roboto\Roboto-Medium.ttf)", 16.f, nullptr, s_Ranges);
	m_ImGuiBackend.RebuildFonts();
	
	auto config = File::ReadEntire(ConfigFilepath);

	if (!config.HasValue())return;

	json j = json::parse(config.Value().Data());
	m_ProjectTypes = j["ProjectTypes"];
	m_SearchPaths = j["SearchPaths"];

	QueryProjects();
}

Application::~Application() {
	json j;

	j["ProjectTypes"] = m_ProjectTypes;
	j["SearchPaths"] = m_SearchPaths;
	
	std::string content = j.dump(4);
	File::WriteEntire(ConfigFilepath, { content.data(), content.size() });
}

int Application::Run(){
	Semaphore acquire, present;
	Clock frametime_clock;

	while (m_Window.IsOpen()) {
		float dt = frametime_clock.Restart().AsSeconds();
		if (m_IsFocused) {
			OnUpdate();
			m_Window.AcquireNextFramebuffer(&acquire);
			m_ImGuiBackend.NewFrame(dt, Mouse::RelativePosition(m_Window), m_Window.Size());
			OnImGuiRender();
			m_ImGuiBackend.RenderFrame(m_Window.CurrentFramebuffer(), &acquire, &present);

			//GPU::Execute(nullptr, acquire, present);
			m_Window.PresentCurrentFramebuffer(&present);
		} else {
			Sleep(Milliseconds(16));
		}
		m_Window.DispatchEvents();
	}

	GPU::WaitIdle();

	return 0;
}

static bool DoesProjectTypeMatch(const String &project, const ProjectType &type) {
	for (const auto& expr : type.MatchExprs) {
		bool matched = false;
		std::regex match_expr(expr.Data());
		for (const auto& entry : Directory(project)) {
			if (std::regex_match(entry.Name.Data(), match_expr))
				matched = true;
		}
		if (!matched)
			return false;
	}

	return true;
}

void Application::OnUpdate() {

}

void Application::OnImGuiRender() {

	{
		auto window_size = m_Window.Size();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
		ImGui::SetNextWindowPos({ 0, 0 });
		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoTitleBar;
		flags |= ImGuiWindowFlags_NoResize;
		flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		flags |= ImGuiWindowFlags_NoMove;

		bool search_paths_requested = false;
		bool project_types_requested = false;

		ImGui::Begin("Dockspace", nullptr, flags);
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Settings"))
				{
					search_paths_requested = ImGui::MenuItem("Search Paths", "Config");
					project_types_requested = ImGui::MenuItem("Project Types", "Config");

					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
			ImGui::PopStyleVar();

			ImGui::DockSpace(ImGui::GetID("__Dockspace"));

			ImGui::PopStyleVar(2);
		}
		
		if (search_paths_requested)
			m_SearchPathsView.Open();
		m_SearchPathsView.OnImGuiRender();
		if (project_types_requested)
			m_ProjectTypesView.Open();
		m_ProjectTypesView.OnImGuiRender();
		ImGui::End();

	}

	ImGui::Begin("Toolbar");
	{
		if (ImGui::Button("Refresh"))
			QueryProjects();
		ImGui::SameLine();
		if (ImGui::Button("Clear Selected"))
			ClearSelected();
		ImGui::SameLine();
		if (ImGui::Button("Clear All"))
			ClearAll();
	}
	ImGui::End();


	ImGui::Begin("Projects View");
	{
		if (ImGui::BeginTable("Projects", 3, ImGuiTableFlags_RowBg)) {
			for (auto [project, index] : IndexedRange(m_Projects)) {
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				if (ImGui::Selectable(project->Name.Data(), index == m_Selected, ImGuiSelectableFlags_SpanAllColumns))
					m_Selected = index;
				ImGui::TableNextColumn();
				ImGui::Text(m_ProjectTypes[project->TypeIndex].Name);
				ImGui::TableNextColumn();
				ImGui::Text(project->Path);
			}
			ImGui::EndTable();
		}
	}
	ImGui::End();

	ImGui::Begin("Project Inspector");
	{
		if (m_Projects.IsValidIndex(m_Selected)) {
			const Project& project = m_Projects[m_Selected];

			std::regex clean_expr(m_ProjectTypes[project.TypeIndex].CleanExpr.Data());

			for (auto entry : Directory(project.Path + "/" + project.Name)) {
				bool should_be_removed = std::regex_match(entry.Name.Data(), clean_expr);

				auto color = should_be_removed ? IM_COL32(20, 200, 255, 255) : IM_COL32_WHITE;

				ImGui::PushStyleColor(ImGuiCol_Text, color);
				ImGui::Text(entry.Name);
				ImGui::PopStyleColor();
			}
		}
	}
	ImGui::End();

	ImGui::Begin("Log");
	{
		for (const auto& line : m_Log) {
			ImGui::PushStyleColor(ImGuiCol_Text, LogTypeColor(line.First));
			ImGui::Text(line.Second);
			ImGui::PopStyleColor();
		}
	}
	ImGui::End();
	
	//m_ProjectTypesView.OnImGuiRender();
	//ImGui::ShowDemoWindow();
}

void Application::OnEvent(const Event& e){
	if (e.Type == EventType::WindowClose)
		m_Window.Close();

	if (e.Type == EventType::FocusIn)
		m_IsFocused = true;
	if (e.Type == EventType::FocusOut)
		m_IsFocused = false;
	
	m_ImGuiBackend.HandleEvent(e);
}

void Application::QueryProjects() {
	m_Selected = InvalidIndex;
	m_Projects.Clear();
	for (const auto &path : m_SearchPaths) {
		for (const auto& project : Directory(path)) {
			if (!project.IsDirectory)continue;

			for (auto [type, index]: IndexedRange(m_ProjectTypes)) {
				if (DoesProjectTypeMatch(path + "/" + project.Name, *type)) {
					m_Projects.Add(Project{
						//XXX: Change to project index
						index,
						project.Name,
						path
					});
					break;
				}
			}
			
		}
	}
}

void Application::ClearSelected() {
	if (m_Projects.IsValidIndex(m_Selected))
		ClearProject(m_Projects[m_Selected]);
}

void Application::ClearAll() {
	for (const auto &project : m_Projects)
		ClearProject(project);
}

void Application::ClearProject(const Project& project) {
	Log(LogType::Info, "Clearing % project '%' at '%'", m_ProjectTypes[project.TypeIndex].Name, project.Name, project.Path);
	if (!Directory::Exists(project.Path))
		return Log(LogType::Error, "Project does not exists anymore");
	
	std::regex clean_expr(m_ProjectTypes[project.TypeIndex].CleanExpr.Data());
	String full_project_path = project.Path + "/" + project.Name;
	for (auto entry : Directory(full_project_path)) {
		bool should_be_removed = std::regex_match(entry.Name.Data(), clean_expr);
		
		if (!should_be_removed)
			continue;

		bool is_removed = false;

		String full_entry_path = full_project_path + "/" + entry.Name;

		if (entry.IsDirectory)
			is_removed = (bool)Directory::Delete(full_entry_path);
		else
			is_removed = (bool)File::Delete(full_entry_path);
			
		if (!is_removed)
			Log(LogType::Warrning, "Can't delete '%'", entry.Name);
		else
			Log(LogType::Info, "Deleted '%' %", entry.Name, entry.IsDirectory ? "directory" : "file");
	}
}
