#pragma once

#include <regex>
#include <core/list.hpp>
#include <core/string.hpp>

struct ProjectType {
	std::regex Matcher;
	List<std::regex> Cleaners;
};

struct Project {
	ProjectType* Type = nullptr;
	String Name;
	String Path;
};

struct ProjectsState {
	List<ProjectType> Types;
	List<Project> Projects;
};