#include "views/projects_view.hpp"
#include <imgui/widgets.hpp>
#include <core/ranges.hpp>

ProjectsView::ProjectsView(List<Project>& projects, const List<ProjectType> &types):
	m_Projects(projects),
	m_ProjectTypes(types)
{}

void ProjectsView::OnImGuiRender(){
	ImGui::Begin(StringID);
	
	for (auto [project, index] : IndexedRange(m_Projects)) {
		ImGui::PushID(index);
		ImGui::Text("%s - %s", project->Name.Data(), m_ProjectTypes[project->TypeIndex].Name.Data());
		ImGui::PopID();
	}

	ImGui::End();
}

