#pragma once

#include "imgui/widgets.hpp"
#include "models/project.hpp"
namespace ImGui {
	inline void ProjectType(const ::ProjectType& type) {
		ImGui::Text(type.Name);
	}
}//namespace ImGui::

class ProjectTypeInputView {
private:
	static constexpr const char* StringID = "Project Type Input";

	String Name;
	List<String> MatchExprs;
	String CurrentMatchExpr;
	String CleanExpr;

	String Error;
public:

	bool OnImGuiRender();

	void Open();

	ProjectType Data() {
		return {
			Move(Name),
			Move(MatchExprs),
			Move(CleanExpr)
		};
	}

	bool IsDataValid()const;
};

class ProjectTypesView {
private:
	List<ProjectType> &m_Types;

	ProjectTypeInputView m_ProjectTypeInput;

	static constexpr const char* StringID = "Project Types Settings";
public:
	ProjectTypesView(List<ProjectType>& types);

	void OnImGuiRender();

	void Open();
};