#include "application.hpp"
#include <core/os/clock.hpp>
#include <graphics/api/gpu.hpp>

int Application::Run(){
	m_Window.SetEventsHanlder({ this, &Application::OnEvent });

	Semaphore acquire, present;
	Clock frametime_clock;
	while (m_Window.IsOpen()) {
		float dt = frametime_clock.Restart().AsSeconds();
		m_Window.AcquireNextFramebuffer(&acquire);
		m_ImGuiBackend.NewFrame(dt, Mouse::RelativePosition(m_Window), m_Window.Size());
		RenderImGui();
		m_ImGuiBackend.RenderFrame(m_Window.CurrentFramebuffer(), &acquire, &present);
		m_Window.PresentCurrentFramebuffer(&present);
		m_Window.DispatchEvents();
	}

	GPU::WaitIdle();

	return 0;
}

void Application::RenderImGui(){
	ImGui::Begin("Test");
	ImGui::End();
}

void Application::OnEvent(const Event& e){
	if (e.Type == EventType::WindowClose)
		m_Window.Close();

	m_ImGuiBackend.HandleEvent(e);
}
