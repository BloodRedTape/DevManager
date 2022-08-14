#include "views/project_types_view.hpp"

bool ProjectTypeInputView::OnImGuiRender(){
	bool res = false;

	if (ImGui::BeginPopup(StringID)) {
		bool name = ImGui::InputText("Name", Name, 0);

		for (const auto& match_expr: MatchExprs) 
			ImGui::Text(match_expr);
		bool match = ImGui::InputText("Match Expr", CurrentMatchExpr, 0);
		ImGui::SameLine();
		if (ImGui::Button("Append") && CurrentMatchExpr.Size())
			MatchExprs.Add(Move(CurrentMatchExpr));
	
		bool clean = ImGui::InputText("Clean Expr", CleanExpr, 0);

		const bool written = name || match || clean;

		if (written)
			Error = {};

		ImGui::Separator();

		if (ImGui::Button("Add")) {
			if (IsDataValid()) {
				res = true;
				ImGui::CloseCurrentPopup();
			} else {
				Error = "Error: Wrong Data";
			}
		}

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
		ImGui::Text(Error);
		ImGui::PopStyleColor();

		ImGui::EndPopup();
	}
	return res;
}

void ProjectTypeInputView::Open(){
	Name = {};
	MatchExprs = {};
	CurrentMatchExpr = {};
	CleanExpr = {};

	Error = {};

	ImGui::OpenPopup(StringID);
}

bool ProjectTypeInputView::IsDataValid() const{
	return Name.Size() && MatchExprs.Size() && CleanExpr.Size();
}

ProjectTypesView::ProjectTypesView(List<ProjectType>& types):
	m_Types(types)
{}

void ProjectTypesView::OnImGuiRender(){
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	
	bool is_open = true;

	if (!ImGui::BeginPopupModal(StringID, &is_open))return;
	
	if (ImGui::Button("New"))
		m_ProjectTypeInput.Open();
	
	if (m_ProjectTypeInput.OnImGuiRender())
		m_Types.Add(m_ProjectTypeInput.Data());

	if (ImGui::BeginTable("Types", 1, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_RowBg)) {
		for (const auto &type : m_Types) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::ProjectType(type);
		}
		ImGui::EndTable();
	}

	if (!is_open)
		ImGui::CloseCurrentPopup();
	ImGui::EndPopup();
}

void ProjectTypesView::Open() {
	ImGui::OpenPopup(StringID);
}

