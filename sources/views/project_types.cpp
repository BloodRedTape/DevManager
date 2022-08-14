#include "views/project_types.hpp"

ProjectTypesView::ProjectTypesView(List<ProjectType>& types):
	m_Types(types)
{}

void ProjectTypesView::OnImGuiRender(){
	ImGui::Begin("ProjectTypes");

	ImGui::End();
}
