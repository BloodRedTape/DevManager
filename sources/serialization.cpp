#include "serialization.hpp"

using json = nlohmann::json;

void to_json(json& j, const ProjectType& p) {
	j["Name"] = p.Name;
	j["MatchExprs"] = p.MatchExprs;
	j["CleanExpr"] = p.CleanExpr;
}
void to_json(json& j, const String& p) {
	j = p.Data();
}

void from_json(const json& j, ProjectType& p) {
	p.Name = j["Name"].get<String>();
	p.MatchExprs = j["MatchExprs"].get<List<String>>();
	p.CleanExpr = j["CleanExpr"].get<String>();
}
void from_json(const json& j, String& p) {
	std::string str = j.get<std::string>();
	p = String(str.data(), str.size());
}
