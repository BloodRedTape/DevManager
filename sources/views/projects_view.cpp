#include "views/projects_view.hpp"
#include <imgui/widgets.hpp>
#include <core/ranges.hpp>

ProjectsView::ProjectsView(List<Project>& projects):
	m_Projects(projects)
{}

void ProjectsView::OnImGuiRender(){
	ImGui::Begin(StringID);
	
	for (auto [project, index] : IndexedRange(m_Projects)) {
		ImGui::PushID(index);
		ImGui::Text("%s - %s", project->Name.Data(), project->Type->Name.Data());
		ImGui::PopID();
	}

	ImGui::End();
}

