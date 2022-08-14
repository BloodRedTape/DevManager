#pragma once

#include "models/project.hpp"

class ProjectsView {
private:
	List<Project>& m_Projects;
	const List<ProjectType>& m_ProjectTypes;

	static constexpr const char* StringID = "ProjectsView";
public:
	ProjectsView(List<Project> &projects, const List<ProjectType> &type);

	void OnImGuiRender();
};