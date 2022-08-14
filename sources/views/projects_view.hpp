#pragma once

#include "models/project.hpp"

class ProjectsView {
private:
	List<Project>& m_Projects;

	static constexpr const char* StringID = "ProjectsView";
public:
	ProjectsView(List<Project> &projects);

	void OnImGuiRender();
};