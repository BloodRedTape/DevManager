#include "application.hpp"
#include <core/os/clock.hpp>
#include <core/os/sleep.hpp>
#include <graphics/api/gpu.hpp>

int Application::Run(){
	m_Window.SetEventsHanlder({ this, &Application::OnEvent });

	Semaphore acquire, present;
	Clock frametime_clock;
	while (m_Window.IsOpen()) {
		float dt = frametime_clock.Restart().AsSeconds();
		if (m_IsFocused) {
			m_Window.AcquireNextFramebuffer(&acquire);
			m_ImGuiBackend.NewFrame(dt, Mouse::RelativePosition(m_Window), m_Window.Size());
			RenderImGui();
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

void Application::RenderImGui(){
	ImGui::DockspaceWindow("Dock", m_Window.Size());

	ImGui::Begin("Test");
	ImGui::End();
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
