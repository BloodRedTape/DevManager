#include "application.hpp"
#include <core/os/clock.hpp>
#include <core/os/sleep.hpp>
#include <core/print.hpp>
#include <graphics/api/gpu.hpp>
#include <core/os/directory.hpp>

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
}

int Application::Run(){
	Semaphore acquire, present;
	Clock frametime_clock;

	while (m_Window.IsOpen()) {
		float dt = frametime_clock.Restart().AsSeconds();
		if (m_IsFocused) {
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

	//m_ProjectTypesView.OnImGuiRender();
	ImGui::ShowDemoWindow();
	
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
