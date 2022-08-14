#pragma once

#include <core/list.hpp>
#include <core/string.hpp>
#include <core/defs.hpp>

struct ProjectType {
	String Name;
	List<String> MatchExprs;
	String CleanExpr;
};

struct Project {
	size_t TypeIndex = InvalidIndex;
	String Name;
	String Path;
};

struct ProjectsState {
	List<ProjectType> Types;
	List<Project> Projects;
};