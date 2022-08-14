#pragma once

#include "imgui/widgets.hpp"
#include "models/project.hpp"

class ProjectTypesView {
private:
	List<ProjectType> &m_Types;
public:
	ProjectTypesView(List<ProjectType>& types);

	void OnImGuiRender();
};