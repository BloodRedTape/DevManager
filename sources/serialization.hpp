#pragma once

#include <nlohmann/json.hpp>
#include "models/project.hpp"


void to_json(nlohmann::json &j, const ProjectType& p);
void to_json(nlohmann::json &j, const String& p);

template <typename T>
void to_json(nlohmann::json& j, const List<T>& p) {
	for (const T& e : p)
		j.push_back(e);
}


void from_json(const nlohmann::json& j, ProjectType& p);
void from_json(const nlohmann::json& j, String& p);

template <typename T>
void from_json(const nlohmann::json& j, List<T>& p) {
	for (int i = 0; i < j.size(); i++)
		p.Add(j.at(i).get<T>());
}

