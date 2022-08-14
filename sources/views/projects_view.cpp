#include "views/projects_view.hpp"
#include <imgui/widgets.hpp>
#include <core/ranges.hpp>

ProjectsView::ProjectsView(List<Project>& projects, const List<ProjectType> &types):
	m_Projects(projects),
	m_ProjectTypes(types)
{}

void ProjectsView::OnImGuiRender(){
	ImGui::Begin(StringID);
	
	if (ImGui::BeginTable("Projects", 3, ImGuiTableFlags_RowBg)) {
		for (const auto& project : m_Projects) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text(project.Name);
			ImGui::TableNextColumn();
			ImGui::Text(m_ProjectTypes[project.TypeIndex].Name);
			ImGui::TableNextColumn();
			ImGui::Text(project.Path);
		}
		ImGui::EndTable();
	}

	ImGui::End();
}

