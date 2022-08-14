#pragma once

#include "imgui/widgets.hpp"
#include "models/project.hpp"
namespace ImGui {
	inline void ProjectType(const ::ProjectType& type) {
		ImGui::Text(type.Name);
		ImGui::Text(type.MatchExpr);
		ImGui::Text(type.CleanExpr);
	}
}//namespace ImGui::

class ProjectTypeInputView {
private:
	static constexpr const char* StringID = "Project Type Input";

	String Name;
	String MatchExpr;
	String CleanExpr;

	String Error;
public:

	bool OnImGuiRender();

	void Open();

	ProjectType Data() {
		return {
			Move(Name),
			Move(MatchExpr),
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