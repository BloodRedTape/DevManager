#include "search_paths_view.hpp"
#include <core/ranges.hpp>
#include <core/os/dialog.hpp>
#include <core/defs.hpp>
#include "imgui/widgets.hpp"

SearchPathsView::SearchPathsView(List<String>& paths):
	m_Paths(paths)
{}

void SearchPathsView::OnImGuiRender() {
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	bool is_open = true;

	if (!ImGui::BeginPopupModal(StringID, &is_open))return;

	if (ImGui::Button("Add"))
		m_Paths.Add(Dialog::SelectFolder("Select search folder"));
	
	ImGui::SameLine();

	if (ImGui::Button("Remove") && m_Paths.IsValidIndex(m_Selected)) {
		m_Paths.UnorderedRemove(m_Selected);
		m_Selected = InvalidIndex;
	}

	if (m_Paths.Size() && ImGui::BeginTable("Paths", 1, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_RowBg)) {

		for (auto [path, index]: IndexedRange(m_Paths)) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable(path->Data(), m_Selected == index, ImGuiSelectableFlags_DontClosePopups))
				m_Selected = index;
		}
		ImGui::EndTable();
	}

	if (!is_open)
		ImGui::CloseCurrentPopup();

	ImGui::EndPopup();
}

void SearchPathsView::Open(){
	ImGui::OpenPopup(StringID);
}
