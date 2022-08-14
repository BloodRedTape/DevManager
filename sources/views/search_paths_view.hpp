#pragma once

#include <core/list.hpp>
#include <core/string.hpp>
#include <core/defs.hpp>

class SearchPathsView {
private:
	List<String>& m_Paths;
	bool m_IsOpen = false;
	int m_Selected = InvalidIndex;

	static constexpr const char* StringID = "Search Paths Settings";
public:
	SearchPathsView(List<String>& paths);

	void OnImGuiRender();

	void Open();
};