#pragma once

#include <core/list.hpp>
#include <core/string.hpp>

struct ProjectType {
	String Name;
	List<String> MatchExprs;
	String CleanExpr;
};

struct Project {
	const ProjectType* Type = nullptr;
	String Name;
	String Path;
};

struct ProjectsState {
	List<ProjectType> Types;
	List<Project> Projects;
};